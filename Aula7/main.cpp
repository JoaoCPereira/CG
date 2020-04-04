

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

float camX = 00, camY = 30, camZ = 40, radius=1;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

int frame = 0, timefps,timebase = 0,fps = 0, slices = 10;
char s[64];

int n_arvores = 300;
int timeTest = 0;

vector <float> vertexB;

typedef struct coord{
	float x;
	float y;
	float z;
}*Coord;

vector<Coord> coord_trees;

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
	float LO = -128;
	float HI = 128;

	for(int i = 0;i<n_arvores;i++){

		float x = LO +static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));
		float z = LO +static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));

		if(sqrt(pow(x,2)+pow(z,2)) > 50){

			Coord coords = (struct coord*) malloc(sizeof(struct coord));


			coords->x = x;
			coords->y = (float)imageData[(z*256)+x]*1.0;
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


void drawTerrain() {

    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS


	// duvida powerPoint diz para para colocar no render vai ser executado varias vezes
	// no pdf diz para executar uma única vez

	for (int i = 0; i < th-1 ; i++) {
		glColor3f(1,1,1);
		glDrawArrays(GL_TRIANGLE_STRIP, tw*2*i , tw*2);
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

	//glPushMatrix();

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

    //glPopMatrix();

	//glPushMatrix();

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

	//glPopMatrix();

	vectorD();
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

	if (tw != 256 && th != 256){
    	cout << "The image could not be loaded" << endl;
        return;
    }
    
	imageData = ilGetData();
	
// 	Build the vertex arrays

	vertexB.clear();

    float min=-(((float)(tw-1))/2.0), alturaMax=30;

    float escala= alturaMax/tw;

    // linha
    for (int z = 0; z < th-1; z++) {
    	// coluna
    	for (int x = 0; x < tw; x++) {
    		// para cada i j 2 pontos
    		vertexB.push_back(min+(1*x)); // X
			vertexB.push_back( (float)imageData[(z*tw)+x]*escala ); // Y
			vertexB.push_back(min+(1*z)); // Z

			vertexB.push_back(min+(1*x)); // X
			vertexB.push_back( (float)imageData[((z+1)*tw)+x]*escala ); // Y 
			vertexB.push_back(min+(1*z)+1); // Z
		}
    }

    

    glGenBuffers(1, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER,vertexB.size()*sizeof(float), &vertexB[0], GL_STATIC_DRAW);
    glVertexPointer(3,GL_FLOAT,0,0);

// 	OpenGL settings
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT,GL_LINE);
	// GL_FILL e GL_LINE
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


void myIdle(){
	timeTest += 0.5;
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
    glEnableClientState(GL_VERTEX_ARRAY);

	vectorF();

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(myIdle);

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

