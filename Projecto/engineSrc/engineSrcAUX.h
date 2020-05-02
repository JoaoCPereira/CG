#ifndef __READXMLAUX__
#define __READXMLAUX__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyxml.h"
#include "tinystr.h"
#include <unistd.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <iostream>
#include<bits/stdc++.h> 
using namespace std; 


struct point{
	float x;
	float y;
	float z;
};

typedef struct point Point;

struct modelo
{
	float diffR;
	float diffG;
	float diffB;
	int numPoints;
	int posInitVBO;
};

struct geo_transf{
	int tipo; // int para o tipo de transformação
	float x;
	float y;
	float z;
	float angle;
	float time;
};

struct translate{
	float time;
	vector<struct point*> cp;
};

typedef struct translate Translate;
typedef struct modelo Modelo;
typedef struct geo_transf Geo_Transf;

void writeModelo3D(Modelo *);

void writeGeo(Geo_Transf *);

void buildRotMatrix(float *, float *, float *, float *);
void cross(float *, float *, float *);
void normalize(float *);
float length(float *);
void multMatrixVector(float *m, float *v, float *res);
void getCatmullRomPoint(float , Point *, Point *, Point *, Point *, float *, float *);
void getGlobalCatmullRomPoint(float , float *, float *, Translate *);
void renderCatmullRomCurve(Translate *);

void writeTranslate(Translate *);

#endif