#include <stdlib.h>
#include <GL/glut.h>
#include <gl/GL.h>
#include <stdio.h>
#include "poliedros.h"


#define MENU_ENTRY_LINES GL_LINE
#define MENU_ENTRY_FILL  GL_FILL
#define MENU_ENTRY_POINT GL_POINT
#define ZOOM_SPEED 0.03f;

int SHOW_EIXOS = 1;
int font1=(int)GLUT_BITMAP_HELVETICA_12;
int font2=(int)GLUT_BITMAP_HELVETICA_10;
int font3=(int)GLUT_BITMAP_9_BY_15;
int curr_polygon_mode = MENU_ENTRY_FILL;
int w=70,h=70;

char estado[200];
float VAL_CHANGE;
float raioCam = 74.317108;
float betaCam=0.45, alfaCam=0.6;
float xla=50.843082, yla=43.496548, zla=raioCam;
float zoom=1.0;
char forma='1';




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
	gluLookAt(xla,yla,zla, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	glPushMatrix();
	glPopMatrix();
	// pôr instruções de desenho aqui
	glScalef(zoom, zoom, zoom);
	glPolygonMode(GL_FRONT, curr_polygon_mode);
	if(SHOW_EIXOS) eixos(30);

	switch(forma) {
		case '1': glColor3f(0,0,1); plano(25.0); break;
		case '2': cubo(25.0); break;
		case '3': cilindro(5.0,20.0,12); break;
		case '4': esfera(13.0,10); break;
		case '0': exit(0);
	}
	// iniciar nova projeccao (gluOrtho2d) para escrever info no ecra
	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	glColor3f(1,0.3,0.3);
	switch(forma) {
		case '1': renderSpacedBitmapString(1,1,1,(void*) font1,"<1#Plano>  2-Cubo   3-Cilindro   4-Esfera   0-Sair"); break;
		case '2': renderSpacedBitmapString(1,1,1,(void*) font1," 1-Plano  <2#Cubo>  3-Cilindro   4-Esfera   0-Sair"); break;
		case '3': renderSpacedBitmapString(1,1,1,(void*) font1," 1-Plano   2-Cubo  <3#Cilindro>  4-Esfera   0-Sair"); break;
		case '4': renderSpacedBitmapString(1,1,1,(void*) font1," 1-Plano   2-Cubo   3-Cilindro  <4#Esfera>  0-Sair"); break;
	}
	
	glColor3f(1,1,1);
	if(SHOW_EIXOS)
		renderSpacedBitmapString(1,3,1,(void*) font3,"Camara: <w> <a> <s> <d>   Zoom: <up> <down>   Eixos: ON  <o>");
	else
		renderSpacedBitmapString(1,3,1,(void*) font3,"Camara: <w> <a> <s> <d>   Zoom: <up> <down>   Eixos: OFF <o>");
	renderSpacedBitmapString(1,5,1,(void*) font2,estado);
	glPopMatrix();

	resetPerspectiveProjection();

	sprintf_s(estado,"beta:%.2f  alfa:%.2f  x:%.2f  y:%.2f  z:%.2f\n", betaCam, alfaCam, xla, yla, zla);
	// End of frame
	glutSwapBuffers();
}



// funcoes auxiliares para demonstracao dos objectos produzidos
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

void resetPerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderSpacedBitmapString(

			float x, 
			float y,
			int spacing, 
			void *font,
			char *string) {

  char *c;
  int x1=x;

  for (c=string; *c != '\0'; c++) {

	glRasterPos2f(x1,y);
	glutBitmapCharacter(font, *c);
	x1 = x1 + spacing;
  }
}



// Atualizar coordenadas do posicionamento da camara
void coord(){
  float r=raioCam;
  float rr=raioCam;
  rr=r*cos(betaCam);
  zla=rr*cos(alfaCam);
  xla=rr*sin(alfaCam);
  yla=r*sin(betaCam);
}


/* .:eixos
 * (desenhar eixos do referencial na cena)
 * parametros:
 * float tam: define intervalo [-tam,tam] onde desenhar segmento dos eixos
 */
void eixos(float tam) {
	glBegin(GL_LINES);
	glColor3f(1,0,0); // eixo xx -> vermelho
	glVertex3f(-tam, 0.0f, 0.0f); // inicio da representacao do eixo xx
	glVertex3f(tam, 0.0f, 0.0f); // fim
	
	glColor3f(0,1,0); // eixo zz -> verde
	glVertex3f(0.0f, 0.0f, -tam); // inicio da representacao do eixo zz
	glVertex3f(0.0f, 0.0f, tam); // fim

	glColor3f(0,0,1); // eixo yy -> azul
	glVertex3f(0.0f, -tam, 0.0f); // inicio da representacao do eixo yy
	glVertex3f(0.0f, tam, 0.0f); // fim
	glEnd( );
}



// Escrever função de processamento do teclado
void keyboard_handle(unsigned char key, int x, int y){
  switch(key) {
    case 'W':
    case 'w':
	  betaCam+=0.05; coord(); break;
    case 'S':
    case 's':
      betaCam-=0.05; coord(); break;
    case 'A':
    case 'a':
      alfaCam-=0.05; coord(); break;
    case 'D':
    case 'd':
      alfaCam+=0.05; coord(); break;
	case 'O':
    case 'o':
	  if(SHOW_EIXOS) { SHOW_EIXOS = 0; break; }
	  else { SHOW_EIXOS = 1; break; }
	case '1':
	  forma = '1'; break;
	case '2':
	  forma = '2'; break;
	case '3':
	  forma = '3'; break;
	case '4':
	  forma = '4'; break;
	case '0':
	  forma = '0'; break;
  }

  glutPostRedisplay();
}


void keyboard_special_handle(int key, int x, int y) {
   switch(key) {
      case GLUT_KEY_UP:
         zoom += ZOOM_SPEED; break;
      case GLUT_KEY_DOWN:
         zoom -= ZOOM_SPEED; break;
   }

   glutPostRedisplay();
}


// escrever função de processamento do menu
void menu_handle(int op) {
   curr_polygon_mode = op;

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
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// pôr aqui registo da funções do teclado e rato
	glutKeyboardFunc(keyboard_handle);
	glutSpecialFunc(keyboard_special_handle);

// pôr aqui a criação do menu
    glutCreateMenu(menu_handle);
   glutAddMenuEntry("Cheio", MENU_ENTRY_FILL);
   glutAddMenuEntry("Linhas", MENU_ENTRY_LINES);
   glutAddMenuEntry("Pontos", MENU_ENTRY_POINT);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	//
	//
   glEnable(GL_CULL_FACE);
	
// entrar no ciclo do GLUT 
	glutMainLoop();
	
	return 1;
}