#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include <glew.h>
#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include "classes.h"
#include <GL/glut.h>


// MENU
#define MNU_LINHAS 0
#define MNU_OPACO 1
#define MNU_PONTOS 2

GLuint* buffers;
int* npontos;  //usado para armazenar em paralelo a quantidade de pontos de cada primitiva


//variavel que vai conter o ficheiro xml inicializado a ""
string file = "";

int linhas = 0;  // Usado para selecionar visualização por linhas opaco ou pontos
extern bool tanslationLines = true;

// IMPRESSÃO NO ECRA
int font1 = (int)GLUT_BITMAP_HELVETICA_12;
int w = 70, h = 70; // usado para setOrthographicProjection


//MOVIMENTO DA CAMARA
//float raiocamara = 5;  // raio rotação
float alfa = 0;  // usado para rotação da câmara	DESACTIVADOS PARA UTILIZACAO DO RATO
//float beta = 0;  // usado para rotação da câmara


float zoom = 1.0;  // zoom inicial
float zoomInc = 0.05;  //incremento zoom

float panlr = 0;
float panud = 0;
float panbf = 0;

//MOVIMENTO DO RATO
float camX = 0, camY, camZ = 5;
int startX, startY, tracking = 0;
int alpha = 0, beta = 0, r = 5;


// TAMANHO DA JANELA
unsigned int wwidth = 1300;
unsigned int wheigth = 690;

// Contador de frames
int timebase = 0, frame = 0;



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





// PASSAR AS FUNCOES DO XML PARA FICHEIROS SEPARADOS
//Le de um filho <modelos> e guarda todas as primitivas na classe cena
void readModelos(TiXmlElement* modelos) {
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

void readTranslacaoT(TiXmlElement* modelos, float tempo) {
	vector<float> curva;

	for (TiXmlElement* elem = modelos->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		if (elemName == "ponto")
		{
			float x = 0, y = 0, z = 0;
			if (elem->QueryFloatAttribute("X", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Y", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Z", &z) == TIXML_SUCCESS) {}
			curva.push_back(x); curva.push_back(y);	curva.push_back(z);
		}
	}
	c.addTransf(new translacaoT(curva,tempo));
}

//Le recursivamente todo um <grupo> e armazena todas as transformações/primitivas na classe cena 
//na mesma ordem que devem sao lidas	
void readGrupo(TiXmlElement* grupo) {
	//glPushMatrix();
	c.addTransf(new psMatrix());
	for (TiXmlElement* elem = grupo->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		string elemName = elem->Value();
		if (elemName == "grupo") {
			readGrupo(elem);
		}
		else if (elemName == "modelos") {
			readModelos(elem);
		}
		else if (elemName == "translacaoT") {
			float t;
			if (elem->QueryFloatAttribute("tempo", &t) == TIXML_SUCCESS) {}
			readTranslacaoT(elem,t);
		}
		else if (elemName == "translacao") {
			float x=0, y=0, z=0;
			if (elem->QueryFloatAttribute("X", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Y", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("Z", &z) == TIXML_SUCCESS) {}
			c.addTransf(new translacao(x, y, z));
		}
		else if (elemName == "rotacao") {
			float a = 0, x = 0, y = 0, z = 0;
			if (elem->QueryFloatAttribute("angulo", &a) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoX", &x) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoY", &y) == TIXML_SUCCESS) {}
			if (elem->QueryFloatAttribute("eixoZ", &z) == TIXML_SUCCESS) {}
			c.addTransf(new rotacao(a, x, y, z));
		}
		else if (elemName == "rotacaoT") {
				float t = 0, x = 0, y = 0, z = 0;
				if (elem->QueryFloatAttribute("tempo", &t) == TIXML_SUCCESS) {}
				if (elem->QueryFloatAttribute("eixoX", &x) == TIXML_SUCCESS) {}
				if (elem->QueryFloatAttribute("eixoY", &y) == TIXML_SUCCESS) {}
				if (elem->QueryFloatAttribute("eixoZ", &z) == TIXML_SUCCESS) {}
				c.addTransf(new rotacaoT(t, x, y, z));
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

// Carrega em memoria todos as primitivas e transformações de um ficheiro Xml (usa readGrupo e readModelos)
void readXml(string ficheiro) {
	TiXmlDocument doc;
	doc.LoadFile(ficheiro.c_str());
	TiXmlElement* root = doc.FirstChildElement();
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
		string elemName = elem->Value();
		if (elemName == "grupo")
		{
			readGrupo(elem);
		}
	}
}
////////////////////////////////////



//Desenha uma cena no ecra por ordem que armazenamento //  OBSOLETO APOS VBO
void drawCena(cena c)
{
	vector<drawable*> aux = c.getItens();
	for (vector<drawable*>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		drawable* d = *it._Ptr;
		d->draw();
	}
}


// aloca dinamicamente variaveis buffers e npontos
void iniciaVBO() {

	vector<primitive*> primitivas = c.getPrimitivas();
	int n_primitivas = c.getN_primitivas();  //numero de objectos


	buffers = (GLuint*)malloc(sizeof(GLuint)*n_primitivas);
	npontos = (int*)malloc(sizeof(int)*n_primitivas);
	if ((buffers == NULL) || (npontos == NULL)) exit(1);


	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(n_primitivas, buffers);

	// carrega todos os bufers para a placa gráfica
	int i = 0;
	for (vector<primitive*>::iterator it = primitivas.begin(); it != primitivas.end(); it++) {

		vector<float> pontos = (*it._Ptr)->getPontos();
		npontos[i] = pontos.size(); // guarda o numero de pontos da primitiva

		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, pontos.size()*sizeof(float), &pontos[0], GL_STATIC_DRAW);
		
		i++;
	}
}

//Percorre todos os itens desenhaveis, desenha transformações na classe cena
//e objectos carregados para a placa grafica.. Assume 

void drawCenaVBO(cena c)
{
	int i = 0;
	vector<drawable*> aux = c.getItens();
	for (vector<drawable*>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		drawable* d = *it._Ptr;
		if (d->type() == TYPE_PRIMITIVE) {
			//	primitive* p = (primitive*) d;
			glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, npontos[i]);
			i++;
		}
		else {
			d->draw(); // imprime transformações
		}
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
	float fps;
	int time;

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	/* DESACTIVADO PARA ACTIVAR MOVIMENTO COM O RATO
	gluLookAt(raiocamara*cos(beta)*sin(alfa)+panlr, raiocamara*sin(beta)+panud, raiocamara*cos(beta)*cos(alfa)+panbf,
		0.0+panlr, 0.0+panud, 0.0+panbf,
		0.0f, 1.0f, 0.0f);*/



	//inserir instruções de desenho
	glScalef(zoom, zoom, zoom);

	switch (linhas) {
	case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
	}


	drawCenaVBO(c);


	///////////IMPRESSAO AJUDA/////////////////
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);

	renderSpacedBitmapString(1, 2, 1, (void*)font1, " Zoom in: <a>   Zoom out: <z>   Camara: <up> <down> <left> <right>");

	glPopMatrix();
	resetPerspectiveProjection();
	

	//////////////// FPS ///////////////////////
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame*1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
		std::string buff = std::to_string(fps);
		glutSetWindowTitle(buff.c_str());
	}

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
		case 'r':
			panbf += 0.3;
			break;
		case 'f':
			panbf -= 0.3;
			break;
		case 'q':
			panlr -= 0.3;
			break;
		case 'e':
			panlr += 0.3;
			break;
		case 'w':
			panud += 0.3;
			break;
		case 's':
			panud -= 0.3;
			break;
		}
		glutPostRedisplay();
	}

void processMouseButtons(int button, int state, int xx, int yy)
{
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {

		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux *							     sin(betaAux * 3.14 / 180.0);
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
		case 4:
			 
			if (tanslationLines) tanslationLines = false;
			else tanslationLines = true;
			break;
		}
		glutPostRedisplay();
	}




int main(int argc, char **argv) {


// Aberta por default do ficheiro "ficheiro.xml" ou outro fornecido como parametro
	if (argc > 1)
		file = argv[1];
	else
		file = "ficheiro.xml";


// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	int ww = (glutGet(GLUT_SCREEN_WIDTH) - wwidth) / 2;
//	int hh = (glutGet(GLUT_SCREEN_HEIGHT) - wheigth) / 2;
	glutInitWindowPosition(ww, 0);
	glutInitWindowSize(wwidth,wheigth);
	glutCreateWindow("Motor 3D@CG");
	glewInit();

// carrega o ficheiro xml para a estrutura
	readXml(file);

//  converte cena em vbo e carrega para a placa gráfica
	iniciaVBO();

// registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// pôr aqui registo da funções do teclado e rato
	glutSpecialFunc(sK);
	glutKeyboardFunc(kP);

	// pôr aqui registo da funções do rato
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

// pôr aqui a criação do menu
	int i = glutCreateMenu(menu);
	glutAddMenuEntry("Exibir Linhas",1);
	glutAddMenuEntry("Exibir Opaco",2);
	glutAddMenuEntry("Exibir Pontos",3);
	glutAddMenuEntry("Exibir Linhas de Tanslação", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// entrar no ciclo do GLUT 
	glutMainLoop();
	

	//return 0;
}

