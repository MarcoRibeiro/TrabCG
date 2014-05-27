#define _USE_MATH_DEFINES

#include <math.h>
#include <fstream>
#include <iostream>
#include <glew.h>
#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include "classes.h"
#include <GL/glut.h>
#include <IL/il.h>


// MENU
#define MNU_LINHAS 0
#define MNU_OPACO 1
#define MNU_PONTOS 2

GLuint* buffers;  //coordenadas dos pontos
GLuint* normais;  //coordenadas das normais
GLuint* texCoord; //coordenadas das texturas
GLuint* texID;    //buffer texturas

GLuint textBackground = 100;
bool background = true;

int GLprimitiva=0;

//variavel que vai conter o ficheiro xml inicializado a ""
string file = "";

int linhas = 1;  // Usado para selecionar visualização por linhas opaco ou pontos
extern bool tanslationLines = true;

// IMPRESSÃO NO ECRA
int font1 = (int)GLUT_BITMAP_HELVETICA_12;
int w = 70, h = 70; // usado para setOrthographicProjection


//MOVIMENTO DA CAMARA

float alfa = 0;  

//MOVIMENTO CAMARA
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

//MOVIMENTO DO RATO
float camX = 0, camY=0, camZ = 5;

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
	const char* textura;
	vector<material> materiais;
	for (TiXmlElement* elem = modelos->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		if (elemName == "modelo")
		{
			float r = 0, g = 0, b = 0;
			attr = elem->Attribute("ficheiro");	
			textura = elem->Attribute("textura");
			if (elem->QueryFloatAttribute("diffR", &r) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("diffG", &g) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("diffB", &b) == TIXML_SUCCESS) {
				materiais.push_back(material(1, r, g, b, 1.0f)); //1->TYPE_DIFFUSE
			}
			if (elem->QueryFloatAttribute("emiR", &r) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("emiG", &g) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("emiB", &b) == TIXML_SUCCESS) {
				materiais.push_back(material(2,r, g, b, 1.0f)); //2->TYPE_EMISSION
			}
			if (elem->QueryFloatAttribute("ambR", &r) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("ambG", &g) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("ambB", &b) == TIXML_SUCCESS) {
				materiais.push_back(material(3, r, g, b, 1.0f)); //3->TYPE_AMBIENT
			}
			if (elem->QueryFloatAttribute("speR", &r) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("speG", &g) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("speB", &b) == TIXML_SUCCESS) {
				materiais.push_back(material(4, r, g, b, 1.0f)); //4->TYPE_SPECULAR
			}
			if (elem->QueryFloatAttribute("ambdifR", &r) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("ambdifG", &g) == TIXML_SUCCESS &&
				elem->QueryFloatAttribute("ambdifB", &b) == TIXML_SUCCESS) {
				materiais.push_back(material(5, r, g, b, 1.0f)); //5->TYPE_AMB_AND_DIFF
			}
			if (elem->QueryFloatAttribute("shininess", &r) == TIXML_SUCCESS) {
				materiais.push_back(material(6,r)); //6->TYPE_SHININESS
			}

			int id = c.exists(attr);
			if (id == -1) id = c.addPrimitiva(new primitiveVBO(attr));
			textura != NULL ?	c.addModelo(new modelo(attr, id, textura,materiais)) : c.addModelo(new modelo(attr, id,materiais));
		
		
		
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



void loadTexture(ILstring file, GLuint Tid) {

	unsigned int t, tw, th;
	unsigned char *texData;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage(file);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	texData = ilGetData();  //textura

	//	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, Tid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);  //copia para a grafica

}

/*
retorna indice dos buffers
*/
int loadPrimitive(primitiveVBO* p)
{

	p->setBuffer(GLprimitiva);
	vector<vertex> vertices = p->getPontos();


	vector<float> pontos;
	vector<float> pnormais;
	vector<float> tex2d;

	for (vector<vertex>::iterator it2 = vertices.begin(); it2 != vertices.end(); it2++)
	{
		vertex* v = it2._Ptr;
		pontos.push_back(v->getX()); pontos.push_back(v->getY()); pontos.push_back(v->getZ());
		pnormais.push_back(v->getNx()); pnormais.push_back(v->getNy()); pnormais.push_back(v->getNz());
		tex2d.push_back(v->getTx()); tex2d.push_back(v->getTy());
	}


	glBindBuffer(GL_ARRAY_BUFFER, buffers[GLprimitiva]);
	glBufferData(GL_ARRAY_BUFFER, pontos.size()*sizeof(float), &pontos[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normais[GLprimitiva]);
	glBufferData(GL_ARRAY_BUFFER, pnormais.size()*sizeof(float), &pnormais[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoord[GLprimitiva]);
	glBufferData(GL_ARRAY_BUFFER, tex2d.size()*sizeof(float), &tex2d[0], GL_STATIC_DRAW);

	GLprimitiva++;
	return GLprimitiva - 1;

}


// aloca dinamicamente variaveis buffers e npontos
void iniciaVBO() {

	vector<modelo*> modelos = c.getModelos();
	vector<primitiveVBO*> primitivas = c.getPrimitivas();
	int n_primitivas = c.getN_primitivas();  //numero de objectos
	int n_texturas = c.getNtexturas();

	int lastloadedPrimitive = -1;

	//Inicialização de variaveis globais
	buffers = (GLuint*)malloc(sizeof(GLuint)*n_primitivas);
	normais = (GLuint*)malloc(sizeof(GLuint)*n_primitivas);
	texCoord = (GLuint*)malloc(sizeof(GLuint)*n_primitivas);
	texID = (GLuint*)malloc(sizeof(GLuint)*n_texturas);


	if (buffers == NULL || normais == NULL ||
		normais == NULL || texID == NULL) exit(1);

	glGenBuffers(n_primitivas, buffers);
	glGenBuffers(n_primitivas, normais);
	glGenBuffers(n_primitivas, texCoord);
	glGenBuffers(n_texturas, texID);


	// carrega todos os bufers para a placa gráfica
	int i = 0;  //nº de texturas

	for (vector<modelo*>::iterator it = modelos.begin(); it != modelos.end(); it++) {
		bool text = false;
		int id = (*it._Ptr)->getId();

		if (id > lastloadedPrimitive)  // se ainda não foi carregada a primitiva
		{
			if ((*it._Ptr)->getTextura() != "") {
				text = true;
				loadTexture((ILstring)(*it._Ptr)->getTextura().c_str(), texID[i]);
			}

			int p = loadPrimitive(primitivas[id]);  //a primitva fica guardada nos buffers com pos p
			(*it._Ptr)->setBuffer(p);
			
			if(text==true) (*it._Ptr)->setBufferText(i++);

			lastloadedPrimitive++;
		}
		else
		{
			if ((*it._Ptr)->getTextura() != "") {
				text = true;
				loadTexture((ILstring)(*it._Ptr)->getTextura().c_str(), texID[i]);
			}

			int p = primitivas[id]->getBuffer();
			(*it._Ptr)->setBuffer(p); 
			
			if (text == true) (*it._Ptr)->setBufferText(i++);
		}
	}
}

void drawMateriais(vector<material> materiais)
{
	for (vector<material>::iterator it = materiais.begin(); it != materiais.end(); ++it)
	{
		float r = it->getR();
		float g = it->getG();
		float b = it->getB();
		float a = it->getA();
		float m[] = { r, g, b, a };

		switch (it->getType())
		{
		case 1:	glMaterialfv(GL_FRONT, GL_DIFFUSE, m); break;
		case 2:	glMaterialfv(GL_FRONT, GL_EMISSION, m); break;
		case 3:	glMaterialfv(GL_FRONT, GL_AMBIENT, m); break;
		case 4:	glMaterialfv(GL_FRONT, GL_SPECULAR, m); break;
		case 5:	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m); break;
		case 6:	glMaterialf(GL_FRONT, GL_SHININESS,it->getShininess()); break;
		}
	}
}

void clearMaterials() {
	GLfloat amb[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat dif[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat spec[] = { 0.0, 0.0, 0.0, 1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_EMISSION, spec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
//	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, );
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

}

//Percorre todos os itens desenhaveis, desenha transformações na classe cena
//e objectos carregados para a placa grafica.. 
void drawCenaVBO(cena c)
{
	bool mat = true;
	vector<drawable*> aux = c.getItens();
	vector<primitiveVBO*> primitivas = c.getPrimitivas();
	for (vector<drawable*>::iterator it = aux.begin(); it != aux.end(); ++it)
	{
		drawable* d = *it._Ptr;
		if (d->type() == TYPE_PRIMITIVE) {  //imprime primitivas
			modelo* p = (modelo*) d;
			

			vector<material> materiais = p->getMateriais();
			if (materiais.size() != 0) {
				drawMateriais(materiais); mat = true;
			}
			else if (mat == true) {
				clearMaterials();
				mat = false;
			}

			//Desenha primitiva
			vector<int> indices = primitivas[p->getId()]->getIndices();

			glBindBuffer(GL_ARRAY_BUFFER, buffers[p->getBuffer()]);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, normais[p->getBuffer()]);
			glNormalPointer(GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER,  texCoord[p->getBuffer()]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			if (p->getBufferText() != -1) 
				glBindTexture(GL_TEXTURE_2D, texID[p->getBufferText()]);
			
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
			
			if (p->getBufferText() != -1)
				glBindTexture(GL_TEXTURE_2D, 0);

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


float* getPontoD(float* l, float* p)
{
	float *res = (float*)malloc(sizeof(float)* 3); // resultado
	res[0] = l[0] - p[0];
	res[1] = l[1] - p[1];
	res[2] = l[2] - p[2];
	return res;
}

float* prodExterno(float* va, float* vb) {

	float *res = (float*)malloc(sizeof(float)* 3); // resultado

	

	float x = (va[1] * vb[2]) - (va[2] * vb[1]);		//produto externo
	float y = -1 * ((va[0] * vb[2]) - (va[2] * vb[0]));
	float z = (va[0] * vb[1]) - (va[1] * vb[0]);


	res[0] = x;
	res[1] = y;
	res[2] = z;

	return res;
}

void displayBackground()
{
	//NEW
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	glBindTexture(GL_TEXTURE_2D, textBackground);
	// Draw 2d graphics

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 1, 0);
	glEnd();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)wwidth / (GLfloat)wheigth, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);

}
void renderScene(void) {
	float fps;
	int time;

	// clear buffers


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (background == true) displayBackground();


	// set the camera
	glLoadIdentity();



	gluLookAt(camX, camY, camZ,
		centerX, centerY, centerZ,
		upX, upY, upZ);



	GLfloat whiteSpecularLight[] = { 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 0.15, 1.0 };
	GLfloat mat_shininess[] = { 90.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };


	GLfloat redDiffuseMaterial[] = { 1.0,1.0,1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, whiteSpecularLight);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);

	//glLightfv(GL_LIGHT0, GL_EMISSION, mat_specular);


	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


	//inserir instruções de desenho

	//glDisable(GL_LIGHTING);

	//glColor3f(1, 0, 1);

	//glutSolidSphere(1, 10, 10);

	glBindTexture(GL_TEXTURE_2D, 0);

	drawCenaVBO(c);




	switch (linhas) {
	case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	case 2: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
	}



	/*
	///////////IMPRESSAO AJUDA/////////////////
	glDisable(GL_LIGHTING);
	
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);

	renderSpacedBitmapString(1, 2, 1, (void*)font1, " Zoom in: <a>   Zoom out: <z>   Camara: <up> <down> <left> <right>");

	glPopMatrix();
	resetPerspectiveProjection();
	
	glEnable(GL_LIGHTING);
	*/
	

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

void sK(int codigo, int x, int y){

	float* d;
	float p[3] = { camX, camY, camZ };
	float l[3] = { centerX, centerY, centerZ };
	float up[3] = { upX, upY, upZ };
	d = getPontoD(l, p);
	
	float *vr = prodExterno(d, up);

	switch (codigo)
	{

	case GLUT_KEY_RIGHT:
		camX += 0.02*vr[0];
		camY += 0.02*vr[1];
		camZ += 0.02*vr[2];
		centerX += 0.02*vr[0];
		centerY += 0.02*vr[1];
		centerZ += 0.02*vr[2];
		break;
	case GLUT_KEY_LEFT:
		camX -=	0.02*vr[0];
		camY -= 0.02*vr[1];
		camZ -= 0.02*vr[2];
		centerX -= 0.02*vr[0];
		centerY -= 0.02*vr[1];
		centerZ -= 0.02*vr[2];
		break;
	case GLUT_KEY_UP:
		camX += 0.02*d[0];
		camY += 0.02*d[1];
		camZ += 0.02*d[2];
		centerX += 0.02*d[0];
		centerY += 0.02*d[1];
		centerZ += 0.02*d[2];
		break;
	case GLUT_KEY_DOWN:
		camX -= 0.02*d[0];
		camY -= 0.02*d[1];
		camZ -= 0.02*d[2];
		centerX -= 0.02*d[0];
		centerY -= 0.02*d[1];
		centerZ -= 0.02*d[2];
		break;
	case GLUT_KEY_PAGE_UP:
		camX += 0.02*up[0];
		camY += 0.02*up[1];
		camZ += 0.02*up[2];
		centerX += 0.02*up[0];
		centerY += 0.02*up[1];
		centerZ += 0.02*up[2];
		break;
	case GLUT_KEY_PAGE_DOWN:
		camX -= 0.02*up[0];
		camY -= 0.02*up[1];
		camZ -= 0.02*up[2];
		centerX -= 0.02*up[0];
		centerY -= 0.02*up[1];
		centerZ -= 0.02*up[2];
		break;
	}
		glutPostRedisplay();
}
void kP(unsigned char codigo, int x, int y)
	{
		switch(codigo)
		{
		case 'a':
			centerX -= 0.1;
			break;
		case 'd':
			centerX += 0.1;
			break;
		case 'b':
			if (background == true) background = false;
			else background = true;
			break;
		case 'f':

			break;
		case 'q':

			break;
		case 'e':

			break;
		case 'w':
			centerY += 0.1;

			break;
		case 's':
			centerY -= 0.1;

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


void initGL() {

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);

	glEnable(GL_TEXTURE_2D);
}

int main(int argc, char **argv) {
	float a = 0;
	float t = glutGet(GLUT_ELAPSED_TIME);


// Aberta por default do ficheiro "ficheiro.xml" ou outro fornecido como parametro
	if (argc > 1)
		file = argv[1];
	else
		file = "ficheiro.xml";


// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	int ww = (glutGet(GLUT_SCREEN_WIDTH) - wwidth) / 2;
	glutInitWindowPosition(ww, 0);
	glutInitWindowSize(wwidth,wheigth);
	glutCreateWindow("Motor 3D@CG");
	glewInit();

	
//Inicia alguns settings OpenGL
	initGL();


// carrega o ficheiro para a estrutura
	readXml(file);

	loadTexture((ILstring)"background.jpg", textBackground);


	a = glutGet(GLUT_ELAPSED_TIME) - t;
	std::cout << "Leitura Xml - " << a/1000.0f << endl;
//  converte cena em vbo e carrega para a placa gráfica
	t = glutGet(GLUT_ELAPSED_TIME);

	iniciaVBO();
	a = glutGet(GLUT_ELAPSED_TIME) - t;
	std::cout << "Inicializacao vbo - " << a/1000.0f << endl;


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

	
// entrar no ciclo do GLUT 
	glutMainLoop();
	

	//return 0;
}

