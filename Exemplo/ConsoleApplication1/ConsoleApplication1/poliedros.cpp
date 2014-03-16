#include "poliedros.h"


/* .:plano
 * (desenhar quadrado no plano xOz centrado na origem)
 * parametros:
 * float comprimento: comprimento da aresta
 */
void plano(float comprimento) {
	float lim = comprimento/2;
	glBegin(GL_TRIANGLES);

	glVertex3f(-lim,0.0f,lim);
	glVertex3f(lim,0.0f,lim);
	glVertex3f(-lim,0.0f,-lim);

	glVertex3f(-lim,0.0f,-lim);
	glVertex3f(lim,0.0f,lim);
	glVertex3f(lim,0.0f,-lim);

	glEnd();
}


/* .:cubo
 * (desenhar cubo centrado na origem)
 * parametros:
 * float aresta: comprimento da aresta do cubo
 */
void cubo(float aresta) {
	float lim = aresta/2;
	// face 1
	glColor3f(0,0,1); // temporario
	glPushMatrix();
	glTranslatef(0,-lim,0);
	plano(aresta);
	glPopMatrix();
	// face 2
	glColor3f(0,1,0); // temporario
	glPushMatrix();
	glTranslatef(-lim,0,0);
	glRotatef(90,0,0,1);
	plano(aresta);
	glPopMatrix();
	// face 3
	glColor3f(0,1,1); // temporario
	glPushMatrix();
	glTranslatef(0,0,lim);
	glRotatef(90,1,0,0);
	plano(aresta);
	glPopMatrix();
	// face 4
	glColor3f(1,0,0); // temporario
	glPushMatrix();
	glTranslatef(lim,0,0);
	glRotatef(-90,0,0,1);
	plano(aresta);
	glPopMatrix();
	// face 5
	glColor3f(1,0,1); // temporario
	glPushMatrix();
	glTranslatef(0,0,-lim);
	glRotatef(-90,1,0,0);
	plano(aresta);
	glPopMatrix();
	// face 6
	glColor3f(1,1,1); // temporario
	glPushMatrix();
	glTranslatef(0,lim,0);
	plano(aresta);
	glPopMatrix();
	
}


/* .:cilindro
 * (desenhar cilindro centrado na origem)
 * parametros:
 * float raio: raio do cilindro
 * float altura: altura do cilindro
 * int ppc: numero de subdivisoes (prismas triangulares a desenhar para criar o cilindro (qualidade do objecto))
*/
void cilindro(float raio, float altura, int ppc) {

	int i;
	double alfa = 0.0;
	float lim=altura/2;
	
	// numero de prismas triangulares para construir o cilindro
	for(i=0;i<ppc;i++) {
		glPushMatrix();
		glBegin(GL_TRIANGLES);
		//base prisma baixo
		glColor3f(1,0,0); // temporario
		glVertex3f(0.0f,-lim,0.0f);
		glVertex3f(raio*sin(alfa),-lim,raio*cos(alfa));
		alfa += (2*PI) / ((float) ppc);
		glVertex3f(raio*sin(alfa),-lim,raio*cos(alfa));
		// lado prisma cima
		glColor3f(0,1,0); // temporario
		glVertex3f(raio*sin(alfa),-lim,raio*cos(alfa));
		glVertex3f(raio*sin(alfa),lim,raio*cos(alfa));
		alfa -= (2*PI) / ((float) ppc);
		glVertex3f(raio*sin(alfa),-lim,raio*cos(alfa));
		// lado prisma baixo
		glColor3f(0,1,0); // temporario
		glVertex3f(raio*sin(alfa),-lim,raio*cos(alfa));
		alfa += (2*PI) / ((float) ppc);
		glVertex3f(raio*sin(alfa),lim,raio*cos(alfa));
		alfa -= (2*PI) / ((float) ppc);
		glVertex3f(raio*sin(alfa),lim,raio*cos(alfa));
		// base prisma cima
		glColor3f(0,0,1); // temporario
		glVertex3f(raio*sin(alfa),lim,raio*cos(alfa));
		alfa += (2*PI) / ((float) ppc);
		glVertex3f(raio*sin(alfa),lim,raio*cos(alfa));
		alfa -= (2*PI) / ((float) ppc);
		glVertex3f(0.0f,lim,0.0f);
		
		glEnd();
		glPopMatrix();
		alfa += (2*PI) / ((float) ppc);
	}
	alfa = 0.0;
}


/* .:esfera
 * (desenhar esfera centrada na origem)
 * parametros:
 * float raio: raio da esfera
 * int complex: numero de subdivisoes (seccoes a desenhar ao longo de beta [-PI/2,PI/2] e de alfa [0,2*PI] (qualidade do objecto))
 */
void esfera(float raio, int complex) {
	float alfa, beta;
	float incA = 2*PI/complex;
	float incB = PI/30;
	float x0,xa,xb,x,y0,y,z0,za,zb,z;

	glBegin(GL_TRIANGLES);
	for(alfa=0.0;alfa<2*PI;alfa+=incA) {
		for(beta=-PI/2;beta<PI/2;beta+=incB) {
			
			x0 = raio*cos(beta)*sin(alfa);
			xa = raio*cos(beta)*sin(alfa+incA);
			xb = raio*cos(beta+incB)*sin(alfa);
			x = raio*cos(beta+incB)*sin(alfa+incA);
			
			y0 = raio*sin(beta);
			y = raio*sin(beta+incB);

			z0 = raio*cos(beta)*cos(alfa);
			za = raio*cos(beta)*cos(alfa+incA);
			zb = raio*cos(beta+incB)*cos(alfa);
			z = raio*cos(beta+incB)*cos(alfa+incA);

			glColor3f(1,0,0); // temporario
			glVertex3f(xb, y, zb);
			glVertex3f(x0, y0, z0);
			glVertex3f(xa, y0, za);

			glColor3f(0,1,1); // temporario
			glVertex3f(xa, y0, za);
			glVertex3f(x, y, z);
			glVertex3f(xb, y, zb);

		}
	}
	glEnd();
}