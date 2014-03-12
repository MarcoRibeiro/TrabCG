#include <array>
#include <vector>

using namespace std;

#define TSIZE 3

class color {
	float cr;
	float cg;
	float cb;

public:
	color() : cr(0.0), cg(0.0), cb(0.0) {};

	color(float r, float g, float b) : cr(r), cg(g), cb(b) {}

	float R() { return cr; }
	float G() { return cg; }
	float B() { return cb; }

	void setColor(float r, float g, float b) { cr = r; cg = g; cb = b; }
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
};

class triangles
{
	vector<triangle> triangulos;

	public:

		triangles() {
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

};