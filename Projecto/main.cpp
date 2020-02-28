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
#include <iostream>
#include <vector>

using namespace std; 

// variáveis globais
float scale = 1;
float angle = 0.0f;
float x_x=0,z=0;
float angleBeta = 0.0f,angleAlfa = 0.0f;
float distanciaCamera = 10.0f;

vector<struct modelo*> modelos; 

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
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

// put the geometric transformations here

    glTranslatef(x_x,0,z);
    glRotatef(angle,0,1,0);
    glScalef(1.0,scale,1.0);

// put drawing instructions here

    glBegin(GL_TRIANGLES);

    for (int i=0; i< modelos.size(); i++){
        write3d(modelos[i]);
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
        /*case 'a' :
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
*/
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


int main(int argc, char **argv) {

    char *pFilename = (char*) "demo.xml";

    // abrir ficheiro xml
    TiXmlDocument doc( pFilename );
    bool loadOkay = doc.LoadFile();

    if (loadOkay) {
        TiXmlElement *l_pRootElement = doc.RootElement();

        TiXmlElement *model = l_pRootElement->FirstChildElement("model");

        while (model) {
            char *name3D = (char *) model->Attribute("file");

            Modelo *modelo = NULL;
            modelo = read3d(name3D);

            modelos.push_back(modelo);

            model = model->NextSiblingElement("model");
        }
    }

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@DI-UM");

// Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

// put here the registration of the keyboard callbacks
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);



//  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT,GL_LINE);

// enter GLUT's main cycle
    glutMainLoop();

    return 1;
}
