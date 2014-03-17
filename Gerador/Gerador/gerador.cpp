#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include "classes.h"

primitive drawPlane(float largura, float comprimento)
{
	primitive p;
	float l = largura / 2;
	float c = comprimento / 2;

	vertex p1(-c, -l, 0);
	vertex p2(c, -l, 0);
	vertex p3(c, l, 0);
	vertex p4(-c, l, 0);
	triangle t1(p1, p2, p3);	t1.setColor(1, 0, 0);
	triangle t2(p3, p4, p1);	t2.setColor(1, 0, 0);

	p.addTriangle(t1);
	p.addTriangle(t2);

	return p;
}
primitive drawCube(float size) {
	primitive p;
	float c = size / 2;

	vertex p1(-c, c, c);
	vertex p2(-c, -c, c);
	vertex p3(c, -c, c);
	vertex p4(c, c, c);
	vertex p5(c, c, -c);
	vertex p6(c, -c, -c);
	vertex p7(-c, -c, -c);
	vertex p8(-c, c, -c);

	p.addTriangle(triangle(p1, p2, p4));
	p.addTriangle(triangle(p2, p3, p4));
	p.addTriangle(triangle(p4, p3, p5));
	p.addTriangle(triangle(p3, p6, p5));
	p.addTriangle(triangle(p1, p4, p5));
	p.addTriangle(triangle(p1, p5, p8));
	p.addTriangle(triangle(p8, p7, p2));
	p.addTriangle(triangle(p2, p1, p8));
	p.addTriangle(triangle(p8, p5, p6));
	p.addTriangle(triangle(p6, p7, p8));
	p.addTriangle(triangle(p2, p7, p6));
	p.addTriangle(triangle(p6, p3, p2));

	return p;
}
primitive drawCilinder(float n_lados, float altura, float raio)
{
	primitive p;
	float delta = 2 * M_PI / n_lados;
	for (int count = 0; count < n_lados; count++) {

		//glColor3f(1, 0, 0);

		vertex a(0, altura / 2, 0);  //O
		vertex b(raio*sin((delta)*(count)), altura / 2, raio*cos((delta)*(count))); //P
		vertex c(raio*sin((delta)*(count + 1)), altura / 2, raio*cos((delta)*(count + 1)));  //Q

		//glColor3f(0, 1, 0);
		vertex d(raio*sin((delta)*(count)), altura / 2, raio*cos((delta)*(count))); //P
		vertex e(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count)));  //P'
		vertex f(raio*sin((delta)*(count + 1)), altura / 2, raio*cos((delta)*(count + 1)));  //Q

		vertex g(raio*sin((delta)*(count + 1)), altura / 2, raio*cos((delta)*(count + 1)));  //Q
		vertex h(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count)));  //P'
		vertex i(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  // Q'

		//glColor3f(1, 0, 0);
		vertex j(0, -altura / 2, 0);  // O'
		vertex k(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  // Q'
		vertex l(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count)));  //P'

		triangle t1(a, b, c); t1.setColor(1, 0, 0);
		triangle t2(d, e, f); t2.setColor(0, 1, 0);
		triangle t3(g, h, i); t3.setColor(0, 1, 0);
		triangle t4(j, k, l); t4.setColor(1, 0, 0);
		p.addTriangle(t1); p.addTriangle(t2); p.addTriangle(t3); p.addTriangle(t4);
	}
	return p;
}
primitive drawCone(float n_lados, float altura, float raio)
{
	primitive p;
	float delta = 2 * M_PI / n_lados;
	for (int count = 0; count < n_lados; count++) {
		vertex a(0, -altura / 2, 0);  //O
		vertex b(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  //Q
		vertex c(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); //P

		vertex d(0, altura / 2, 0);  //O
		vertex e(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); //P
		vertex f(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  //Q

		triangle t1(a, b, c);	t1.setColor(1, 0, 0);
		triangle t2(d, e, f);	t2.setColor(0.5, 0, 0);

		p.addTriangle(t1);
		p.addTriangle(t2);
	}
	return p;
}

primitive triangulos;

void iniciaClass()
{

	triangle a(vertex(-.5, 0.0, 0.5), vertex(0.5, 0.0, 0.5), vertex(0.0, 1.5, 0.0));    // p1 p2 p5
	a.setColor(0, 1, 0);  // Green
	triangle b(vertex(0.5, 0.0, 0.5), vertex(0.5, 0.0, -0.5), vertex(0.0, 1.5, 0.0));   // p2 p3 p5
	b.setColor(1, 0, 0);  // Red
	triangle c(vertex(0.5, 0.0, -0.5), vertex(-0.5, 0.0, -0.5), vertex(0.0, 1.5, 0.0));  // p3 p4 p5
	c.setColor(0, 0, 1); // Blue
	triangle d(vertex(-0.5, 0.0, -0.5), vertex(-.5, 0.0, 0.5), vertex(0.0, 1.5, 0.0));   //p4 p1 p5
	d.setColor(1, 1, 0);  // yellow
	triangle e(vertex(-.5, 0.0, 0.5), vertex(0.5, 0.0, -0.5), vertex(0.5, 0.0, 0.5));    // p1 p3 p2
	e.setColor(0, 1, 1);  // light blue
	triangle f(vertex(-.5, 0.0, 0.5), vertex(-0.5, 0.0, -0.5), vertex(0.5, 0.0, -0.5));   // p1 p4 p3
	f.setColor(0, 1, 1);   // light blue

	triangulos.addTriangle(a);
	triangulos.addTriangle(b);
	triangulos.addTriangle(c);
	triangulos.addTriangle(d);
	triangulos.addTriangle(e);
	triangulos.addTriangle(f);
}


static void show_usage()
{
	std::cerr << "Usage: primitiva <options> <destiny file>" << endl << "cilindro (lados altura raio)" << endl <<
		"cone (lados altura raio" << endl << "cubo (aresta)" << endl << "plano (comprimento largura)" << endl;
}

int main(int argc, char **argv) {
	primitive p;

	vector <string> sources;
	string destination;
	if (argc == 1) show_usage();
	else
	{
		for (int i = 1; i < argc; ++i) {
			string arg = argv[i];
			if (arg == "cilindro"){
				float n_lados = stof(argv[i + 1]);
				float altura = stof(argv[i + 2]);
				float raio = stof(argv[i + 3]);
				string file = argv[i + 4];
				p = drawCilinder(n_lados, altura, raio);
				p.saveFile(file);
			}
			else if (arg == "cone") {
				float n_lados = stof(argv[i + 1]);
				float altura = stof(argv[i + 2]);
				float raio = stof(argv[i + 3]);
				string file = argv[i + 4];
				p = drawCone(n_lados, altura, raio);
				p.saveFile(file);
			}
			else if (arg == "cubo") {
				float aresta = stof(argv[i + 1]);
				string file = argv[i + 2];
				p = drawCube(aresta);
				p.saveFile(file);
			}
			else if (arg == "plano") {
				float largura = stof(argv[i + 1]);
				float comprimento = stof(argv[i + 2]);
				string file = argv[i + 3];
				p = drawPlane(largura, comprimento);
				p.saveFile(file);
			}
		}
	}
}	


