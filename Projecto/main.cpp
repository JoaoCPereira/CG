#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include "engineSrcClass.h"

// variáveis globais
float scale = 1;
float angle = 0.0f;
float x_x=0,z=0;
float angleBeta = 0.0f,angleAlfa = 0.0f;
float distanciaCamera = 230.0f;

int frame=0,timefps=0,timebase=0,fps=0;

int alpha = 0, beta = -20;
float r = 2, radius=1,sensitivity = 0.0005;;

// constante da camara para deslocações
float k_X=1, k_Z=1;

// iniciar Px, Pz
float Px = 0.003,Py = 50, Pz = -230;

//iniciar lookAt
float Lx = Px+sin(alpha* 3.14 / 180.0)*cos(beta * 3.14 / 180.0), Lz = Pz+cos(alpha* 3.14 / 180.0)*cos(beta * 3.14 / 180.0), Ly = Py+sin(beta* 3.14 / 180.0);
int startX = 0, startY = 0, tracking = 0;


GLuint buffers[3];

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

void processKeys(unsigned char key, int xx, int yy) {
    float d_x = Lx-Px, d_z = Lz - Pz;

    float P_x= 0, P_z=0, L_x = 0, L_z=0;
    int max=300, min=-300; // para manter a camara dentro do terreno

    k_Z = 0;
    k_X = 0;

    switch (key) {
        case 'w' :
            k_Z = 1;
            break;
        case 's' :
            k_Z = -1;
            break;
        case 'a' :
            k_X = -1;
            break;
        case 'd' :
            k_X = 1;
            break;
    }

    P_x =Px + k_Z*d_x + k_X*(-d_z);
    P_z =Pz + k_Z*d_z + k_X*d_x;

    L_x =Lx + k_Z*d_x + k_X*(-d_z);
    L_z =Lz + k_Z*d_z + k_X*d_x;


    if ((P_x <= max) && (P_z <= max) && (P_x >= min) && (P_z >= min)){
        Px = P_x;
        Pz = P_z;

        Lx = L_x;
        Lz = L_z;
    }
}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {

        case GLUT_KEY_PAGE_DOWN:
            Py += 0.6;
            break;

        case GLUT_KEY_PAGE_UP:
            Py -= 0.6;
            break;

        case GLUT_KEY_LEFT:
            alpha++;
            break;
        case GLUT_KEY_RIGHT:
            alpha--;
            break;
        case GLUT_KEY_UP:
            beta++;
            break;
        case GLUT_KEY_DOWN:
            beta--;
            break;
    }

    //Lx = Px+sin(alpha* 3.14 / 180.0);
    //Lz = Pz+cos(alpha* 3.14 / 180.0);

    Lx = Px+sin(alpha* 3.14 / 180.0)*cos(beta * 3.14 / 180.0);
    Lz = Pz+cos(alpha* 3.14 / 180.0)*cos(beta * 3.14 / 180.0);
    Ly = Py+sin(beta* 3.14 / 180.0);   


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
    glPolygonMode(GL_FRONT,GL_FILL);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //printInfo();
// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}
