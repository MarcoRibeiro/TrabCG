#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glut.h>


using namespace std;


#define N_PATCHES_BEZIER 16  //numero de pontos num patch
#define N_PONTOS 3 // numero de coordenadas num ponto
 
#define POINT_COUNT 4 //numero de pontos numa curva


float camX, camY, camZ = 10;
int startX, startY, tracking = 0;

int alpha = 0, beta = 0, r = 10;

float incr = 360 / 200;
float delta = 0;

GLuint buffers[1];

float a = 0;



class patchFile{

private:

	int **patches;
	float **vertices;

	int npatches;
	int nvertices;

	//posicao no array
	int row;


	void error_message(int n_erro) {
		string messages[] = {
			"Erro: Não foi possivel ler número patches\n",
			"Erro: Não foi possivel ler linha de patches\n",
			"Erro: Não foi possivel ler número vertices\n",
			"Erro: Não foi possivel ler linha de vertices\n",
			"Erro: Não foi possivel abrir ficheiro" };

		std::cerr << messages[n_erro];
	}

	int addPatches(string str) {
		stringstream ss(stringstream::in | stringstream::out);
		string::size_type sz;

		int index = 0;
		int pos = 0;

		while (str[index] != '\n')
		{
			// se for conteudo
			if ((str[index] != ',') && (str[index] != '\0'))
			{
				if (str[index] != ' ')
					ss << str[index];
			}
			//se for carater de separação
			else
			{
				patches[row][pos++] = std::stoi(ss.str(), &sz);
				ss.str(std::string()); // clear buffer
				if (pos == N_PATCHES_BEZIER)
				{
					pos = 0;
					row++;
					index = 0;
					return 0;
				}
			}
			index++;
		}
		return 1; // sai com erro
	}
	int addVertices(string str) {
		stringstream ss(stringstream::in | stringstream::out);
		string::size_type sz;

		int index = 0;
		int pos = 0;

		while (str[index] != '\n')
		{
			// se for conteudo
			if ((str[index] != ',') && (str[index] != '\0'))
			{
				if (str[index] != ' ')
					ss << str[index];
			}
			//se for carater de separação
			else
			{
				vertices[row][pos++] = std::stof(ss.str(), &sz);
				ss.str(std::string()); // clear buffer
				if (pos == N_PONTOS)
				{
					pos = 0;
					row++;
					index = 0;
					return 0;
				}
			}
			index++;
		}
		return 1; //sai com erro
	}

	int loadFile(string file)  // retorna 1 caso leitura com sucesso 0 se falha
	{

		stringstream content(stringstream::in | stringstream::out);
		string line;
		ifstream myfile(file);


		if (myfile.is_open())
		{
			//Lê numero de patches
			if (getline(myfile, line))
			{
				try	{ npatches = stoi(line); }
				catch (invalid_argument) { error_message(0); return 1; }
			}
			else { error_message(0); return 1; }

			row = 0;
			//lê patches
			patches = (int**)malloc(npatches*sizeof(int*));
			for (int j = 0; j < npatches; j++)	{
				if (getline(myfile, line)) {
					patches[j] = (int*)malloc(N_PATCHES_BEZIER*sizeof(int));
					addPatches(line);
				}
				else { error_message(1); return 1; }
			}

			row = 0;
			//le vertices
			if (getline(myfile, line))
			{
				try	{ nvertices = stoi(line); }
				catch (invalid_argument) { error_message(2); return 1; }
			}
			else { error_message(2); return 1; }

			//lê vertices
			vertices = (float**)malloc(nvertices*sizeof(float*));
			for (int j = 0; j < nvertices; j++)	{
				if (getline(myfile, line)) {
					vertices[j] = (float*)malloc(N_PONTOS*sizeof(float));
					addVertices(line);
				}
				else { error_message(3); return 1; }
			}
		}
		else { error_message(5); return 1; }

		myfile.close();
		return 0;
	}

public:
	patchFile(string file)
	{
		loadFile(file);
	}

	int ** getPatches() { return patches; }
	float ** getVertices() { return vertices; }

	int get_NPatches() { return npatches; }
	int get_NVertices() { return nvertices; }

	/* USADO APENAS PARA DEBUG*/
	void printMultiArray(int ** array, int rows)
	{
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < N_PATCHES_BEZIER; k++)
			{
				printf("so the values for the array ar %d: \n", array[j][k]);
			}

		}

	}
	void printMultiArray(float ** array, int rows)
	{
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < N_PONTOS; k++)
			{
				printf("so the values for the array ar %g: \n", array[j][k]);
			}

		}

	}



/*
Dado um patch retorna uma matrix 4x12 com todos os pontos
*/
float** getPontosPatch(int* patch, float** vertices)
{
	float **pontos;

	pontos = (float**)malloc(4 * sizeof(float *));
	for (int i = 0, inc = 0, pos = 0; i < 4; i++) // para cada curva
	{
		pontos[i] = (float *)malloc(4 * 3 * (sizeof(float)));
		for (int j = 0; j < 4; j++) // para cada ponto
		{
			pontos[i][pos++] = vertices[patch[j + inc]][0]; //X
			pontos[i][pos++] = vertices[patch[j + inc]][1]; //Y
			pontos[i][pos++] = vertices[patch[j + inc]][2]; //Z
		}
		inc += 4; pos = 0;
	}
	return pontos;
}

float length(float x, float y, float z) {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

/*
Dado uma matrix 4x12 com todos os pontos de controle e um determinado ponto de tesselação retorna um array com as coordenadas do ponto
p-> matriz de pontos de controle - Matriz 4x12 contem todas as coordenadas dos 4 pontos
u e v -> ponto de tesselação [0..1]
res variavel onde vao ser guardados as coordenadas do ponto calculado
*/
vertex getBezierPointSurface(float **p, float u, float v) {

	// beziermatrix
	float m[4][4] = { { -1.0f, 3.0f, -3.0f, 1.0f },
	{ 3.0f, -6.0f, 3.0f, 0.0f },
	{ -3.0f, 3.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f, 0.0f } };

	float res[3];
	float norm[3];
	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
	norm[0] = 0.0; norm[1] = 0.0; norm[2] = 0.0;

	
	// Calcular o ponto res = T * M * P

	float u3[1][4] = { u*u*u, u*u, u, 1 };
	float v3[1][4] = { v*v*v, v*v, v, 1 };

	float u3T[4][1] = { { u*u*u }, { u*u }, { u }, { 1 } };
	float v3T[4][1] = { { v*v*v }, { v*v }, { v }, { 1 } };


	float tmp[1][4];
	float tmp2[1][4];
	float tmp3[1][4];
	float tmp4 = 0;
	float tparcial = 0;

	for (int i = 0; i < 4; i++) // U x Mb
	{
		float aux = 0;
		for (int h = 0; h < 4; h++)  		//tmp[0][i] = u3[0][0] * m[0][i] + u3[0][1] * m[1][i] + u3[0][2] * m[2][i] + u3[0][3] * m[3][i];
			aux += u3[0][h] * m[h][i];
		tmp[0][i] = aux;

	}

	for (int j = 0; j < 3; j++)  // realiza 3 vezes para as coordenadas x, y, z
	{
		for (int i = 0, inc = 0; i < 4; i++) //U x Mb x PC (incrementa 3 para suportar matrix 12x4
		{
			float aux = 0;
			for (int h = 0; h < 4; h++)  //tmp2[0][i] = tmp[0][0] * p[0][j+inc] +tmp[0][1] * p[1][j + inc] + tmp[0][2] * p[2][j + inc] + tmp[0][3] * p[3][j + inc];
			{
				 aux += tmp[0][h] * p[h][j + inc];  
				 tmp2[0][i] = aux;
			}
			inc += 3;
		}

		for (int i = 0; i < 4; i++) //U x Mb x PC x MbT
		{
			float aux = 0;
			for (int h = 0; h < 4; h++)  //tmp3[0][i] = tmp2[0][0] * m[0][i] + tmp2[0][1] * m[1][i] + tmp2[0][2] * m[2][i] + tmp2[0][3] * m[3][i];
				aux += tmp2[0][h] * m[h][i];
			tmp3[0][i] = aux;
		}

		for (int i = 0; i < 4; i++) //U x Mb x PC x MbT x vT
		{
			tmp4 += tmp3[0][i] * v3T[i][0];
		}

		res[j] = tmp4;
		tmp4 = 0;
	}
	float norma = length(res[0], res[1], res[2]); //norma do vector

	for (int i = 0; i < 3;i++)  //calcula normais
		norm[i] = res[i] / norma;

	vertex ret(res[0], res[1], res[2], norm[0], norm[1], norm[2]);
	return ret;
}



primitiveVBO getprimitivaVBO(int tesselacao)
{
	primitiveVBO p;

	int nindices = 4;   //numero de indices adicionados por ciclo
	int incr = 0;
	int a = 0;


	float delta = 1.0f / tesselacao;  //coeficiente de tesselacao

	float u = 0, v = 0; //coordenadas(tesselacao) que vao ser calculadas
	float inc = 0;

	for (int h = 0; h < npatches; h++) {

		float **pontos = getPontosPatch(patches[h], vertices);
		for (int j = 0; j < tesselacao; j++)
		{
			for (int i = 0; i < tesselacao; i++)
			{
				incr = nindices*a;

				//PRIMEIRO TRIANGULO   P Q O
				u = i*delta;
				v = inc;
				vertex p0 = getBezierPointSurface(pontos, u, v);  // P = i*delta,inc

				u = i*delta;
				v = inc + delta;
				vertex p1 = getBezierPointSurface(pontos, u, v);  // Q = i*delta,inc+delta

				u = (i + 1)*delta;
				v = inc;
				vertex p2 = getBezierPointSurface(pontos, u, v);  // O = (i+1)*delta,inc


				//SEGUNDO TRIANGULO		O Q R

	
				u = (i + 1)*delta;
				v = delta + inc;
				vertex p3 = getBezierPointSurface(pontos, u, v); // R = (i+1)*delta, delta+inc

				p.addPonto(p0); p.addPonto(p1); p.addPonto(p2);  p.addPonto(p3); 


				p.addTriangulo(incr+0, incr+1, incr+2);
				p.addTriangulo(incr+2, incr+1, incr+3);

				a++;
			}
			inc += delta;
		}
		inc = 0;
	}
	return p;
}

};
