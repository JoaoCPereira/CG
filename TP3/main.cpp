#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml.h"
#include "tinystr.h"
#include "readXMLAUX.hpp"
#include <unistd.h>
#include <limits.h>

float scale = 1;
float angle = 0.0f;
float x_x=0,z=0;
float angleBeta = 0.0f,angleAlfa = 0.0f;
float distanciaCamera = 10.0f;

void cylinder0(float radius, float height, int sides) {

    int i;
    float step;

    step = 360.0/sides;

    glColor3f(1,0,0);
    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(0,height*0.5,0);
    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();

    glColor3f(0,1,0);
    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(0,-height*0.5,0);
    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_TRIANGLE_STRIP);

    for (i=0; i <= sides; i++) {
        glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
        glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
    }
    glEnd();
}

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



void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(sin(angleAlfa)*cos(angleBeta)*distanciaCamera,sin(angleBeta)*distanciaCamera,cos(angleAlfa)*(cos(angleBeta)*distanciaCamera),
            //5.0f,5.0f,5.0f,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

// put the geometric transformations here

    glTranslatef(x_x,0,z);
    glRotatef(angle,0,1,0);
    glScalef(1.0,scale,1.0);

// put drawing instructions here

    //glutWireTeapot(2.0);

    glBegin(GL_TRIANGLES);
/*
    glColor3f(1,0,0);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);


    glColor3f(0,1,0);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0,0,1);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(0,1,1);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    glColor3f(1,1,0);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    */


    /*
    float slices = 30;
    float alfa = 2*M_PI / slices;
    float h = 1;
    float r = 1;

    for(int i=0;i<slices;i++){
        // criação da base
        glColor3f(1,1,0);
        glVertex3f(sin(alfa*i)*r,-h/2,cos(alfa*i)*r);
        glVertex3f(0,-h/2,0);
        glVertex3f(sin(alfa*(i+1))*r,-h/2,cos(alfa*(i+1))*r);

        // criação da topo
        glColor3f(0,1,0);
        glVertex3f(sin(alfa*i)*r,h/2,cos(alfa*i)*r);
        glVertex3f(sin(alfa*(i+1))*r,h/2,cos(alfa*(i+1))*r);
        glVertex3f(0,h/2,0);

        glColor3f(1-0.05*i,1,1);
        glVertex3f(sin(alfa*i)*r,-h/2,cos(alfa*i)*r);
        glVertex3f(sin(alfa*(i+1))*r,-h/2,cos(alfa*(i+1))*r);
        glVertex3f(sin(alfa*(i+1))*r,h/2,cos(alfa*(i+1))*r);

        glColor3f(1,0,1-0.05*i);
        glVertex3f(sin(alfa*(i+1))*r,h/2,cos(alfa*(i+1))*r);
        glVertex3f(sin(alfa*i)*r,h/2,cos(alfa*i)*r);
        glVertex3f(sin(alfa*i)*r,-h/2,cos(alfa*i)*r);

    }
*/

    char *pFilename = "demo.xml";

    // abrir ficheiro xml
    TiXmlDocument doc( pFilename );
    bool loadOkay = doc.LoadFile();

    if (loadOkay) {
        TiXmlElement *l_pRootElement = doc.RootElement();

        TiXmlElement *model = l_pRootElement->FirstChildElement("model");

        while (model) {
            // l_pForename->Attribute("file");
            char *name3D = (char *) model->Attribute("file");
            // parse do nome
            name3D = name3D + 3;
            name3D[strlen(name3D) - 3] = '\0';

            Modelo *modelo = NULL;

            modelo = read3d(name3D);
            write3d(modelo);

            model = model->NextSiblingElement("model");
        }
    }


    glEnd();

    // End of frame
    glutSwapBuffers();
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
            if (angleBeta < (M_PI/2)) {
                angleBeta += 0.1f;
            }
            break;
        case GLUT_KEY_DOWN :
            if (angleBeta > (-M_PI/2)) {
                angleBeta -= 0.1f;
            }
            break;
        case GLUT_KEY_LEFT :
            if (angleAlfa > (-M_PI/2)) {
                angleAlfa -= 0.1f;
            }
            break;
        case GLUT_KEY_RIGHT :
            if (angleAlfa < (M_PI/2)) {
                angleAlfa += 0.1f;
            }
            break;
    }

    glutPostRedisplay();
}

void processKeys(unsigned char key, int x, int y) {

    switch (key) {
        case 'a' :
            x_x -= 1;
            break;
        case 'd' :
            x_x += 1;
            break;
        case 's' :
            z -= 1;
            break;
        case 'w' :
            z += 1;
            break;
        case 'r' :
            distanciaCamera += 1;
            break;
        case 'f' :
            distanciaCamera -=1;
            break;
    }

    glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    //glutTimerFunc(0,timer,0);


// put here the registration of the keyboard callbacks



//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT,GL_LINE);

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
