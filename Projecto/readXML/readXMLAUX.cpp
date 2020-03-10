#include "readXMLAUX.h"

struct point
{
	float x;
	float y;
	float z;

};

struct modelo
{
	int numPoints;
	Point *vector;
};

struct geo_transf{
	int tipo; // int para o tipo (0-translate, 2-rotate, 3-scale)
	float x;
	float y;
	float z;
	float angle;
};

Modelo* read3d(char *filename){

	Modelo *mod =(struct modelo*) malloc(sizeof(struct modelo));

	Point *vector=NULL;

	int counterP = 0;
	int numPoints = 0;
	int validArgs = 0;

	bool erro = false;

	FILE * fp;
	char * line = NULL;
    size_t len = 0;
    ssize_t read;

	fp = fopen(filename,"r");
	if (fp) {

		// parse da primeira linha que contem os numeros dos pontos
		if ((read = getline(&line, &len, fp)) != -1) { 
			validArgs = sscanf(line, "%d", &numPoints);

			if (validArgs!=1){
				printf("Number of lines not defined!!\n");
				erro = true;
			}
			free(line);
			line=NULL;
		}

		if (!erro){
			// criar um vector de n pontos
			vector = (struct point*) calloc(numPoints,sizeof(struct point));
		
			while ((counterP < numPoints) && (read = getline(&line, &len, fp)) != -1) {
		       	validArgs = sscanf(line,"%f %f %f",&vector[counterP].x,&vector[counterP].y,&vector[counterP].z);

		       	if (validArgs!=3) {
		       		printf("Invalid point!! on line: %d (of %d)\n",counterP+1, numPoints+1);
		       		erro = true;
		       		break;
		       	}
		       	else {
		       		++counterP;
		       		free(line);
					line=NULL;
		       	}
		    }
		}

	}

	fclose(fp);

	mod->numPoints = numPoints;
	mod->vector = vector;

	return (mod);
}

void writeModelo3D(Modelo *model){

	if (model){
		glBegin(GL_TRIANGLES);
		for(int i=0; i< model->numPoints; i++){
			glVertex3f(model->vector[i].x,model->vector[i].y,model->vector[i].z);
		}
		glEnd();
	}
}

void writeGeoTransf(Geo_Transf *transf){

	if(transf){
		switch (transf->tipo) {
			case 0: // translate
				glTranslatef(transf->x,transf->y,transf->z);
				break;
			case 1: // rotate
				glRotatef(transf->angle,transf->x,transf->y,transf->z);
				break;
			case 2: // scale
				glScalef(transf->x,transf->y,transf->z);
				break;
		}
	}
}

void freeModel(Modelo *model){

	if (model){	
		free(model->vector);
		model->vector=NULL;
		free(model);
	}

}

void readXMLaux(TiXmlElement *element,vector<struct modelo*> &modelos,vector<struct geo_transf*> &geo_tran,vector<int> &sequencia){

		//if para verificar que filho é
        //primeiro verificar se n é nulo
        while (element){

        	//caso model
        	if(!strcmp(element->Value(), "model")){

                char *name3D = (char *) element->Attribute("file");
        
                Modelo *modelo = NULL;
                modelo = read3d(name3D);
        
                modelos.push_back(modelo);

                //adicinar 3(model ao array de sequencia)
                sequencia.push_back(3);
            }

        	//caso translate
        	if(!strcmp(element->Value(), "translate")){
        		Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
        		geo->x=0;
        		geo->y=0;
        		geo->z=0;

        		geo->tipo = 0;

        		element->QueryFloatAttribute("X",&geo->x);
        		element->QueryFloatAttribute("Y",&geo->y);
        		element->QueryFloatAttribute("Z",&geo->z);

        		geo_tran.push_back(geo);

        		sequencia.push_back(2);
        	}

        	//caso rotate
        	if(!strcmp(element->Value(), "rotate")){
        		Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
        		geo->x=0;
        		geo->y=0;
        		geo->z=0;

        		geo->tipo = 1;

        		element->QueryFloatAttribute("angle",&geo->angle);
        		element->QueryFloatAttribute("axisX",&geo->x);
        		element->QueryFloatAttribute("axisY",&geo->y);
        		element->QueryFloatAttribute("axisZ",&geo->z);

        		geo_tran.push_back(geo);

        		sequencia.push_back(2);
        	}

        	//caso scale
        	if(!strcmp(element->Value(), "scale")){

        		Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
        		geo->x=0;
        		geo->y=0;
        		geo->z=0;

        		geo->tipo = 2;

        		element->QueryFloatAttribute("X",&geo->x);
        		element->QueryFloatAttribute("Y",&geo->y);
        		element->QueryFloatAttribute("Z",&geo->z);

        		geo_tran.push_back(geo);

        		sequencia.push_back(2);
        	}

        	//caso group
        	if(!strcmp(element->Value(), "group")){
        		sequencia.push_back(0);

        		readXMLaux(element->FirstChildElement(),modelos,geo_tran,sequencia);

        		sequencia.push_back(1);
        	}

        	//pode n estar correto 
        	if(!strcmp(element->Value(), "models")){

        		readXMLaux(element->FirstChildElement(),modelos,geo_tran,sequencia);

        	}

        	//proximo filho

        	//element != nullptr;

        	if(element){
        	    element = element->NextSiblingElement();
        	}
        }
}

void writeSeq(vector<struct modelo*> &modelos,vector<struct geo_transf*> &geo_tran,vector<int> &sequencia){
	int numGeo = 0;
	int numModelo = 0;

	//percorrer vector sequencia
	for(int i=0;i<sequencia.size();i++){
		//verificar caso
		switch (sequencia[i]) {
			case 0:
				glPushMatrix();
				break;
			case 1:
				glPopMatrix();
				break;
			case 2:
				if(numGeo<geo_tran.size()) writeGeoTransf(geo_tran[numGeo++]);
				break;
			case 3:
				if(numModelo<modelos.size()) writeModelo3D(modelos[numModelo++]);
				break;
		}
	}


}


void readXML(char *pFilename,vector<struct modelo*> &modelos,vector<struct geo_transf*> &geo_tran,vector<int> &sequencia){
// abrir ficheiro xml
    TiXmlDocument doc( pFilename );
    bool loadOkay = doc.LoadFile();

    if (loadOkay) {
        TiXmlElement *l_pRootElement = doc.RootElement();

        TiXmlElement *element = l_pRootElement->FirstChildElement("group");

        if (element){
       	    readXMLaux(element->FirstChildElement(),modelos,geo_tran,sequencia);
        }

        //procuramos o proximo filho 
        element = element->FirstChildElement();  
    }
}
