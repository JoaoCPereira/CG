#ifndef __READXMLAUX__
#define __READXMLAUX__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyxml.h"
#include "tinystr.h"
#include <GL/glut.h>


struct point;
struct modelo;

typedef struct point Point;
typedef struct modelo Modelo;

//função que vai ler de um ficheiro 3d
Modelo* read3d(char *);

void write3d(Modelo *);

void freeModel(Modelo *);


#endif