#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml.h"
#include "tinystr.h"
#include "engineSrcClass.h"
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <stdio.h>
#include<bits/stdc++.h> 

using namespace std; 

// variáveis globais
float scale = 1;
float angle = 0.0f;
float x_x=0,z=0;
float angleBeta = 0.0f,angleAlfa = 0.0f;
float distanciaCamera = 230.0f;

int frame=0,timefps=0,timebase=0,fps=0;

GLuint buffers;

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}


// write function to process keyboard events

void processSpecialKeys(int key, int xx, int yy) {
    // xx e yy coordenadas do rato
    // não podemos ter translacoes e rotacoes (so mudar variaveis)

    switch (key) {
        case GLUT_KEY_F1 :
            scale += 1.0f;
            break;
        case GLUT_KEY_F2 :
            scale -= 1.0f;
            break;
        case GLUT_KEY_F3 :
            angle += 1.0f;
            break;
        case GLUT_KEY_F4 :
            angle -= 1.0f;
            break;
        case GLUT_KEY_UP :
            if (angleBeta < (M_PI/2)-0.1) {
                angleBeta += 0.1f;
            }
            break;
        case GLUT_KEY_DOWN :
            if (angleBeta > (-M_PI/2)+0.1) {
                angleBeta -= 0.1f;
            }
            break;
        case GLUT_KEY_LEFT :
            angleAlfa -= 0.1f;
            break;
        case GLUT_KEY_RIGHT :
            angleAlfa += 0.1f;
            break;
    }

    glutPostRedisplay();
}

void processKeys(unsigned char key, int x, int y) {

    switch (key) {
        case 'r' :
            distanciaCamera += 1;
            break;
        case 'f' :
            distanciaCamera -=1;
            break;
        case '1' :
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case '2' :
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
    }

    glutPostRedisplay();

}

void fpsshow(void){
    frame++;
    timefps=glutGet(GLUT_ELAPSED_TIME);
    if (timefps - timebase > 1000) {
        fps = frame*1000.0/(timefps-timebase);
        timebase = timefps;
        frame = 0;
    }
    char s[64];
    sprintf(s,"fps %d",fps);

    glutSetWindowTitle(s);

    glutPostRedisplay();
}

int main(int argc, char **argv) {
    char *pFilename = NULL;


    if (argc >= 2){
        pFilename = argv[1];
    }
    else { 
        printf("Não foi introduzido o nome do ficheiro xml\n");
        return 1;
    }

    //criar modelos.size() buffers

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("CG@DI-UM");

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);
    
    //readXML(pFilename,modelos,geo_tr,sequencia);
    SysState system (pFilename);

// Required callback registry
    glutDisplayFunc(system.renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(fpsshow);

// put here the registration of the keyboard callbacks
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);



//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT,GL_LINE);

    //printInfo();
// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}
