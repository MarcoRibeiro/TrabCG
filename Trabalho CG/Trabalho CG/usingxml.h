#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"
#include <fstream>
#include <iostream>
#include "classes.h"
#include "transf.h"



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
		else if (elemName == "translacao") {
			float x = 0, y = 0, z = 0;
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