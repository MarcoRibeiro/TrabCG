#include <math.h>
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
//triangle a, b;

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
	vertex x(-.5, 0.0, 0.5);
	vertex y(0.5, 0.0, 0.5);
	vertex z(0.0, 2.0, 0.0);

	triangle t(x, y, z);
	t.setColor(1, 0, 0);
	triangle b(y, x, z);
	b.setColor(0, 1, 0);
	triangulos.addTriangle(&t);
	triangulos.addTriangle(&b);

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

	iniciaClass();
	triangle* a;
	for (int i = 0; i < 2; i++)
	{
		a = triangulos.getTriangle(i);
		glBegin(GL_TRIANGLES);
		glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
		glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
		glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
		glEnd();

	}
	
	/*
	glColor3f(0,1,0);  //green

	glBegin(GL_TRIANGLES);
		glVertex3f(-.5,0.0,0.5);    //p1
		glVertex3f(0.5,0.0,0.5);	//p2
		glVertex3f(0.0,alt,0.0);      //p5
	glEnd();

	glColor3f(1,0,0); //Red

	glBegin(GL_TRIANGLES);
		glVertex3f(0.5,0.0,0.5);	 //p2
		glVertex3f(0.5,0.0,-0.5);    //p3
		glVertex3f(0.0,alt,0.0);       //p5
	glEnd();
	
	glColor3f(0,0,1); //Blue

	glBegin(GL_TRIANGLES);
		glVertex3f(0.5,0.0,-0.5); 	 //p3
		glVertex3f(-0.5,0.0,-0.5);    //p4
		glVertex3f(0.0,alt,0.0);     //p5
	glEnd();

	glColor3f(1,1,0); //yellow

	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,0.0,-0.5);	 //p4
		glVertex3f(-.5,0.0,0.5);    //p1
		glVertex3f(0.0,alt,0.0);       //p5
	glEnd();

	//base
	glColor3f(r,g,b); //light Blue

	glBegin(GL_TRIANGLES);
		glVertex3f(-.5,0.0,0.5);	 //p1
		glVertex3f(0.5,0.0,-0.5);   //p3
		glVertex3f(0.5,0.0,0.5);       //p2
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex3f(-.5,0.0,0.5);	 //p1
		glVertex3f(-0.5,0.0,-0.5);    //p4
		glVertex3f(0.5,0.0,-0.5);      //p3
	glEnd();
	*/

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

