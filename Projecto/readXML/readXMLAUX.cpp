#include "readXMLAUX.h"

extern GLuint buffers; // variaveis globais externas do ficheiro main.cpp

typedef struct modelo Modelo;
typedef struct geo_transf Geo_Transf;

void writeModelo3D(Modelo *model){

	// desenhar com vbo com indece
	glColor3f(model->diffR,model->diffG,model->diffB);
	//printf("posInicial = %d, numPontos = %d\n", model->posInitVBO, model->numPoints);

	glDrawArrays(GL_TRIANGLES, model->posInitVBO, model->numPoints);

	/*
	if (model){
		glColor3f(model->diffR,model->diffG,model->diffB);
		glBegin(GL_TRIANGLES);
		for(int i=0; i< model->numPoints; i++){
			glVertex3f(model->vector[i].x,model->vector[i].y,model->vector[i].z);
		}
		glEnd();
	}
	*/
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