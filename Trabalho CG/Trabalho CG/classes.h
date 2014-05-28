#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <GL/glut.h>
#include "C:\Users\Nelson\Documents\GitHub\TrabCG\Trabalho CG\Trabalho CG\transf.h"

using namespace std;

#define TYPE_DIFFUSE 1;
#define TYPE_EMISSION 2;
#define TYPE_AMBIENT 3;
#define TYPE_SPECULAR 4;
#define TYPE_AMB_AND_DIFF 5;
#define TYPE_SHININESS 6;
#define POINT 7;
#define DIRECTIONAL 8;
#define SPOTLIGHT 9;

class vertex {

	float p[3]; //coordenadas
	float n[3]; //normais
	float t[2]; //coordenadas textura 2d


public:
	vertex() {
		p[0] = 0.0f; p[1] = 0.0f; p[2] = 0.0f;
		n[0] = 0.0f; n[1] = 0.0f; n[2] = 0.0f;
		t[0] = 0.0f; t[1] = 0.0f;
	}

	vertex(float x, float y, float z) {
		p[0] = x; p[1] = y; p[2] = z;
		n[0] = 0.0f; n[1] = 0.0f; n[2] = 0.0f;
		t[0] = 0.0f; t[1] = 0.0f;
	}

	vertex(float x, float y, float z, float nx, float ny, float nz)
	{
		p[0] = x; p[1] = y; p[2] = z;
		setNormal(nx, ny, nz);
		setText2D(0.0f, 0.0f);
	}

	void setVertex(float x, float y, float z) {
		p[0] = x; p[1] = y; p[2] = z;
	}

	void setNormal(float x, float y, float z) {
		n[0] = x; n[1] = y; n[2] = z;
	}

	void setText2D(float x, float y) {
		t[0] = x;
		t[1] = y;
	}

	float getX() { return p[0]; }
	float getY() { return p[1]; }
	float getZ() { return p[2]; }

	float getNx() { return n[0]; }
	float getNy() { return n[1]; }
	float getNz() { return n[2]; }

	float getTx() { return t[0]; }
	float getTy() { return t[1]; }

	string toString() {
		stringstream ss(stringstream::in | stringstream::out);
		ss << p[0] << "," << p[1] << "," << p[2] << "|" << n[0] << "," << n[1] << "," << n[2] << "|" << t[0] << "," << t[1] << "|" ;
		return ss.str();
	}

};


class primitiveVBO : public drawable
{
	vector<vertex> pontos;
	vector<int> indices;
	int buffer; //index do buffer em que vai ficar guardada

public:

	primitiveVBO() { setType(TYPE_PRIMITIVE); }

	primitiveVBO(string file) {
		loadFile(file);
		setType(TYPE_PRIMITIVE);
	}

	void setBuffer(int b)
	{
		buffer = b;
	}

	int getBuffer() {
		return buffer;
	}
	/*
	Calcula normal dados tres pontos // NÃO UTILZADO
	*/
	float* calculaNormal(vertex p1, vertex p2, vertex p3) {

		float *va = (float*)malloc(sizeof(float)* 3); //vector a=p1p2
		float *vb = (float*)malloc(sizeof(float)* 3); //vector  b=p2p3
		float *res = (float*)malloc(sizeof(float)* 3); // resultado

		va[0] = p2.getX() - p1.getX();
		vb[0] = p3.getX() - p2.getX();

		va[1] = p2.getY() - p1.getY();
		vb[1] = p3.getY() - p2.getY();

		va[2] = p2.getZ() - p1.getZ();
		vb[2] = p3.getZ() - p2.getZ();


		float x = (va[1] * vb[2]) - (va[2] * vb[1]);		//produto externo
		float y = -1 * ((va[0] * vb[2]) - (va[2] * vb[0]));
		float z = (va[0] * vb[1]) - (va[1] * vb[0]);


		float length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); //comprimento vector

		x == 0 ? res[0] = 0 : res[0] = x / length;   //Normalização
		y == 0 ? res[1] = 0 : res[1] = y / length;
		z == 0 ? res[2] = 0 : res[2] = z / length;


		free(va);
		free(vb);

		return res;
	}

	
	void addPonto(vertex v) {
		pontos.push_back(v);
	}

	
	void addTriangulo(int i, int j, int k)
	{
		indices.push_back(i); indices.push_back(j); indices.push_back(k);
	}

	vector<vertex> getPontos()
	{
		return pontos;
	}

	vector<int> getIndices()
	{
		return indices;
	}


	//Usado para guardar primitivas VBO c/ indices
	string toString()
	{
		stringstream ss(stringstream::in | stringstream::out);

		ss << pontos.size() << endl;  //Numero de vertices
		for (vector<vertex>::iterator it = pontos.begin(); it != pontos.end(); ++it)
		{
			ss << it._Ptr->toString() << endl;
		}
		
		ss << indices.size() << endl;  //Numero de indices
		for (int i = 0; i < (int)indices.size(); i++)
		{
			ss << indices[i] << ",";
		}
		ss << endl;
		
		return ss.str();
	}

	int saveFile(string file)
	{
		ofstream f(file);
		if (f.is_open())
		{
			f << toString();
			f.close();
		}
		else return 0;

		return 1;
	}


	int addIndices(string str) {
		stringstream ss(stringstream::in | stringstream::out);
		string::size_type sz;

		int index = 0;
		int pos = 0;

		while (str[index] != '\0')
		{
			// se for conteudo
			if ((str[index] != ',') && (str[index] != '\n'))
			{
				ss << str[index];
			}
			//se for carater de separação
			else
			{
				indices.push_back(std::stoi(ss.str(), &sz));
				ss.str(std::string()); // clear buffer
			}
			index++;
		}
		return 0;
	}

	int addVertice(string str) {
		stringstream ss(stringstream::in | stringstream::out);
		string::size_type sz;

		int stop = 3; // caso de paragem;
		int modo = 0; //ponto, normal ou textura
		int index = 0;
		int pos = 0;
		float p[3];
		vertex vertice;

		while (str[index] != '\0')
		{
			// se for conteudo
			if (str[index] != ',' && str[index] != '|' && str[index] != '\n')
			{
				ss << str[index];
			}
			//se for carater de separação
			else
			{
				p[pos++] = std::stof(ss.str(), &sz);
				ss.str(std::string()); // clear buffer
				if (pos == stop)  //Numero coordenadas por vertice
				{
					pos = 0;
					if (modo == 0) {
						vertice.setVertex(p[0], p[1], p[2]); modo++;
					}
					else if (modo == 1) {
						vertice.setNormal(p[0], p[1], p[2]); modo++; stop = 2;
					}
					else if (modo == 2)
						vertice.setText2D(p[0], p[1]);
				}
			}
			index++;
		}
		pontos.push_back(vertice);

		return 0;
	}


	int loadFile(string file)  // retorna 1 caso leitura com sucesso 0 se falha
	{
		stringstream content(stringstream::in | stringstream::out);
		string line;
		ifstream myfile(file);
		int npontos,nindices;

		if (myfile.is_open())
		{
			//Lê numero de pontos
			if (getline(myfile, line))
			{
				try	{ npontos = stoi(line); }
				catch (invalid_argument) { /*error_message(0);*/ return 1; }
			}
			else { /*error_message(0);*/ return 1; }

			//Le pontos
			for (int j = 0; j < npontos; j++)	{
				if (getline(myfile, line)) {
					addVertice(line);
				}
				else { /*error_message(1);*/ return 1; }
			}

			//le numero de indices
			if (getline(myfile, line))
			{
				try	{ nindices = stoi(line); }
				catch (invalid_argument) { /*error_message(2)*/; return 1; }
			}
			else { /*error_message(2);*/ return 1; }

			//lê indices
			if (getline(myfile, line)) {
					addIndices(line);
			}
				else { /*error_message(1);*/ return 1; }
		}
		else { /*error_message(5);*/ return 1; }

		myfile.close();
		return 0;
	}


};

class material {
	float mr, mg, mb, ma;
	int type;
	float shininess;

	public:
		material()
		{
			type = TYPE_DIFFUSE;
			mr = -1; mg = -1; mb = -1; ma = -1;
		}

		material(int t, float r, float g, float b, float a)
		{
			type = t;
			mr = r; mg = g; mb = b; ma = a;
		}
		material(int t, float shi)
		{
			type = t;
			mr = -1; mg = -1; mb = -1; ma = -1;
			shininess = shi;
		}

		float getR() { return mr; }
		float getG() { return mg; }
		float getB() { return mb; }
		float getA() { return ma; }
		float getShininess() { return shininess; }
		int getType() { return type;  }
};

class modelo : public drawable{

	string textura;
	string nome;
	int id;
	short type;
	int buffer,btext; //buffer associado a pontos, normais e coordenadas, e buffer texid
	vector<material> mat;
	vector<transf*> transformacoes;

public:

	modelo(string n, int i)
	{
		nome = n;
		textura = "";
		id = i;
		buffer = -1;
		btext = -1;
		setType(TYPE_PRIMITIVE);
	}

	modelo(string n, int i, string t)
	{
		nome = n;
		textura = t;
		id = i;
		buffer = -1;
		btext = -1;
		setType(TYPE_PRIMITIVE);
	}

	modelo(string n, int i, string t, vector<material> m)
	{
		nome = n;
		textura = t;
		id = i;
		buffer = -1;
		btext = -1;
		setType(TYPE_PRIMITIVE);
		mat = m;
	}

	modelo(string n, int i, vector<material> m)
	{
		nome = n;
		textura = "";
		id = i;
		buffer = -1;
		btext = -1;
		setType(TYPE_PRIMITIVE);
		mat = m;
	}

	void addTransf(transf* t)
	{
		transformacoes.push_back(t);
	}
	void addTransf(vector<transf*> t)
	{
		transformacoes = t;
	}
	
	string getNome() {
		return nome;
	}

	void setBuffer(int b){
		buffer = b;
	}

	void setBufferText(int b){
		btext = b;
	}
	
	int getBuffer() {
		return buffer;
	}
	int getBufferText() {
		return btext;
	}

	int getId() {
		return id;
	}

	void setTextura(string t)
	{
		textura = t;
	}
	vector<material> getMateriais() { return mat; }


	

	string getTextura()
	{
		return textura;
	}
};

class light {

	GLfloat color[3];
	GLfloat pos[4];	
	GLfloat spot_direction[3];
	GLenum type; // 1->Diffuse 3->Ambient 4->Specular
	int mode; // 7 -> Point 8->Directional 9->Spot light 
	


public:
	light()
	{
		//defaults
		type = GL_DIFFUSE;// TYPE_DIFFUSE;
		mode = DIRECTIONAL;
		for(int i=0;i<3;i++) color[i] = -1;
		for (int i = 0; i<4; i++) pos[i] = 0;
		
	}

	light(GLenum t, int m, GLfloat r, GLfloat g, GLfloat b)
	{
		type = t;
		mode = t;
		color[0] = r; color[1] = g; color[2] = b;
		for (int i = 0; i<4; i++) pos[i] = 0;
		for (int i = 0; i<4; i++) spot_direction[i] = 0;
	}
	light(GLenum t, int m, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z)
	{
		type = t;
		mode = m;
		color[0] = r; color[1] = g; color[2] = b;
		for (int i = 0; i<4; i++) spot_direction[i] = 0;
		setPosition(m, x, y, z);
	}
	light(GLenum t, int m, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz)
	{
		type = t;
		mode = m;
		color[0] = r; color[1] = g; color[2] = b;
		spot_direction[0] = sx; spot_direction[1] = sy; spot_direction[2] = sz;
		setPosition(m, x, y, z);
	}

	void setPosition (int m, GLfloat x, GLfloat y, GLfloat z)
	{
		pos[0] = x; pos[1] = y; pos[2] = z; mode = m;
		(m == 8) ? pos[3] = 0 : pos[3] = 1;
	}


	GLfloat* getPos() { return pos; }
	GLfloat* getSpotLightDirection() { return spot_direction; }
	GLfloat* getColor() { return color; }
	int getMode() { return mode; }
	GLenum getType() { return type; }

};

class cena {
	vector<modelo*> modelos;
	vector<drawable*> itens;
	vector<transf*> transformacoes;
	vector<primitiveVBO*> primitivas;
	vector<light*> luzes;
	int n_primitivas = 0;


public:
	cena() {};

	int addPrimitiva(primitiveVBO* p) {
		primitivas.push_back(p);
		n_primitivas++;
		return n_primitivas - 1;
	}
	void addTransf(transf* t) {
		itens.push_back(t);
		transformacoes.push_back(t);
	}
	void addModelo(modelo* mod) {
		itens.push_back(mod);
		modelos.push_back(mod);
	}
	void addLight(light* l) { luzes.push_back(l); }

	int getN_primitivas() { return n_primitivas;  }
	int getNtexturas()
	{
		int i = 0;
		for (vector<modelo*>::iterator it = modelos.begin(); it != modelos.end(); it++)
		{
			if ((*it._Ptr)->getTextura() != "") i++;
		}
		return i;
	}


	/*
	Pesquisa nos modelos a existencia de uma determinada primitiva, se sim, retorna id, caso contrario retorna -1
	*/
	int exists(string nome)
	{
		int i = 0;
		for (vector<modelo*>::iterator it = modelos.begin(); it != modelos.end(); it++)
		{
			if ((*it._Ptr)->getNome() == nome) return (*it._Ptr)->getId();
			i++;
		}
		return -1;
	}
	
	vector<drawable*> getItens() { return itens; }
	vector<primitiveVBO*> getPrimitivas() {	return primitivas; }
	vector<modelo*> getModelos() {	return modelos;	}
	vector<light*> getLights() { return luzes;  }


	
};

