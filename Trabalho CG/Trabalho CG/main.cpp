#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include "classes.h"
#include <GL/glut.h>


int font1 = (int)GLUT_BITMAP_HELVETICA_12;


float lookatx=0, lookaty=0, lookatz=5.0; // Não usado
float rotateAng=0.0;
float rotateTop=0.0;

int linhas = 0;  // 0 -> linhas; 1 -> opaco; 3-> pontos  usado para o menu


float raiocamara = 5;  // raio rotação
float alfa = 0;  // usado para rotação da câmara
float beta = 0.5;  // usado para rotação da câmara
float zoom = 1.0;  // zoom inicial
float zoomInc = 0.05;  //incremento zoom

int w = 70, h = 70; // usado para setOrthographicProjection

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

void resetPerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);

	// invert the y axis, down is positive
	glScalef(1, -1, 1);

	// move the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}


void renderSpacedBitmapString( float x,	float y, int spacing, void *font, char *string) {

	char *c;
	int x1 = x;

	for (c = string; *c != '\0'; c++) {

		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + spacing;
	}
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(raiocamara*cos(beta)*sin(alfa), raiocamara*sin(beta), raiocamara*cos(beta)*cos(alfa),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glPopMatrix();


	//inserir instruções de desenho
	glScalef(zoom, zoom, zoom);

	switch (linhas) {
	case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
	}


	// Permite rodar objecto NÂO UTILIZADO
	glRotatef(rotateAng, 0, 1, 0);
	glRotatef(rotateTop, 1, 0, 0);

	// Realiza o deseho conforme class cena
	drawScene();


	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);

	renderSpacedBitmapString(1, 2, 1, (void*)font1, " Zoom in: <a>   Zoom out: <z>   Camara: <up> <down> <left> <right>");

	glPopMatrix();

	resetPerspectiveProjection();

	// End of frame
	glutSwapBuffers();
}

// escrever função de processamento do teclado

void sK(int codigo, int x, int y)
	{
		switch(codigo)
		{

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
		case 'a':
			zoom += zoomInc;
			break;
		case 'z':
			zoom -= zoomInc;
			break;
		}
		glutPostRedisplay();
	}


// escrever função de processamento do menu
void menu(int opcao) {
		switch (opcao)
		{
		case 1:
			linhas = 0;
			break;
		case 2:
			linhas = 1;
			break;
		case 3:
			linhas = 2;
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
	glutAddMenuEntry("Pontos",3);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// entrar no ciclo do GLUT 
	glutMainLoop();
	

	return 0;
}

