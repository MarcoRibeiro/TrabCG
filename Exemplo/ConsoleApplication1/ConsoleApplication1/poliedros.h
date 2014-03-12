#include <GL/glut.h>
#include <gl/GL.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1415926535897932384626433832795

extern void cilindro(float raio, float altura, int ppc);
extern void plano(float comprimento);
extern void cubo(float aresta);
extern void esfera(float raio, int complex);
void eixos(float tam);
void resetPerspectiveProjection();
void setOrthographicProjection();
void renderSpacedBitmapString(float x, float y, int spacing, void *font, char *string);