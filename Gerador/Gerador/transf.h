#include <array>
#include <vector>

#include <GL/glut.h>
#include "catmull-rom.h"


#define TYPE_TRANSLACAO 1
#define TYPE_ROTACAO 2
#define TYPE_ESCALA 3
#define TYPE_PPMATRIX 4
#define TYPE_PSMATRIX 5
#define TYPE_PRIMITIVE 6
#define TYPE_ROTACAOT 7
#define TYPE_TRANSLACAOT 8

using namespace std;

extern bool tanslationLines;

// class abstrata para objectos desenhaveis
class drawable {
	short tipo;
public:

	void setType(short tp) { tipo = tp; }
	short type() { return tipo; }

	virtual void draw() {}
	

};


// Classe base das transformações geométricas
class transf : public drawable {
	float Pontox = 0, Pontoy = 0, Pontoz = 0, ang = 0;
public:
	void set_values(float px, float py, float pz, short tipo)
	{
		Pontox = px;
		Pontoy = py;
		Pontoz = pz;
		setType(tipo);
	}

	float x() { return Pontox; }
	float y() { return Pontoy; }
	float z() { return Pontoz; }

	void set_angulo(float angulo) { ang = angulo; }
	float angulo() { return ang; } // tentar melhorar de forma que o angulo apenas faca parte da class rotacao
};


class ppMatrix : public transf {

public:
	ppMatrix() { setType(TYPE_PPMATRIX); }

	void draw()	{
		glPopMatrix();
	}
};


class psMatrix : public transf {

public:
	psMatrix() 	{ setType(TYPE_PSMATRIX); }

	void draw() {
		glPushMatrix();
	}
};


class translacao : public transf {

public:
	translacao(float px, float py, float pz) {
		set_values(px, py, pz, TYPE_TRANSLACAO);
	}

	void draw(){
		glTranslatef(transf::x(), transf::y(), transf::z());
	}
};

class translacaoT : public transf {
	vector<float> curva;
	float tmp;


public:
	translacaoT(vector<float> c, float t)
	{
		curva = c;
		tmp = t;
		setType(TYPE_TRANSLACAOT);
	}

	void draw(){

		static float a = 0;
		float res[3];
		int elapsed_time;
		float racio, delta;

		float** pontos;
		int npontos = curva.size() / 3;

		pontos = (float**)malloc((npontos)*sizeof(float*));
		for (int j = 0; j < npontos; j++)	{
			pontos[j] = (float*)malloc(3 * sizeof(float));  // 3-> número de pontos por vertice
		}

		int m = 0;
		for (int i = 0; i < npontos; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				pontos[i][j] = curva[m++];
			}
		}

		////////////////////////

		//exibir linhas de translacao
		if (tanslationLines == true)
			renderCatmullRomCurve(pontos,npontos);
	

		elapsed_time = glutGet(GLUT_ELAPSED_TIME);
		racio = (elapsed_time / (tmp*1000.0f));
		delta = 360 * racio;


		getGlobalCatmullRomPoint(pontos,npontos,racio, res);  // busca os pontos para posicionar o bule
		glTranslatef(res[0], res[1], res[2]);

	}
};

class rotacao : public transf {

public:
	rotacao(float angulo, float eixoX, float eixoY, float eixoZ)
	{
		set_values(eixoX, eixoY, eixoZ, TYPE_ROTACAO);
		set_angulo(angulo);
	}
	void draw(){
		glRotatef(transf::angulo(), transf::x(), transf::y(), transf::z());
	}
};

class rotacaoT : public transf {
	float tmp;

public:
	rotacaoT(float tempo, float eixoX, float eixoY, float eixoZ)
	{
		set_values(eixoX, eixoY, eixoZ, TYPE_ROTACAOT);
		tmp = tempo;	
	}

	void draw(){
		int elapsed_time;
		float racio;
		float delta;

		elapsed_time = glutGet(GLUT_ELAPSED_TIME);
		racio = (elapsed_time / (tmp*1000.0f));
		delta = 360 * racio;

		glRotatef(delta, transf::x(), transf::y(), transf::z());
	}
};

class escala : public transf {

public:
	escala(float px, float py, float pz)
	{
		set_values(px, py, pz, TYPE_ESCALA);
	}
	void draw(){
		glScalef(transf::x(), transf::y(), transf::z());
	}
};
