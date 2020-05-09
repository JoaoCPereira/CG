#include "engineSrcAUX.h"

extern GLuint buffers[3]; // variaveis globais externas do ficheiro main.cpp

float pos[4]{0}, deriv[4]{0};
float y[3] = {0,1,0};

typedef struct modelo Modelo;
typedef struct geo_transf Geo_Transf;
float scaleTime = 0.001; // passar de segundos para milissegundos
unsigned int texture;

void writeModelo3D(Modelo *model){
	// cor do objecto
	glColor3f(model->diffR,model->diffG,model->diffB);
	float emissive[]= {model->emiR,model->emiG,model->emiB,1};

	// Define a material

	float dark[] = { 0.2, 0.2, 0.2, 1.0 };
	float white[] = { 0.8, 0.8, 0.8, 1.0 };
	float red[] = { 0.8, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	glMaterialfv(GL_FRONT, GL_EMISSION, emissive);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
	glNormalPointer(GL_FLOAT,0,0);

	// desenhar com vbo com indice
	glDrawArrays(GL_TRIANGLES, model->posInitVBO, model->numPoints);
}

void writeGeo(Geo_Transf *transf){

	if(transf){
		switch (transf->tipo) {
			case 1: // rotate
				if (transf->angle) glRotatef(transf->angle,transf->x,transf->y,transf->z);
				else {
					glRotatef(((2*M_PI)/transf->time)*glutGet(GLUT_ELAPSED_TIME)*0.01,transf->x,transf->y,transf->z);
				}
				break;

			case 2: // scale
				glScalef(transf->x,transf->y,transf->z);
				break;
		}
	}
}


void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}


void cross(float *a, float *b, float *res) {

	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}


void normalize(float *a) {

	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}


float length(float *v) {
	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

void getCatmullRomPoint(float t, Point *p0, Point *p1, Point *p2, Point *p3, float *pos, float *deriv) {

	float A[4], P[4];

	// catmull-rom matrix
	float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f}};
			

	for(int i = 0; i < 4; i++){
		if(i==0){
			P[0] = p0->x; P[1] = p1->x ; P[2] = p2->x; P[3] = p3->x;
		}
		if(i==1){
			P[0] = p0->y; P[1] = p1->y ; P[2] = p2->y; P[3] = p3->y;
		}
		if(i==2){
			P[0] = p0->z; P[1] = p1->z ; P[2] = p2->z; P[3] = p3->z;
		}
		if(i==3){
			P[0] = 1; P[1] = 1 ; P[2] = 1; P[3] = 1;
		}

		// Compute A = M * P
		multMatrixVector(*m,P,A);

		// Compute pos = T * A
		pos[i] = t*t*t*A[0] + t*t*A[1] + t*A[2] + A[3];

		// Compute deriv = T' * A
		deriv[i] = 3*t*t*A[0] + 2*t*A[1] + A[2];
	}
	
}

// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, Translate *translate) {

	int POINT_COUNT = translate->cp.size();

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + POINT_COUNT-1)%POINT_COUNT;	
	indices[1] = (indices[0]+1)%POINT_COUNT;
	indices[2] = (indices[1]+1)%POINT_COUNT; 
	indices[3] = (indices[2]+1)%POINT_COUNT;

	getCatmullRomPoint(t, translate->cp[indices[0]], translate->cp[indices[1]], translate->cp[indices[2]], translate->cp[indices[3]], pos, deriv);
}

void renderCatmullRomCurve(Translate *translate) {
	float gt = 0;
	glColor3f(1,1,1);
	glBegin(GL_LINE_LOOP);
	for(; gt<=1; gt+=0.01){
		getGlobalCatmullRomPoint(gt,pos,deriv,translate);
		for(int i=0;i<4;++i){
			glVertex3f(pos[0],pos[1],pos[2]);
		}
	}
	glEnd();
}

void writeTranslate(Translate *translate) {
	float tempo = ((glutGet(GLUT_ELAPSED_TIME)*scaleTime)*translate->cp.size())/translate->time;

	float X[3];
	float Y[3];
	float Z[3];

	renderCatmullRomCurve(translate);

	// apply transformations here
	getGlobalCatmullRomPoint(tempo,pos,deriv, translate);

	glTranslatef(pos[0],pos[1],pos[2]);
	
	X[0] = deriv[0];X[1] = deriv[1];X[2] = deriv[2]; // X = P'(t)
	
	cross(X,y,Z); // Z = X x Y anterior
	normalize(Z); normalize(X);


	cross(Z,X,Y); // Y = Z cross X
	y[0]=Y[0]; y[1] = Y[1]; y[2] = Y[2]; // guardamos o novo y para usar na proxima invocação

	normalize(Y);
	
	float rot_matrix[16];

	buildRotMatrix(X,Y,Z,rot_matrix);

	glMultMatrixf(rot_matrix);
}

void writeLigth(Light *light, int numberLight){

	// falta ver o caso DIRECTIONAL (1)
	if(light->type==1){
		float pos[4] = {light->point->x, light->point->y, light->point->z, 0.0};
	}
	else float pos[4] = {light->point->x, light->point->y, light->point->z, 1.0};

	//cout << light->point->x << " " << light->point->y << " " << light->point->z << endl;

	glLightfv(GL_LIGHT0,GL_POSITION, pos);
}


void loadTexture(char *file){
	unsigned int t, tw, th;
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)file);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

}