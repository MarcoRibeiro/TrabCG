#include <array>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#define TSIZE 3

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

class primitive
{
	vector<triangle> triangulos;

	public:

		primitive() {
		}

		void addTriangle(triangle t) {
			triangulos.push_back(t);
		}	


		triangle getTriangle(int posicao)
		{
			return triangulos[posicao];
		}

		vector<triangle> getTriangulos()
		{
			vector<triangle> aux;
			for (vector<triangle>::iterator it = triangulos.begin(); it != triangulos.end(); ++it)
			{
				aux.push_back(*it);
			}
			return aux;
		}

		string toString()
		{
			stringstream ss(stringstream::in | stringstream::out);
			//ss << triangulos.size() << endl;
			for (vector<triangle>::iterator it = triangulos.begin(); it != triangulos.end(); ++it)
			{
			ss	<< it._Ptr->toString() << endl;
			}
			return ss.str();
		}

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
				//se for carater de separa��o
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
			string parsed;// , input = "text to be parsed";
			stringstream input_stringstream(str);
			stringstream ss(stringstream::in | stringstream::out);

			while (getline(input_stringstream, parsed, '\n'))
			{
				ss << parsed << '\n';  // adiciona '\n' para k fun��o de parsing addtrianglo detecte o fim. MELHORAR!!
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
};

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
		vector<primitive> aux;
		for (vector<primitive>::iterator it = primitivas.begin(); it != primitivas.end(); ++it)
		{
			aux.push_back(*it);
		}
		return aux;
	}
};