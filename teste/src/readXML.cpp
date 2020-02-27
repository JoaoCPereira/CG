#include "readXMLAUX.hpp"


int main(int argc, char* argv[]){

	if (argc==2){

		char *pFilename = argv[1];

		// abrir ficheiro xml
		TiXmlDocument doc( pFilename );
		doc.LoadFile();
 
		TiXmlElement *scene = doc.RootElement();

		TiXmlElement *model = scene->FirstChildElement( "model" );

		while( model ){
			char* name3D = (char*)model->Attribute("file");
			// parse do nome

			name3D = name3D+3;
			name3D[strlen(name3D)-3] = '\0';

			Modelo *modelo=NULL;
		
			modelo = read3d(name3D);
		
	    	write3d(modelo);

	    	model = model->NextSiblingElement( "model" );
		}

	}
    return 0;

}
