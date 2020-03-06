#ifndef __READXMLAUX__
#define __READXMLAUX__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyxml.h"
#include "tinystr.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <vector>
#include <iostream>
#include<bits/stdc++.h> 
using namespace std; 


struct point;
struct modelo;
struct geo_transf;

typedef struct point Point;
typedef struct modelo Modelo;
typedef struct geo_transf Geo_Transf;

//função que vai ler de um ficheiro 3d
Modelo* read3d(char *);

void writeModelo3D(Modelo *);

void writeGeoTransf(Geo_Transf *);

void freeModel(Modelo *);

void readXMLaux(TiXmlElement *, vector<struct modelo*> &,vector<struct geo_transf*> &,vector<int> &);

void writeSeq(vector<struct modelo*> &,vector<struct geo_transf*> &,vector<int> &);

void readXML(char *, vector<struct modelo*> &,vector<struct geo_transf*> &,vector<int> &);

#endif