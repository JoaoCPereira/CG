#include "readXMLAUX.hpp"


int main(int argc, char* argv[]){

	if (argc==2){

		char *pFilename = argv[1];

		// abrir ficheiro xml
		TiXmlDocument doc( pFilename );
		bool loadOkay = doc.LoadFile();

		if (loadOkay){
		 
			TiXmlElement *scene = doc.RootElement();
		
			TiXmlElement *model = scene->FirstChildElement( "model" );
		
			while( model ){
				char* name3D = (char*)model->Attribute("file");
				// parse do nome
		
				Modelo *modelo=NULL;
				
				modelo = read3d(name3D);
				
			   	write3d(modelo);
		
			   	model = model->NextSiblingElement( "model" );
			}
		}

	}
    return 0;

}
