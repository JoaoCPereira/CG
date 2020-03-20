#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
int n_arvores = 300;
float timeTest = 0;


typedef struct coord{
	float x;
	float z;
}*Coord;

vector<Coord> coord_trees;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
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

void tree(void){

	glRotatef(-90, 1, 0, 0); // ang in degrees
	//tronco
	glColor3f(0.5,0.3,0.1);
	glutSolidCone(0.5,5, 10,10);

	glColor3f(0.2,0.3,0);
	glTranslatef(0,0,2);
	glutSolidCone(1.5,5, 15,10);
}


void vectorF(){
	srand(time(NULL));
	float LO = -100;
	float HI = 100;

	for(int i = 0;i<n_arvores;i++){

		float x = LO +static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));
		float z = LO +static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));

		if(sqrt(pow(x,2)+pow(z,2)) > 50){

			Coord coords = (struct coord*) malloc(sizeof(struct coord));


			coords->x = x;
			coords->z = z;

			coord_trees.push_back(coords);
		}
		else i--;
	}
}

void vectorD(void){

	for (int i = 0; i < coord_trees.size(); ++i){

		glPushMatrix();
		
		glTranslatef(coord_trees[i]->x,0,coord_trees[i]->z);
		tree();
		
		glPopMatrix();

	}

}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();


	glColor3f(1, 0, 1);
	glutSolidTorus(2, 4, 20, 20);

	glTranslatef(0,1.5,0);

	//cilo interno
	// 8 teapot
	float angleTea = 360 / 8;

	glPushMatrix();

	glRotatef(timeTest, 0, 1, 0);

	glColor3f(0,0,1);

	for(int i=0;i<8;i++){

		glPushMatrix();

		//aplicar a rotacao na origem
		glRotatef(angleTea*i, 0, 1, 0); // ang in degrees
		glTranslatef(0, 0, 15);

		glRotatef(-90, 0, 1, 0); // ang in degrees

		glutSolidTeapot(2);

		glPopMatrix();
		//voltar a origem
	}

    glPopMatrix();

	glPushMatrix();

	glRotatef(-timeTest, 0, 1, 0);

	angleTea = 360 / 16;

	glColor3f(1, 0, 0);

	for(int i=0;i<16;i++){

		glPushMatrix();

		//aplicar a rotacao na origem
		glRotatef(angleTea*i, 0, 1, 0); // ang in degrees
		glTranslatef(0, 0, 35);

		glutSolidTeapot(2);

		glPopMatrix();
		//voltar a origem
	}

	glPopMatrix();

	vectorD();
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin\n");
}

void myIdle(){
	timeTest += 0.5;
    glutPostRedisplay();
}


int main(int argc, char **argv) {

	vectorF();

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    glutIdleFunc(myIdle);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
