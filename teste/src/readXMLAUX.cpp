#include "readXMLAUX.hpp"

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
		       	}
		    }
		}

	}

	fclose(fp);

	mod->numPoints = numPoints;
	mod->vector = vector;

	return (mod);
}

void readXML(char *filename){

	FILE * fp;
	char * line = NULL;
    size_t len = 0;
    ssize_t read;

	fp = fopen(filename,"r");
	if (fp) {

		while ((read = getline(&line, &len, fp)) != -1) {
        	printf("Retrieved line of length %zu:\n", read);
        	printf("%s", line);
    	}
	}

	fclose(fp);
}

void write3d(Modelo *model){

	for(int i=0; i< model->numPoints; i++){
		printf("(%f,%f,%f)\n", model->vector[i].x,model->vector[i].y,model->vector[i].z);
	}


}