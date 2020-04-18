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

#include <vector>
#include <iostream>
#include<bits/stdc++.h> 
using namespace std; 


typedef struct point{
	float x;
	float y;
	float z;
}*Ponto;

//typedef struct point Ponto;

struct modelo
{
	float diffR;
	float diffG;
	float diffB;
	int numPoints;
	int posInitVBO;
};

struct geo_transf{
	int tipo; // int para o tipo (0-translate, 2-rotate, 3-scale)
	vector<Ponto> control_points;
	float time;
	float x;
	float y;
	float z;
	float angle;
};

typedef struct modelo Modelo;
typedef struct geo_transf Geo_Transf;

void writeModelo3D(Modelo *);

void writeGeoTransf(Geo_Transf *);

#endif