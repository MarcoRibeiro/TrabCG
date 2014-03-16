#include <iostream>
#include "classes.h"
#include <fstream>



/*
int main(){
//
	//triangle t;
	vertex x(1.0, 2.0, 3.0);
	vertex y(5.0, 3.0, 4.0);
	vertex z(1.0, 2.0, 3.0);

	triangle zt(x, y, z);
	triangle ab(z, y, x);
	ab.setColor(0.5, 0.5, 0.5);
	zt.setColor(1.0, 1.0, 1.0);

	vertex cx = zt.getP1();
	
	triangles mod;
	for (int i = 0; i < 30; i++)
	{

		mod.addTriangle(&zt);
		mod.addTriangle(&ab);
	}


	triangle* a = mod.getTriangle(0);

	/// ESCRITA


	ifstream::pos_type size = sizeof(mod);
	triangles *memblock;
	memblock = &mod;

	ofstream save1;
	save1.open("teste.3d", ios::binary);
	save1.seekp(0, ios::beg);
	save1.write((char*)&memblock, size);
	save1.close();
	


	/// LEITURA
	
	ifstream file;
	file.open("teste.3d", ios::binary);
	int beginning, end;

	beginning = file.tellg();
	file.seekg(0, ios::end);
	end = file.tellg();

	ifstream::pos_type size2 = (end - beginning);

	triangles *mem = new triangles();
	triangles aux;

	file.seekg(0, ios::beg);
	file.read((char*)&mem, size2);
	file.close();

	


	cout << a->getP1().getX();
	return 0;
}*/