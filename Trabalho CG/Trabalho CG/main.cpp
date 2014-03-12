#include <math.h>
//#include <vector>
#include "classes.h"
#include <GL/glut.h>

float lookatx=5.0, lookaty=-1.0, lookatz=5.0;
float rotateAng=0.0;
float rotateTop=0.0;
float alt=1.0;
float r=0;
float g=1;
float b=1;

triangles triangulos;


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void iniciaClass()
{


	triangle a(	vertex(-.5, 0.0, 0.5), vertex(0.5, 0.0, 0.5), vertex(0.0, 1.5, 0.0));    // p1 p2 p5
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

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(lookatx,lookaty,lookatz, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


	// pôr instruções de desenho aqui

	glRotatef(rotateAng,0,1,0);
	glRotatef(rotateTop,1,0,0);


	// Desenha a estrutura guardada em "triangulos"

	vector<triangle> aux = triangulos.getTriangulos();
	triangle* a;
	for (vector<triangle>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		a = it._Ptr;
		glBegin(GL_TRIANGLES);
		glColor3f(a->getColorR(), a->getColorG(), a->getColorB());
		glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
		glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
		glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
		glEnd();
	}
	

	// End of frame
	glutSwapBuffers();
}



// escrever função de processamento do teclado

	void sK(int codigo, int x, int y)
	{
		switch(codigo)
		{
		case GLUT_KEY_HOME:
			lookatx=lookatx+0.3;
			break;
		case GLUT_KEY_END:
			lookatx=lookatx-0.3;
			break;
		case GLUT_KEY_RIGHT:
			rotateAng=rotateAng+10;
			break;
		case GLUT_KEY_LEFT:
			rotateAng=rotateAng-10;
			break;
		case GLUT_KEY_UP:
			rotateTop=rotateTop+10;
			break;
		case GLUT_KEY_DOWN:
			rotateTop=rotateTop-10;
			break;
		}
		glutPostRedisplay();
	}

	void kP(unsigned char codigo, int x, int y)
	{
		switch(codigo)
		{
		case 'e':
			lookatx=lookatx+0.3;
			break;
		case 'q':
			lookatx=lookatx-0.3;
			break;
		case 'w':
			lookaty=lookaty+0.3;
			break;
		case 's':
			lookaty=lookaty-0.3;
			break;
		case 'r':
			alt=alt+0.3;
			break;
		case 'f':
			alt=alt-0.3;
			break;
		}
		glutPostRedisplay();
	}



// escrever função de processamento do menu
	void menu(int opcao) {
		switch (opcao)
		{
		case 1:
			r=1; g=0; b=0;
			break;
		case 2:
			r=0; g=0; b=1;
			break;
		case 3:
			r=0; g=1; b=0;
			break;
		case 4:
			r=0; g=1; b=1;
			break;
		}
		glutPostRedisplay();
	}



int main(int argc, char **argv) {

// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
	

	// cria piramide
	iniciaClass();


// registo de funções 
	glutDisplayFunc(renderScene);
//	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(sK);
	glutKeyboardFunc(kP);


// pôr aqui a criação do menu
	int i = glutCreateMenu(menu);
	glutAddMenuEntry("Vermelho",1);
	glutAddMenuEntry("Azul",2);
	glutAddMenuEntry("Verde",3);
	glutAddMenuEntry("Azul Claro",4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// entrar no ciclo do GLUT 
	glutMainLoop();
	
	return 1;
}

