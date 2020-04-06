

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
#include <random>
#endif

using namespace std;

unsigned int t,tw,th;
unsigned char*imageData;
float escala = 0, sensitivity = 0.003;

int alpha = 0, beta = 0, r = 3;
float camX = 1, camY = 1, camZ = 1, radius=1, k_X=1, k_Z=1,Px = camX*r, Pz = camY*r, Lx = Px+sin(alpha*3.14 / 180), Lz = Pz+cos(alpha*3.14 / 180);
int startX = 0, startY = 0, tracking = 0;

int frame = 0, timefps,timebase = 0,fps = 0, slices = 10;
char s[64];

int n_arvores = 300;
float timeTest = 0;

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

float height(int x, int z){
	//cout << (float)imageData[z*256+x]*escala;
	return (float)imageData[(z*256)+x]*escala;
	//return vertexB[z*6*256+x*6+1];
}

float random_height(float px,float pz){
	int x1 = floor(px); int z1 = floor(pz);
	int x2 = x1+1; int z2 = z1+1;
	float fz = pz-z1; float fx = px-x1;

	float h_x1_z = height(x1,z1) * (1-fz) + height(x1,z2) * fz;
	float h_x2_z = height(x2,z1) * (1-fz) + height(x2,z2) * fz;

	float h_xz = h_x1_z * (1-fx) + h_x2_z * fx;

	//cout << "h_xz " << h_xz << "; h_x1_z " << h_x1_z <<  "; (1-fx) " << (1-fx) << "; h_x2_z " << h_x2_z << "; fx " << fx<< endl;

	return h_xz;
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
	float LO = -128.0;
	float HI = 127.0;

	for(int i = 0;i<n_arvores;i++){

		float x = ((float)rand() / RAND_MAX) * (HI - LO) + LO;
		float z = ((float)rand() / RAND_MAX) * (HI - LO) + LO;
		//float x = LO + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));
		//float z = LO + static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(HI-LO));

		if(sqrt(pow(x,2)+pow(z,2)) > 50){

			Coord coords = (struct coord*) malloc(sizeof(struct coord));

			coords->x = x;
			coords->y = random_height(x+HI+1,z+HI+1);
			coords->z = z;

			coord_trees.push_back(coords);
		}
		else i--;
	}
}
void vectorD(void){

	for (int i = 0; i < coord_trees.size(); ++i){

		glPushMatrix();
		
		glTranslatef(coord_trees[i]->x,coord_trees[i]->y,coord_trees[i]->z);
		tree();
		
		glPopMatrix();

	}

}


void drawTerrain() {

    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS


	// duvida powerPoint diz para para colocar no render vai ser executado varias vezes
	// no pdf diz para executar uma única vez

	for (int i = 0; i < th-1 ; i++) {
		glColor3f(0.5,0.5,0.5);
		glDrawArrays(GL_TRIANGLE_STRIP, tw*2*i , tw*2);
	}
}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	/*
	gluLookAt(camX*radius, camY*radius, camZ*radius, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	*/

	//Px = camX*r;
	//Pz = camZ*r;

	if (!camX) Px=r;
	if (!camZ) Pz=r;

	if (Px <= -128) Px = -128;
	else if (Px >= 126) Px = 126;

	if (Pz <= -126) Pz = -126;
	else if (Pz >= 126) Pz = 126;

	float Py = random_height(Px+128,Pz+128) + 5;
	
	gluLookAt(	Px, Py, Pz, 
		      	Lx,Py,Lz,
				//0.0f, Py, 0.0f,
			  	0.0f,1.0f,0.0f);
	

	drawTerrain();

	glPushMatrix();
	/*
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	*/

	glColor3f(1, 0, 1);

	glPushMatrix();

	glTranslatef(0,random_height(128,128),0);

	glutSolidTorus(2, 4, 20, 20);

	glPopMatrix();

	glTranslatef(0,1.5,0);

	//cilo interno
	// 8 teapot
	float angleTea = 360 / 8;

	glPushMatrix();

	glRotatef(timeTest, 0, 1, 0);

	glColor3f(0,0,1);

	int rI = 15; //raio da circunferencia dos teapots internos

	float y=0.0f, z=0.0f, yY=0.0f, zZ=0.0f;

	//cout << "-------------------" << endl;

	for(int i=0;i<8;i++){

		glPushMatrix();

		//aplicar a rotacao na origem
		glRotatef(angleTea*i, 0, 1, 0); // ang in degrees
		glTranslatef(0, 0, rI);

		// translate da altura

		y = sin( (angleTea*i* 3.14 / 180.0))*rI;
		z = cos( (angleTea*i* 3.14 / 180.0))*rI;

		//cout << i << " - "<< "yY =" << y << " zZ =" << z << endl;

		yY = y*cos((-timeTest* 3.14 / 180.0) )-z*sin((-timeTest* 3.14 / 180.0) );
		zZ = y*sin((-timeTest* 3.14 / 180.0) )+z*cos((-timeTest* 3.14 / 180.0) );

		//cout << i << " - " << "yY =" << yY+128 << " zZ =" << zZ+128 << endl;

		glTranslatef(0,random_height(yY+128,zZ+128),0);

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

	int rE = 35; //raio da circunferencia dos teapots externos

	for(int i=0;i<16;i++){

		glPushMatrix();

		//aplicar a rotacao na origem
		glRotatef(angleTea*i, 0, 1, 0); // ang in degrees
		glTranslatef(0, 0, rE);

		y = sin( (angleTea*i* 3.14 / 180.0))*rE;
		z = cos( (angleTea*i* 3.14 / 180.0))*rE;

		//cout << i << " - "<< "yY =" << y << " zZ =" << z << endl;

		yY = y*cos((timeTest* 3.14 / 180.0) )-z*sin((timeTest* 3.14 / 180.0) );
		zZ = y*sin((timeTest* 3.14 / 180.0) )+z*cos((timeTest* 3.14 / 180.0) );

		glTranslatef(0,random_height(yY+128,zZ+128),0);

		glutSolidTeapot(2);

		glPopMatrix();
		//voltar a origem
	}

	glPopMatrix();

	glPopMatrix();

	vectorD();
	// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

 switch (key) {
        case '1' :
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case '2' :
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
    }
}


void processSpecialKeys(int key, int xx, int yy) {

	float d_x = Lx-Px, d_z = Lz - Pz;

	switch (key) {

		case GLUT_KEY_PAGE_DOWN: r -= 0.1f;
			if (r < 0.1f)
				r = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: r += 0.1f; break;

		case GLUT_KEY_UP:
			//camZ -= 1;
			//if (camZ < -128) camZ = -128;
			k_Z = 1;
			Px += k_Z*d_x;
			Pz += k_Z*d_z;
			Lx += k_Z*d_x;
			Lz += k_Z*d_z;
			break;
		case GLUT_KEY_DOWN:
			//camZ += 1;
		 	//if (camZ > 128) camZ=128;
			k_Z = -1;
			Px += k_Z*d_x;
			Pz += k_Z*d_z;
			Lx += k_Z*d_x;
			Lz += k_Z*d_z;
			break;
		case GLUT_KEY_LEFT:
			//timeTest -= 0.5;
			//camX -= 1;
		 	//if (camX < -128) camX =-128;
			k_X = -1;
			Px += k_X*(-d_z);
			Pz += k_X*d_x;
			Lx += k_X*(-d_z);
			Lz += k_X*d_x;
			break;
		case GLUT_KEY_RIGHT:
			//timeTest += 0.5;
			//camX += 1;
			//if (camX > 128) camX =128;
			k_X = 1;
			Px += k_X*(-d_z);
			Pz += k_X*d_x;
			Lx += k_X*(-d_z);
			Lz += k_X*d_x;
			break;
	}

	/*
	A x B = (a2b3  -   a3b2,     a3b1   -   a1b3,     a1b2   -   a2b1)
	distX.push_back(d_x); distX.push_back(0);distX.push_back(d_z);
	up.push_back(0);up.push_back(1);up.push_back(0);
	r = (-d_z,0,d_x)
	*/

	glutPostRedisplay();

}


void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		//cout << "Buttons" << " xx " << xx << " yy " << yy << endl;
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}/*
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
	}*/
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;
	deltaX = (xx - startX)*sensitivity;
	deltaY = (yy - startY)*sensitivity;

	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta - deltaY;

		//cout << "Motion" << " xx " << xx << " yy " << yy << endl;
		//cout << "Motion" << " Startxx " << startX << " Startyy " << startY << endl;
		//cout << "Delta" << " xx " << deltaX << " yy " << deltaY << endl;
	
		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY*sensitivity;

		//cout << "rAux " << rAux << " r " << r << " deltaY " << deltaY << endl;

		if (rAux < 3){
			rAux = 3;
		}
		else if (rAux > 200){
			rAux = 200;
		}
	}

	alpha = alphaAux;
	beta = betaAux;
	//r = rAux;

	Px = (Px/r)*rAux;
	Pz = (Pz/r)*rAux;

	Lx = Px+sin(alpha);
	Lz = Pz+cos(alpha);
	//cout << r << endl;
	//camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	//camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	//camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
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

    escala= alturaMax/tw;

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
	timeTest += 0.5; // para a animação
	glutPostRedisplay();
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280,1280);
	glutCreateWindow("CG@DI-UM");

	ilInit();
	glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);


	

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

	vectorF(); // iniciamos depois de init() porque precisamos de valores da init, imageData


// enter GLUT's main cycle
	glutMainLoop();

	
	return 0;
}

