#include "transf.h"
#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <GL/glut.h>


using namespace std;

//#define TSIZE 3


class color {
	float cr;
	float cg;
	float cb;

public:
	color() : cr(1.0), cg(1.0), cb(1.0) {};

	color(float r, float g, float b) : cr(r), cg(g), cb(b) {}

	float R() { return cr; }
	float G() { return cg; }
	float B() { return cb; }

	void setColor(float r, float g, float b) { cr = r; cg = g; cb = b; }
	string toString() {
		stringstream ss(stringstream::in | stringstream::out);
		ss << "|" << cr << "|" << cg << "|" << cb;
		return ss.str();
	}
};
class vertex {
	float px;
	float py;
	float pz;

public:
	vertex() : px(0), py(0), pz(0) {};
	vertex(float x, float y, float z) : px(x), py(y), pz(z) {}


	float getX() { return px; }
	float getY() { return py; }
	float getZ() { return pz; }

	string toString() {
		stringstream ss(stringstream::in | stringstream::out);
		ss << px << "," << py << "," << pz;
		return ss.str();
	}

	vertex operator =(vertex v)
	{
		float x, y, z;
		x = v.getX();
		y = v.getY();
		z = v.getZ();
		vertex temp(x, y, z);
		return temp;
	}
};
class triangle {
	vertex p1, p2, p3;
	color cor;

public:

	triangle() : p1 (), p2 (), p3 () {}
	triangle(vertex x, vertex y, vertex z) : 
		p1(x.getX(), x.getY(), x.getZ()),
		p2(y.getX(), y.getY(), y.getZ()),
		p3(z.getX(), z.getY(), z.getZ()) {}

	vertex getP1() { return p1; }
	vertex getP2() { return p2;	}
	vertex getP3() { return p3; }

	void setColor(float r, float g, float b) {
		cor.setColor(r, g, b);
	}

	triangle operator =(triangle t) {
		triangle aux(t.getP1(), t.getP2(), t.getP3());
		aux.setColor(t.cor.R(), t.cor.G(), t.cor.B());
		return aux;
	}

	float getColorR() {	return cor.R(); }
	float getColorG() { return cor.G(); }
	float getColorB() { return cor.B(); }

	string toString()
	{
		stringstream ss(stringstream::in | stringstream::out);
		
		ss << p1.toString() << ";" << p2.toString() << ";" << p3.toString() << cor.toString();
		return ss.str();
	}
};


class primitive : public drawable
{
	vector<triangle> triangulos;

	public:

		primitive() { setType(TYPE_PRIMITIVE); }
		primitive(string file) {
			loadFile(file);
			setType(TYPE_PRIMITIVE);
		}

		void addTriangle(triangle t) {
			triangulos.push_back(t);
		}	

		// Passa copia de todos os triangulos  // PASSAR POR REFERENCIA ????
		vector<float> getPontos()
		{
			vector<float> vertices;
			for (vector<triangle>::iterator it = triangulos.begin(); it != triangulos.end(); ++it)
			{
				vertices.push_back(it._Ptr->getP1().getX());
				vertices.push_back(it._Ptr->getP1().getY());
				vertices.push_back(it._Ptr->getP1().getZ());

				vertices.push_back(it._Ptr->getP2().getX());
				vertices.push_back(it._Ptr->getP2().getY());
				vertices.push_back(it._Ptr->getP2().getZ());

				vertices.push_back(it._Ptr->getP3().getX());
				vertices.push_back(it._Ptr->getP3().getY());
				vertices.push_back(it._Ptr->getP3().getZ());
			}
			return vertices;
		}


		//Usado para guardar primitivas
		string toString()
		{
			stringstream ss(stringstream::in | stringstream::out);
			for (vector<triangle>::iterator it = triangulos.begin(); it != triangulos.end(); ++it)
			{
				ss	<< it._Ptr->toString() << endl;
			}
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

		// Leitura de ficheiros com primitivas
		void addTriangle(string str)
		{
			stringstream ss(stringstream::in | stringstream::out);
			std::string::size_type sz;
			

			vector<vertex> p;
			
			float c[3], cor[3];
			int i=0, j=0, k=0, index = 0;

			while (str[index] != '|') 
			{
				// se for conteudo
				if ((str[index] != ',') && (str[index] != ';'))
				{
				ss << str[index];
				}
				//se for carater de separação
				else
				{
					c[i++] = std::stof(ss.str(), &sz);
					ss.str(std::string()); // clear buffer
					if (i == 3)  //adicionar teste de ; no caracter seguinte
					{
						p.push_back(vertex(c[0], c[1], c[2]));
						c[0] = 0.0; c[1] = 0.0; c[2] = 0.0; // clear buffer
						i = 0;
					}
				}
					 
				index++;
			}
			c[i] = std::stof(ss.str(), &sz); //adiciona o ultimo, MELHORAR !!!
			p.push_back(vertex(c[0], c[1], c[2])); //adiciona o ultimo, MELHORAR !!!

			ss.str(std::string());
			index++; // salta caracter |

			while ((str[index] != '\n'))
			{
				if (str[index] != '|') 
					ss << str[index];
				else
				{
					cor[k++] = std::stof(ss.str(), &sz);
					ss.str(std::string());
				}
				index++;
			}
			cor[k] = std::stof(ss.str(), &sz);
			triangle t(p.at(0), p.at(1), p.at(2));
			t.setColor(cor[0], cor[1], cor[2]);
			addTriangle(t);

		}
		void addTriangulos(string str)
		{
			string parsed;
			stringstream input_stringstream(str);
			stringstream ss(stringstream::in | stringstream::out);

			while (getline(input_stringstream, parsed, '\n'))
			{
				ss << parsed << '\n';  // adiciona '\n' para k função de parsing addtrianglo detecte o fim. MELHORAR!!
				addTriangle(ss.str());
				ss.str(std::string());
			}
			int f = 0;
		}
		int loadFile(string file)  // retorna 1 caso leitura com sucesso 0 se falha
		{
			stringstream content(stringstream::in | stringstream::out);
			string line;
			ifstream myfile(file);
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					content << line << "\n";
				}
				myfile.close();
			}
			else return 0 ;
			addTriangulos(content.str());

			return 1;
		}	

		//Imprime em OpenGL
		void draw(){
			for (vector<triangle>::iterator it = triangulos.begin(); it != triangulos.end(); ++it)
			{
				triangle* a = it._Ptr;
				glBegin(GL_TRIANGLES);
				glColor3f(a->getColorR(), a->getColorG(), a->getColorB());
				glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
				glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
				glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
				glEnd();
			}
		}

};



class cena {
	vector<drawable*> itens;
	vector<primitive*> primitivas;
	int n_primitivas = 0;

public:
	cena() {};

	void addPrimitiva(primitive* p) {
		itens.push_back(p);
		primitivas.push_back(p);
		n_primitivas++;
	}

	void addTransf(transf* t) {
		itens.push_back(t);
	}

	int getN_primitivas() { return n_primitivas;  }

	vector<drawable*> getItens() {
		return itens;
	}

	vector<primitive*> getPrimitivas()
	{
		return primitivas;
	}


	
};



//OBSOLETO
class scene {
	vector<primitive> primitivas;
public:

	void addprimitiva(primitive p)
	{
		primitivas.push_back(p);
	}
	void addprimitiva(string file)
	{
		primitive p;
		p.loadFile(file);
		primitivas.push_back(p);
	}
	vector<primitive> getPrimitivas()
	{
		/*
		vector<primitive> aux;
		for (vector<primitive>::iterator it = primitivas.begin(); it != primitivas.end(); ++it)
		{
			aux.push_back(*it);
		}
		return aux;*/
		return primitivas;
	}
};