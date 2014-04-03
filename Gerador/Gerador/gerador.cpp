#include <math.h>
#include <fstream>
#include <iostream>
#include "classes.h"

#define PI 3.14159265358979323846

primitive drawPlane(float largura, float comprimento)
{
	primitive p;
	float l = largura / 2;
	float c = comprimento / 2;

	vertex p1(-c, 0, l);
	vertex p2(c, 0, l);
	vertex p3(c, 0, -l);
	vertex p4(-c, 0, -l);
	triangle t1(p1, p2, p3);	t1.setColor(1, 1, 0);
	triangle t2(p3, p4, p1);	t2.setColor(1, 1, 0);

	p.addTriangle(t1);
	p.addTriangle(t2);

	return p;
}
primitive drawRectangule( float comprimento, float largura, float altura)
{
	primitive p;
	float l = largura / 2, c = comprimento / 2, a = altura / 2;

	vertex p1(-c, a, l);
	vertex p2(-c, -a, l);
	vertex p3(c, -a, l);
	vertex p4(c, a, l);
	vertex p5(c, a, -l);
	vertex p6(c, -a, -l);
	vertex p7(-c, -a, -l);
	vertex p8(-c, a, -l);

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
primitive drawCube(float size) {
	return drawRectangule(size, size, size);
}
primitive drawCilinder(float n_lados, float altura, float raio)
{
	primitive p;
	float delta = 2 * PI / n_lados;
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
	float delta = 2 * PI / n_lados;
	for (int count = 0; count < n_lados; count++) {
		vertex a(0, -altura / 2, 0);  //O
		vertex b(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  //Q
		vertex c(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); //P

		vertex d(0, altura / 2, 0);  //O
		vertex e(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); //P
		vertex f(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1)));  //Q

		triangle t1(a, b, c);	t1.setColor(0, 0, 1);
		triangle t2(d, e, f);	t2.setColor(0, 0, 1);

		p.addTriangle(t1);
		p.addTriangle(t2);
	}
	return p;
}
primitive drawSphere(float raio, int fatias, int camadas)
{
	primitive p;
	float alfa, beta;
	float incA = 2 * PI / camadas;
	float incB = PI / fatias;
	float x0, xa, xb, x, y0, y, z0, za, zb, z;

	for (alfa = 0.0; alfa < 2 * PI; alfa += incA) {
		for (beta = -PI / 2; beta < PI / 2; beta += incB) {

			x0 = raio*cos(beta)*sin(alfa);
			xa = raio*cos(beta)*sin(alfa + incA);
			xb = raio*cos(beta + incB)*sin(alfa);
			x = raio*cos(beta + incB)*sin(alfa + incA);

			y0 = raio*sin(beta);
			y = raio*sin(beta + incB);

			z0 = raio*cos(beta)*cos(alfa);
			za = raio*cos(beta)*cos(alfa + incA);
			zb = raio*cos(beta + incB)*cos(alfa);
			z = raio*cos(beta + incB)*cos(alfa + incA);

			vertex b(xb, y, zb);
			vertex c(x0, y0, z0);
			vertex d(xa, y0, za);

			vertex f(xa, y0, za);
			vertex g(x, y, z);
			vertex h(xb, y, zb);

			triangle t1(b, c, d); t1.setColor(0.5, 0, 0);
			triangle t2(f, g, h); t2.setColor(1, 0, 0);

			p.addTriangle(t1);
			p.addTriangle(t2);
		}
	}
	return p;
}





static void show_usage()
{
	std::cerr << endl << "\nUsage: primitiva <options> <filename.txt>\n" << endl << "cilindro <lados altura raio>" << endl <<
		"cone <lados altura raio>" << endl << "cubo <aresta>" << endl << "paralelipipedo <comprimento largura altura>" << endl <<
		"plano <comprimento largura>" << endl << "esfera <raio fatias camadas>" << endl;  
}

int main(int argc, char **argv) {
	primitive p;

	if (argc == 1) show_usage();
	else {
		for (int i = 1; i < argc; ++i) {
			string arg = argv[i];
			if (arg == "cilindro"){
				string err = "\n-- Erro -- Argumentos invalidos\n Usage: cilindro <lados altura raio> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}
				else{
					try{
						float n_lados = stof(argv[i + 1]);
						float altura = stof(argv[i + 2]);
						float raio = stof(argv[i + 3]);
						string file = argv[i + 4];
						p = drawCilinder(n_lados, altura, raio);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
				
			}
			else if (arg == "cone") {
				string err = "\n--Erro -- Argumentos invalidos\n Usage: cone <lados altura raio> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}
				else{
					try{
						float n_lados = stof(argv[i + 1]);
						float altura = stof(argv[i + 2]);
						float raio = stof(argv[i + 3]);
						string file = argv[i + 4];
						p = drawCone(n_lados, altura, raio);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "cubo") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: cubo <aresta> <filename>\n";
				if (argc != 4){
					std::cerr << err;
				}
				else{
					try{
						float aresta = stof(argv[i + 1]);
						string file = argv[i + 2];
						p = drawCube(aresta);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "paralelipipedo") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: paralelipipedo <comprimento largura altura> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}
				else{
					try{
						float largura = stof(argv[i + 1]);
						float comprimento = stof(argv[i + 2]);
						float altura = stof(argv[i + 3]);
						string file = argv[i + 4];
						p = drawRectangule(largura, comprimento, altura);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "plano") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: plano <comprimento largura> <filename>\n";
				if (argc != 5){
					std::cerr << err;
				}
				else{
					try{
						float largura = stof(argv[i + 1]);
						float comprimento = stof(argv[i + 2]);
						string file = argv[i + 3];
						p = drawPlane(largura, comprimento);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "esfera") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: esfera <raio fatias camadas> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}else{
					try{
						float raio = stof(argv[i + 1]);
						float camadas = stof(argv[i + 2]);
						float fatias = stof(argv[i + 3]);
						string file = argv[i + 4];
						p = drawSphere(raio, camadas, fatias);
						p.saveFile(file);

					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
		}
	}
}	