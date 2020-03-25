

#include<stdio.h>
#include <iostream>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

unsigned int t,tw,th;
unsigned char*imageData;

float camX = 00, camY = 30, camZ = 40, radius=5;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

int imageHeight=256,imageWidth=256;

int frame = 0, timefps,timebase = 0,fps = 0, slices = 10;
char s[64];

vector <float> vertexB;

GLuint buffers[1];

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS


	// duvida powerPoint diz para para colocar no render vai ser executado varias vezes
	// no pdf diz para executar uma única vez

    glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);


	for (int i = 0; i < 255 ; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, 512*i , 512);
	}
}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX*radius, camY*radius, camZ*radius, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawTerrain();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	glutPostRedisplay();

}


void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

void init() {
	// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);
	
	ilLoadImage("terreno.jpg");
	ilConvertImage(IL_LUMINANCE,IL_UNSIGNED_BYTE);

	tw=ilGetInteger(IL_IMAGE_WIDTH);
	th=ilGetInteger(IL_IMAGE_HEIGHT);

	if (tw != 256 && th != 256) return;
	cout << tw << ' ' << th << endl;

	imageData = ilGetData();
	
	

// 	Build the vertex arrays

	vertexB.clear();

    int min=-127.5;

    // linha
    for (int z = 0; z < 255; z++) {
    	// clonua
    	for (int x = 0; x < 256; x++) {
    		// para cada i j 2 pontos
    		vertexB.push_back(min+(1*x)); // X
			vertexB.push_back( (float)imageData[(z*256)+x]*0.117 ); // Y
			//printf("%f -- %d -- %d -- (%d,%d)\n",(float)imageData[(z*256)+x]*0.117,imageData[(z*256)+x],(z*256)+x,x,z);
			vertexB.push_back(min+(1*z)); // Z

			vertexB.push_back(min+(1*x)); // X
			vertexB.push_back( (float)imageData[((z+1)*256)+x]*0.117 ); // Y 
			vertexB.push_back(min+(1*z)+1); // Z
		}
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER,vertexB.size()*sizeof(float), &vertexB[0], GL_STATIC_DRAW);


// 	OpenGL settings
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT,GL_LINE);
}

void fpsshow(void){
	frame++;
	timefps=glutGet(GLUT_ELAPSED_TIME);
	if (timefps - timebase > 1000) {
		fps = frame*1000.0/(timefps-timebase);
		timebase = timefps;
		frame = 0;
	}
	
	sprintf(s,"%d",fps);
	glutSetWindowTitle(s);

	glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");

	ilInit();
	glewInit();

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(fpsshow);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutSpecialFunc(processSpecialKeys);

	init();	

// enter GLUT's main cycle
	glutMainLoop();

	
	return 0;
}

