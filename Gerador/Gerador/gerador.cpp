#include <math.h>
#include <fstream>
#include <iostream>
#include "classes.h"
#include "patchBezier.h"

#define PI 3.14159


primitiveVBO drawPlane(float comprimento, float largura, int camadas)
{
	primitiveVBO p;
	float z0 = largura / 2;
	float x0 = -comprimento / 2;
	float y = 0.0f;
	float incH = comprimento / camadas;
	float incV = largura / camadas;

	float incremento = 1.0 / camadas;

	int nindices = 4;   //numero de indices adicionados por ciclo
	int inc = 0;
	int h = 0;

	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0+(i*incH), y, z0 - (j*incV)); p0.setNormal(0.0f, 1.0f, 0.0f);
			p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0+((i+1)*incH), y, z0-(j*incV)); p1.setNormal(0.0f, 1.0f, 0.0f);
			p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0+((i+1)*incH), y, z0-((j+1)*incV)); p2.setNormal(0.0f, 1.0f, 0.0f);
			p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0+(i*incH), y, z0 - ((j+1)*incV)); p3.setNormal(0.0f, 1.0f, 0.0f);
			p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			//visto de cima
			p.addTriangulo(inc + 0,inc + 1,inc + 2);
			p.addTriangulo(inc + 0,inc + 2,inc + 3);

			//visto de cima
			p.addTriangulo(inc + 0, inc + 2, inc + 1);
			p.addTriangulo(inc + 0, inc + 3, inc + 2);

			h++;
		}
	}
	return p;
}

primitiveVBO drawRectangule( float comprimento, float largura, float altura, int camadas)  //Paralelipipedo
{
	primitiveVBO p;

	float z0 = largura / 2.0f;
	float x0 = -comprimento / 2.0f;
	float y0 = -altura / 2.0f;
	float incC = comprimento / camadas;
	float incL = largura / camadas;
	float incA = altura / camadas;


	float incremento = 1.0 / camadas; // textura

	int nindices = 4;   //numero de indices adicionados por ciclo
	int inc = 0;
	int h = 0;

	//face frontal
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0 + (i*incC), y0 + (j*incA), z0) ; p0.setNormal(0.0f, 0.0f, 1.0f);
			//p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0 + ((i + 1)*incC), y0 + (j*incA), z0); p1.setNormal(0.0f, 0.0f, 1.0f);
			//p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0 + ((i + 1)*incC), y0 + ((j + 1)*incA),z0); p2.setNormal(0.0f, 0.0f, 1.0f);
			//p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0 + (i*incC), y0 + ((j + 1)*incA),z0); p3.setNormal(0.0f, 0.0f, 1.0f);
			//p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 1, inc + 2);
			p.addTriangulo(inc + 0, inc + 2, inc + 3);

			h++;
		}
	}

	//face traseira
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0 + (i*incC), y0 + (j*incA), -z0); p0.setNormal(0.0f, 0.0f, -1.0f);
			//p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0 + ((i + 1)*incC), y0 + (j*incA), -z0); p1.setNormal(0.0f, 0.0f, -1.0f);
			//p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0 + ((i + 1)*incC), y0 + ((j + 1)*incA), -z0); p2.setNormal(0.0f, 0.0f, -1.0f);
			//p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0 + (i*incC), y0 + ((j + 1)*incA), -z0); p3.setNormal(0.0f, 0.0f, -1.0f);
			//p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 2, inc + 1);
			p.addTriangulo(inc + 0, inc + 3, inc + 2);

			h++;
		}
	}

	//face lateral esquerda
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0, y0 + (j*incA), z0 - (i*incL)); p0.setNormal(-1.0f, 0.0f, 0.0f);
			//p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0, y0 + (j*incA), z0 - ((i + 1)*incL)); p1.setNormal(-1.0f, 0.0f, 0.0f);
			//p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0, y0 + ((j + 1)*incA), z0 - ((i + 1)*incL)); p2.setNormal(-1.0f, 0.0f, 0.0f);
			//p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0, y0 + ((j + 1)*incA), z0 - (i*incL)); p3.setNormal(-1.0f, 0.0f, 0.0f);
			//p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 2, inc + 1);
			p.addTriangulo(inc + 0, inc + 3, inc + 2);

			h++;
		}
	}

	//face lateral direita
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(-x0, y0 + (j*incA), z0 - (i*incL)); p0.setNormal(1.0f, 0.0f, 0.0f);
			//p0.setText2D(i*incremento, j*incremento);
			vertex p1(-x0, y0 + (j*incA), z0 - ((i + 1)*incL)); p1.setNormal(1.0f, 0.0f, 0.0f);
			//p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(-x0, y0 + ((j + 1)*incA), z0 - ((i + 1)*incL)); p2.setNormal(1.0f, 0.0f, 0.0f);
			//p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(-x0, y0 + ((j + 1)*incA), z0 - (i*incL)); p3.setNormal(1.0f, 0.0f, 0.0f);
			//p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 1, inc + 2);
			p.addTriangulo(inc + 0, inc + 2, inc + 3);

			h++;
		}
	}

	//Topo
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0 + (i*incC), -y0, z0 - (j*incL)); p0.setNormal(0.0f, 1.0f, 0.0f);
			//	p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0 + ((i + 1)*incC), -y0, z0 - (j*incL)); p1.setNormal(0.0f, 1.0f, 0.0f);
			//	p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0 + ((i + 1)*incC), -y0, z0 - ((j + 1)*incL)); p2.setNormal(0.0f, 1.0f, 0.0f);
			//	p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0 + (i*incC), -y0, z0 - ((j + 1)*incL)); p3.setNormal(0.0f, 1.0f, 0.0f);
			//	p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 1, inc + 2);
			p.addTriangulo(inc + 0, inc + 2, inc + 3);

			h++;
		}
	}

	//Baixo
	for (int i = 0; i < camadas; i++) {
		for (int j = 0; j < camadas; j++) {

			inc = nindices*h;

			vertex p0(x0 + (i*incC), y0, z0 - (j*incL)); p0.setNormal(0.0f, 1.0f, 0.0f);
			//	p0.setText2D(i*incremento, j*incremento);
			vertex p1(x0 + ((i + 1)*incC), y0, z0 - (j*incL)); p1.setNormal(0.0f, 1.0f, 0.0f);
			//	p1.setText2D((i + 1)*incremento, j*incremento);
			vertex p2(x0 + ((i + 1)*incC), y0, z0 - ((j + 1)*incL)); p2.setNormal(0.0f, 1.0f, 0.0f);
			//	p2.setText2D((i + 1)*incremento, (j + 1)*incremento);
			vertex p3(x0 + (i*incC), y0, z0 - ((j + 1)*incL)); p3.setNormal(0.0f, 1.0f, 0.0f);
			//	p3.setText2D(i*incremento, (j + 1)*incremento);

			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2); p.addPonto(p3);

			p.addTriangulo(inc + 0, inc + 2, inc + 1);
			p.addTriangulo(inc + 0, inc + 3, inc + 2);

			h++;
		}
	}

	return p;
}

primitiveVBO drawCube(float size, int camadas) {
	return drawRectangule(size, size, size, camadas);
}
primitiveVBO drawCilinder(float n_lados, float altura, float raio)
{
	primitiveVBO p;

	float incTampaCimaX = 0.4375;
	float incTampaCimaY = 0.1875;
	float incTampaBaixoX = 0.8125;
	float incTampaBaixoY = 0.1875;

	float incCorpoCimaY = 1.0f;
	float incCorpoBaixo = 0.375f;

	float raioTampaCilindro = 0.623 - 0.4375;
	float incremento = 1.0f / n_lados;

	float delta = 2 * PI / n_lados;
	int j = 10; //numero de pontos por ciclos
	int inc = 0;

	for (int i = 0; i < n_lados; i++) {

		inc=j*i;
		
		//topo   OT, PT, QT
		vertex p0(0, altura / 2, 0); p0.setNormal(0.0f, 1.0f, 0.0f);//OT
		p0.setText2D(incTampaCimaX, incTampaCimaY);

		vertex p1(raio*sin(delta*i), altura / 2, raio*cos(delta*i)); p1.setNormal(0.0f, 1.0f, 0.0f);//PT
		p1.setText2D(incTampaCimaX - raioTampaCilindro*sin(i*delta), incTampaCimaY - raioTampaCilindro*cos(i*delta));

		vertex p2(raio*sin(delta*(i + 1)), altura / 2, raio*cos(delta*(i + 1))); p2.setNormal(0.0f, 1.0f, 0.0f);  //QT
		p2.setText2D(incTampaCimaX - raioTampaCilindro*sin((i + 1)*delta), incTampaCimaY - raioTampaCilindro*cos((i + 1)*delta));

		//corpo P, P', Q | Q, P', Q'
		
		vertex p3(raio*sin(delta*(i + 1)), altura / 2, raio*cos(delta*(i + 1))); p3.setNormal(sin(delta*(i + 1)), 0.0f, cos(delta*(i + 1)));  //Q
		p3.setText2D((i + 1)*incremento, incCorpoCimaY);

		vertex p4(raio*sin(delta*i), altura / 2, raio*cos(delta*i)); p4.setNormal(sin(delta*i), 0.0f, cos(delta*i));//P
		p4.setText2D(i*incremento, incCorpoCimaY);


		vertex p5(raio*sin(delta*i), -altura / 2, raio*cos(delta*i)); p5.setNormal(sin(delta*i), 0.0f, cos(delta*i)); //P'
		p5.setText2D(i*incremento, incCorpoBaixo);


		vertex p6(raio*sin(delta*(i + 1)), -altura / 2, raio*cos(delta*(i + 1))); p6.setNormal(sin(delta*(i + 1)), 0, cos(delta*(i + 1)));  // Q'
		p6.setText2D((i + 1)*incremento, incCorpoBaixo);



		//base O'B, P'B, Q'B
		vertex p7(0, -altura / 2, 0); p7.setNormal(0.0f, -1.0f, 0.0f);  // O'B
		p7.setText2D(incTampaBaixoX, incTampaBaixoY);

		vertex p8(raio*sin(delta*i), -altura / 2, raio*cos(delta*i)); p8.setNormal(0.0f, -1.0f, 0.0f); //P'B
		p8.setText2D(incTampaBaixoX - raioTampaCilindro*sin(i*delta), incTampaBaixoY - raioTampaCilindro*cos(i*delta));

		vertex p9(raio*sin(delta*(i + 1)), -altura / 2, raio*cos(delta*(i + 1))); p9.setNormal(0.0f, -1.0f, 0.0f);  // Q'B
		p9.setText2D(incTampaBaixoX - raioTampaCilindro*sin((i + 1)*delta), incTampaBaixoY - raioTampaCilindro*cos((i + 1)*delta));
		

		p.addPonto(p0); p.addPonto(p1); p.addPonto(p2);	p.addPonto(p3); p.addPonto(p4);
		p.addPonto(p5); p.addPonto(p6); p.addPonto(p7); p.addPonto(p8); p.addPonto(p9);

		p.addTriangulo(inc + 0, inc + 1, inc + 2);
		p.addTriangulo(inc + 3, inc + 4, inc + 5);
		p.addTriangulo(inc + 6, inc + 3, inc + 5);
		p.addTriangulo(inc + 7, inc + 9, inc + 8);

	}
	return p;

}
primitiveVBO drawDisc(float raioInt, float raioExt, float tesselacao)
{
	primitiveVBO p;


	float alfa = (2 * PI ) / tesselacao;
	float incr = 1.0f / tesselacao;
	float inc = 0;
	int j = 8;

	for (int i = 0; i < tesselacao; i++) {

		inc = j*i;

		float y0 = 0;
		
		float x1 = raioInt*cos(i*alfa);
		float z1 = raioInt*sin(i*alfa);
		float x2 = raioExt*cos(i*alfa);
		float z2 = raioExt*sin(i*alfa);
		float x3 = raioInt*cos((i + 1)*alfa);
		float z3 = raioInt*sin((i + 1)*alfa);
		float x4 = raioExt*cos((i + 1)*alfa);
		float z4 = raioExt*sin((i + 1)*alfa);

		///FACE CIMA
		vertex p0(x1, y0, z1, 0.0, -1.0, 0.0);
		p0.setText2D(i*incr,0.0f);

		vertex p1(x2, y0, z2, 0.0, -1.0, 0.0);
		p1.setText2D(i*incr, 1.0f);

		vertex p2(x3, y0, z3, 0.0, -1.0, 0.0);
		p2.setText2D((i+1)*incr, 0.0f);

		vertex p3(x4, y0, z4, 0.0, -1.0, 0.0);
		p3.setText2D((i+1)*incr, 1.0f);

		///FACE BAIXO
		vertex p4(x1, y0, z1, 0.0, 1.0, 0.0);
		p4.setText2D(i*incr, 0.0f);

		vertex p5(x3, y0, z3, 0.0, 1.0, 0.0);
		p5.setText2D((i + 1)*incr, 0.0f);

		vertex p6(x2, y0, z2, 0.0, 1.0, 0.0);
		p6.setText2D(i*incr, 1.0f);

		vertex p7(x4, y0, z4, 0.0, 1.0, 0.0);
		p7.setText2D((i + 1)*incr, 1.0f);

		p.addPonto(p0); p.addPonto(p1); p.addPonto(p2);	p.addPonto(p3);
		p.addPonto(p4); p.addPonto(p5); p.addPonto(p6);	p.addPonto(p7);

		p.addTriangulo(inc + 0, inc + 1, inc + 2);
		p.addTriangulo(inc + 2, inc + 1, inc + 3);
		p.addTriangulo(inc + 4, inc + 5, inc + 6);
		p.addTriangulo(inc + 6, inc + 5, inc + 7);

	}
	return p;
}
primitiveVBO drawCone(float n_lados, float altura, float raio)
{

	float incTampaCimaX = 0.4375;
	float incTampaCimaY = 0.1875;
	float incTampaBaixoX = 0.8125;
	float incTampaBaixoY = 0.1875;

	float incCorpoCimaY = 1.0f;
	float incCorpoBaixo = 0.375f;

	float raioTampaCilindro = 0.623 - 0.4375;
	float incremento = 1.0f / n_lados;

	primitiveVBO p;
	int nindices = 6;   //numero de indices adicionados por ciclo
	int inc = 0;
	int h = 0;

	float delta = 2 * PI / n_lados;
	for (int count = 0; count < n_lados; count++) {

		inc = nindices*h;

		// O Q P  BASE
		vertex p0(0, -altura / 2, 0); p0.setNormal(0.0f, 1.0f, 0.0f); //O
		p0.setText2D(incTampaBaixoX, incTampaBaixoY);

		vertex p1(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1))); p1.setNormal(sin(delta*(count + 1)), 0.0f, cos(delta*(count + 1))); //Q
		p1.setText2D(incTampaBaixoX - raioTampaCilindro*sin((count + 1)*delta), incTampaBaixoY - raioTampaCilindro*cos((count + 1)*delta));

		vertex p2(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); p2.setNormal(sin(delta*(count)), 0.0f, cos(delta*(count)));  //P
		p2.setText2D(incTampaBaixoX - raioTampaCilindro*sin(count*delta), incTampaBaixoY - raioTampaCilindro*cos(count*delta));

		// O' P Q CORPO
		vertex p3(0, altura / 2, 0); p3.setNormal(0.0f, 1.0f, 0.0f);  //O'
		p3.setText2D(0.5f, incCorpoCimaY);

		vertex p4(raio*sin((delta)*(count)), -altura / 2, raio*cos((delta)*(count))); p4.setNormal(sin(delta*(count)), 0.0f, cos(delta*(count)));  //P
		p4.setText2D(count*incremento, incCorpoBaixo);

		vertex p5(raio*sin((delta)*(count + 1)), -altura / 2, raio*cos((delta)*(count + 1))); p5.setNormal(sin(delta*(count + 1)), 0.0f, cos(delta*(count + 1))); //Q
		p5.setText2D((count+1)*incremento, incCorpoBaixo);

		p.addPonto(p0); p.addPonto(p1); p.addPonto(p2);	
		p.addPonto(p3); p.addPonto(p4); p.addPonto(p5);

		p.addTriangulo(inc+0, inc+1, inc+2);
		p.addTriangulo(inc+3, inc+4, inc+5);

		h++;
	}
	return p;
}
primitiveVBO drawSphere(float raio, int fatias, int camadas)
{
	primitiveVBO p;

	int nindices = 4;   //numero de indices adicionados por ciclo
	int inc = 0; 
	int h = 0;

	float incA = 2 * PI / fatias;
	float incB = PI / camadas;
	float x1, x2, x3, x4, y0, y1, z1, z2, z3, z4;

	float alfa, beta;

	for (int i = 0; i < fatias; i++) {
		for (int j = 0; j < camadas; j++) {
			alfa = incA*i;
			beta = (-PI / 2.0f) + incB*j;

			inc = nindices*h;

			x1 = raio*cos(beta) * sin(alfa);
			x2 = raio*cos(beta + incB) * sin(alfa);
			x3 = raio*cos(beta) * sin(alfa + incA);
			x4 = raio*cos(beta + incB) * sin(alfa + incA);

			y0 = raio*sin(beta);
			y1 = raio*sin(beta + incB);

			z1 = raio * cos(beta) * cos(alfa);
			z2 = raio * cos(beta + incB) * cos(alfa);
			z3 = raio * cos(beta) * cos(alfa + incA);
			z4 = raio * cos(beta + incB) * cos(alfa + incA);

			float tx, ty;

			vertex p0(x1, y0, z1); p0.setNormal(x1 / raio, y0 / raio, z1 / raio);
			tx = atan2(p0.getNx(), p0.getNz()) / (2. * PI) + 0.5;
			ty = asin(p0.getNy()) / PI + .5;
			p0.setText2D(tx, ty);

			vertex p1(x3, y0, z3); p1.setNormal(x3 / raio, y0 / raio, z3 / raio);
			tx = atan2(p1.getNx(), p1.getNz()) / (2. * PI) + 0.5;
			ty = asin(p1.getNy()) / PI + .5;

			if (tx < 0.75 && p0.getTx() > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && p0.getTx() < 0.75)
				tx -= 1.0;
			p1.setText2D(tx, ty);

			vertex p2(x2, y1, z2); p2.setNormal(x2 / raio, y1 / raio, z2 / raio);
			tx = atan2(p2.getNx(), p2.getNz()) / (2. * PI) + 0.5;
			ty = asin(p2.getNy()) / PI + .5;

			if (tx < 0.75 && p1.getTx() > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && p1.getTx() < 0.75)
				tx -= 1.0;
			p2.setText2D(tx, ty);

			vertex p3(x4, y1, z4); p3.setNormal(x4 / raio, y1 / raio, z4 / raio);
			tx = atan2(p3.getNx(), p3.getNz()) / (2. * PI) + 0.5;
			ty = asin(p3.getNy()) / PI + .5;

			if (tx < 0.75 && p2.getTx() > 0.75)
				tx += 1.0;
			else if (tx > 0.75 && p2.getTx() < 0.75)
				tx -= 1.0;
			p3.setText2D(tx, ty);


			p.addPonto(p0); p.addPonto(p1); p.addPonto(p2);	p.addPonto(p3); 

			p.addTriangulo(inc + 0, inc + 1, inc + 2);
			p.addTriangulo(inc + 1, inc + 3, inc + 2);

			h++;
		}
	}
	return p;
}
primitiveVBO drawBezierSurface(string file, int tesselacao) {
	primitiveVBO pri;
	patchFile p = (file);

	pri = p.getprimitivaVBO(tesselacao);

	return pri;
}


static void show_usage()
{
	std::cerr << endl << "\nUsage: primitiva <options> <filename.txt>\n" << endl << "cilindro <lados altura raio>" << endl <<
		"cone <lados altura raio>" << endl << "cubo <aresta camadas>" << endl << "paralelipipedo <comprimento largura altura camadas>" << endl <<
		"plano <comprimento largura camadas>" << endl << "esfera <raio fatias camadas>" << endl << "disco <raio_interior raio_exterior tesselacao>" << endl << "supBezier <ficheiro .patch tesselacao>" << endl;
}


int main(int argc, char **argv) {
	primitiveVBO p;

	
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
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: cubo <aresta camadas> <filename>\n";
				if (argc != 5){
					std::cerr << err;
				}
				else{
					try{
						float aresta = stof(argv[i + 1]);
						int camadas = atoi(argv[i + 2]);
						string file = argv[i + 3];
						p = drawCube(aresta, camadas);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "paralelipipedo") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: paralelipipedo <comprimento largura altura camadas> <filename>\n";
				if (argc != 7){
					std::cerr << err;
				}
				else{
					try{
						float largura = stof(argv[i + 1]);
						float comprimento = stof(argv[i + 2]);
						float altura = stof(argv[i + 3]);
						int camadas = atoi(argv[i + 4]);
						string file = argv[i + 5];
						p = drawRectangule(largura, comprimento, altura, camadas);
						p.saveFile(file);
					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "plano") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: plano <comprimento largura camadas> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}
				else{
					try{
						float largura = stof(argv[i + 1]);
						float comprimento = stof(argv[i + 2]);
						int camadas= atoi(argv[i + 3]);
						string file = argv[i + 4];
						p = drawPlane(comprimento, largura, camadas);
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
			else if (arg == "disco") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: disco <raio_interior raio_exterior tesselacao> <filename>\n";
				if (argc != 6){
					std::cerr << err;
				}
				else{
					try{
						float raioInt = stof(argv[i + 1]);
						float raioExt = stof(argv[i + 2]);
						int tesselacao = atoi(argv[i + 3]);
						string file = argv[i + 4];
						p = drawDisc(raioInt, raioExt, tesselacao);
						p.saveFile(file);

					}
					catch (invalid_argument)
					{
						std::cerr << err;
					}
				}
			}
			else if (arg == "supBezier") {
				string err = "\n-- Erro -- Argumentos invalidos\nUsage: supBezier <ficheiro.patch tesselacao> <filename>\n";
				if (argc != 5){
					std::cerr << err;
				}
				else{
					try{
						string file = argv[i + 1];
						int tesselacao = stoi(argv[i + 2]);
						string savefile = argv[i + 3];
						p = drawBezierSurface(file, tesselacao);
						p.saveFile(savefile);
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