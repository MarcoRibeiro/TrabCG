#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include "classes.h"
#include <GL/glut.h>


// MENU
#define MNU_LINHAS 0
#define MNU_OPACO 1
#define MNU_PONTOS 2


//variavel que vai conter o ficheiro xml
string file = "";

int linhas = 0;  // default linhas;


// IMPRESSÃO NO ECRA
int font1 = (int)GLUT_BITMAP_HELVETICA_12;
int w = 70, h = 70; // usado para setOrthographicProjection


//MOVIMENTO DA CAMARA
float raiocamara = 5;  // raio rotação
float alfa = 0;  // usado para rotação da câmara
float beta = 0;  // usado para rotação da câmara
float zoom = 1.0;  // zoom inicial
float zoomInc = 0.05;  //incremento zoom

// TAMANHO DA JANELA
unsigned int wwidth = 1200;
unsigned int wheigth = 690;


// CLASS QUE CONTEM TDAS AS PRIMITIVAS
cena c;


// USADO PARA ESCREVER NO ECRA
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
void renderSpacedBitmapString(float x, float y, int spacing, void *font, char *string) {

	char *c;
	int x1 = x;

	for (c = string; *c != '\0'; c++) {

		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + spacing;
	}
}


/*
// CLASS QUE DESENHA NO ECRA
void drawScene()
{
	vector<primitive> c = cena.getPrimitivas();

	for (vector<primitive>::iterator iterator = c.begin(); iterator != c.end(); ++iterator)  // percorrre todas as primitivas
	{
		primitive* paux = iterator._Ptr;



		//Define transformações // melhor invocando outra funcao
		vector<transf*> transformacoes = paux->getTransformacoes();  // carrega todas as transformações que existirem
		if (transformacoes.size() != 0)
		for (vector<transf*>::iterator it2 = transformacoes.begin(); it2 != transformacoes.end(); ++it2)
		{
			transf*  t = *it2._Ptr;
			switch (t->type())
			{
			case TYPE_TRANSLACAO:	glTranslatef(t->x(), t->y(), t->z());	break;
			case TYPE_ROTACAO:  glRotatef(t->angulo(), t->x(), t->y(), t->z()); break;
			case TYPE_ESCALA:  glScalef(t->x(), t->y(), t->z()); break;
			}

		}


		//Desenha primitiva
		vector<triangle> aux = paux->getTriangulos();
		for (vector<triangle>::iterator it = aux.begin(); it != aux.end(); ++it)
		{
			triangle* a = it._Ptr;
			glBegin(GL_TRIANGLES);
			glColor3f(a->getColorR(), a->getColorG(), a->getColorB());
			glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
			glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
			glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
			glEnd();
		}
	}
}
*/


//NÃO UTILIZADO
void drawPrimitiva(primitive p)
{
	vector<triangle> aux = p.getTriangulos();
	for (vector<triangle>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		triangle* a = it._Ptr;
		glBegin(GL_TRIANGLES);
		glColor3f(a->getColorR(), a->getColorG(), a->getColorB());
		glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
		glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
		glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
		glEnd();
	} 
}

//NÃO UTILIZADO
void drawPrimitiva(primitive* p)
{
	vector<triangle> aux = p->getTriangulos();
	for (vector<triangle>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		triangle* a = it._Ptr;
		glBegin(GL_TRIANGLES);
		glColor3f(a->getColorR(), a->getColorG(), a->getColorB());
		glVertex3f(a->getP1().getX(), a->getP1().getY(), a->getP1().getZ());
		glVertex3f(a->getP2().getX(), a->getP2().getY(), a->getP2().getZ());
		glVertex3f(a->getP3().getX(), a->getP3().getY(), a->getP3().getZ());
		glEnd();
	}
}

//Le de um filho <modelos> e guarda todas as primitivas na classe cena
void drawModelos(TiXmlElement* modelos) {
	const char* attr;
	for (TiXmlElement* elem = modelos->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		if (elemName == "modelo")
		{
			attr = elem->Attribute("ficheiro");
			c.addPrimitiva(new primitive(attr));
		}
	}

}

//Le recursivamente todo um grupo e armazena todas as transformações/primitivas na classe cena 
//na mesma ordem que vao sendo imprimidas
void drawGrupo(TiXmlElement* grupo) {
	//glPushMatrix();
	c.addTransf(new psMatrix());
	for (TiXmlElement* elem = grupo->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		string elemName = elem->Value();
		if (elemName == "grupo") {
			drawGrupo(elem);
		}
		else if (elemName == "modelos") {
			drawModelos(elem);
		}
		else if (elemName == "translacao") {
			float x=0, y=0, z=0;
			if (elem->QueryFloatAttribute("X", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Y", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Z", &z) == TIXML_SUCCESS) {}
			c.addTransf(new translacao(x, y, z));
		}
		else if (elemName == "rotacao") {
			float a=0, x = 0, y = 0, z = 0;
			if (elem->QueryFloatAttribute("angulo", &a) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoX", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoY", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoZ", &z) == TIXML_SUCCESS) {}
			c.addTransf(new rotacao(a, x, y, z));
		}
		else if (elemName == "escala") {
			float x = 0, y = 0, z = 0;
			if (elem->QueryFloatAttribute("X", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Y", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Z", &z) == TIXML_SUCCESS) {}
			glScalef(x, y, z);
			c.addTransf(new escala(x, y, z));
		}
	}
	c.addTransf(new ppMatrix());
}


// Carrega em memoria todos as primitivas e transformações de um ficheiro Xml (usa drawGrupo e DrawModelos)
void drawXml(string ficheiro) {
	TiXmlDocument doc;
	doc.LoadFile(ficheiro.c_str());
	TiXmlElement* root = doc.FirstChildElement();
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		string elemName = elem->Value();
		if (elemName == "grupo")
		{
			drawGrupo(elem);
		}
	}
}


//Desenha uma cena no ecra por ordem que armazenamento
void drawCena(cena c)
{
	vector<drawable*> aux = c.getItens();
	for (vector<drawable*>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		drawable* d = *it._Ptr;
		d->draw();
	}
}


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



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(raiocamara*cos(beta)*sin(alfa), raiocamara*sin(beta), raiocamara*cos(beta)*cos(alfa),
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);



	//inserir instruções de desenho
	glScalef(zoom, zoom, zoom);

	switch (linhas) {
	case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
	}


	drawCena(c);

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
			linhas = MNU_LINHAS;
			break;
		case 2:
			linhas = MNU_OPACO;
			break;
		case 3:
			linhas = MNU_PONTOS;
		}
		glutPostRedisplay();
	}


/*
vector<string> desenha_cena(TiXmlDocument elem)
{
	vector<string> modelos;

	TiXmlDocument doc;
	doc.LoadFile(file.c_str());
	TiXmlElement* root = doc.FirstChildElement();
	const char* attr;

	const char* x;
	const char* y;
	const char* z;

	string value;
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		if (elemName == "translacao")
		{
			x = elem->Attribute("X");
			y = elem->Attribute("Y");
			z = elem->Attribute("Z");
		}

		if (elemName == "modelo")
		{
			attr = elem->Attribute("ficheiro");
			modelos.push_back(attr);
		}
	}
	return modelos;
}*/


/*
//	Dado um ficheiro xml carrega todos as primitivas para a cena
void carregaModelos(string file) {
		vector<string> modelos = xmlParse(file);

		for (vector<string>::iterator it = modelos.begin(); it != modelos.end(); ++it)
		{
			cena.addprimitiva(*it);
		}
}*/



int main(int argc, char **argv) {


// Aberta por default do ficheiro "ficheiro.xml" ou outro fornecido como parametro
	if (argc > 1)
		file = argv[1];
	else
		file = "ficheiro.xml";

// Carrega dados para a class cena
//	carregaModelos(file);

// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	int ww = (glutGet(GLUT_SCREEN_WIDTH) - wwidth) / 2;
//	int hh = (glutGet(GLUT_SCREEN_HEIGHT) - wheigth) / 2;
	glutInitWindowPosition(ww, 0);
	glutInitWindowSize(wwidth,wheigth);
	glutCreateWindow("Motor 3D@CG");


	drawXml(file);

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

