#include "TinyXml\tinystr.h"
#include "TinyXml\tinyxml.h"

#include <iostream>


using namespace std;

int main(){
	TiXmlDocument doc;
	doc.LoadFile("text.xml");
	TiXmlElement* root = doc.FirstChildElement();
	const char* attr;
	string value;
	for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		
		if (elemName == "modelo")
		{
			attr = elem->Attribute("ficheiro");
//aqui guarda-se o attr para uma estrutura qq			
			cout << attr;
		}
	}
}
