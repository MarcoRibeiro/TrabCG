
#include <GL/glut.h>


#define TYPE_TRANSLACAO 1
#define TYPE_ROTACAO 2
#define TYPE_ESCALA 3


// class abstrata para objectos desenhaveis
class drawable {
public:

	virtual void draw()	{ }
};


// Classe base das transformações geométricas
class transf : public drawable {
	float Pontox = 0, Pontoy = 0, Pontoz = 0, ang = 0;
	int tipo;
public:
	void set_values(float px, float py, float pz, short type)
	{
		Pontox = px;
		Pontoy = py;
		Pontoz = pz;
		tipo = type;
	}
	void set_angulo(float angulo) { ang = angulo; }

	float x() { return Pontox; }
	float y() { return Pontoy; }
	float z() { return Pontoz; }
	int type() { return tipo; }
	float angulo() { return ang; } // tentar melhorar de forma que o angulo apenas faca parte da class rotacao
};


class ppMatrix : public transf {

public:
	ppMatrix() {}

	void draw()	{
		glPopMatrix();
	}
};


class psMatrix : public transf {

public:
	psMatrix() 	{	}

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
