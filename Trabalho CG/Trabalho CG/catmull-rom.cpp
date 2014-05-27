#include <GL/glut.h>
#include <math.h>



void getCatmullRomPoint(float** p, float t, int *indices, float *res) {

	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
	// Calcular o ponto res = T * M * P
	// sendo Pi = p[indices[i]]

	float t3[4] = { t*t*t, t*t, t, 1 };
	float tparcial = 0;
	float aux = 0;


	for (int k = 0; k < 3; k++){
		aux = 0;
		for (int i = 0; i < 4; i++) {
			tparcial = 0;
			for (int j = 0; j < 4; j++) {
				tparcial += t3[j] * m[j][i];
			}
			aux += tparcial * p[indices[i]][k];
		}
		res[k] = aux;
	}

}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float **p,int point_count, float gt, float *res) {

	float t = gt * point_count; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + point_count - 1) % point_count;	indices[1] = (indices[0] + 1) % point_count;
	indices[2] = (indices[1] + 1) % point_count; indices[3] = (indices[2] + 1) % point_count;

	getCatmullRomPoint(p, t, indices, res);
}


void renderCatmullRomCurve(float** p, int point_count) {

	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	float n_pontos = 100;
	float res[3];
	float inc = 1 / n_pontos;
	float f = 0;


	glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINE_LOOP);
	for (int a = 0; a < n_pontos; a++)
	{
		f += inc;
		getGlobalCatmullRomPoint(p,point_count, f, res);
		glVertex3fv(res);
	}
	glEnd();
	glEnable(GL_LIGHTING);

}