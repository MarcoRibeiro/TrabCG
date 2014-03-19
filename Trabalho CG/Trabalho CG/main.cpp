#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include "classes.h"
#include <GL/glut.h>

float lookatx=0, lookaty=0, lookatz=5.0; // Não usado
float rotateAng=0.0;
float rotateTop=0.0;

bool linhas = false;  // usado para o menu


float raiocamara = 5;  // raio rotação
float alfa = 0;  // usado para rotação da câmara
float beta = 0;  // usado para rotação da câmara



// class que contem a primitiva
scene cena;


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


void drawScene()
{
	vector<primitive> c = cena.getPrimitivas();
	primitive* paux;
	for (vector<primitive>::iterator iterator = c.begin(); iterator != c.end(); ++iterator)
	{
		paux = iterator._Ptr;
		vector<triangle> aux = paux->getTriangulos();
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
	}
}


void renderScene(void) {

	// clear buffers
	if (linhas==true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(raiocamara*cos(beta)*sin(alfa), raiocamara*sin(beta), raiocamara*cos(beta)*cos(alfa),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);


	// pôr instruções de desenho aqui

	glRotatef(rotateAng,0,1,0);
	glRotatef(rotateTop,1,0,0);

	// Realiza o deseho conforme class cena
	drawScene();

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
			alfa = alfa + 0.087;
			break;
		case GLUT_KEY_LEFT:
			alfa = alfa - 0.087;
			break;
		case GLUT_KEY_UP:
			if (beta < 1.5) 
				beta = beta + 0.087;  // 5 graus 
			break;
		case GLUT_KEY_DOWN:
			if (beta > -1.5)
				beta = beta - 0.087;  // 5 graus
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
		}
		glutPostRedisplay();
	}


// escrever função de processamento do menu
void menu(int opcao) {
		switch (opcao)
		{
		case 1:
			linhas = true;
			break;
		case 2:
			linhas = false;
			break;
		}
		glutPostRedisplay();
	}


/*
	Dado um ficheiro retorna um vector de strings com os ficheiros que contêm os modelos
	*/
vector<string> xmlParse(string file) {

		vector<string> modelos;

		TiXmlDocument doc;
		doc.LoadFile(file.c_str());
		TiXmlElement* root = doc.FirstChildElement();
		const char* attr;
		string value;
		for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
			string elemName = elem->Value();

			if (elemName == "modelo")
			{
				attr = elem->Attribute("ficheiro");
				//aqui guarda-se o attr para uma estrutura qq			
				modelos.push_back(attr);
			}
		}
		return modelos;
	}
/*
	Dado um ficheiro de modelo carrega os triangulos para a cena
	*/
void carregaModelos(string file)
	{
		vector<string> modelos = xmlParse(file);
		for (vector<string>::iterator it = modelos.begin(); it != modelos.end(); ++it)
		{
			cena.addprimitiva(*it);
		}
	}

int main(int argc, char **argv) {
	string file;

	if (argc > 1)
		file = argv[1];
	else
		file = "ficheiro.xml";

	carregaModelos(file);

// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Motor 3D@CG");
	


// registo de funções 
	glutDisplayFunc(renderScene);
//	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(sK);
	glutKeyboardFunc(kP);


// pôr aqui a criação do menu
	int i = glutCreateMenu(menu);
	glutAddMenuEntry("Linhas",1);
	glutAddMenuEntry("Opaco",2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// entrar no ciclo do GLUT 
	glutMainLoop();
	

	return 0;
}

