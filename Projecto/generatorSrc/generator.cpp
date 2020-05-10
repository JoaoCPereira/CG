#include "generator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

vector< vector<int> > patches;
vector<int> patch;

vector<struct point*> points;
vector<struct point*> vectorFinal;


void print_sphere(float radius,float slices,float stacks,char* file_name){
    vector<string> normals;
    char str1[100];
    ofstream myfile;
    myfile.open (file_name);

    if (myfile.is_open()){
        float alfa = 2*M_PI / (slices*4);
        float beta = (M_PI/2) / stacks;

        int numT = (int)(slices*4*6+(stacks-1)*slices*4*12); // numero total de pontos
        myfile << numT << "\n";

        // desenhar topo e base
        for(int i=0; i< (slices*4) ; i++){

            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "0 %f 0\n",radius); // topo da esfera
            myfile << str1;
            normals.push_back(str1);
             
            

            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "0 %f 0\n",-radius); // base da esfera
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            myfile << str1;
            normals.push_back(str1);
             
        }

        if (stacks<2) stacks = 2;

        for(int j = 0; j< stacks-1;j++){ // ciclo externo beta
            for(int i = 0;i< (slices*4);i++){ // ciclo interno alfa
            // metade superior
                // primeiro trinagulo
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1); 
                
            // segundo triangulo

            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);
             
            //metade inferior
                // primeiro trinagulo
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            myfile << str1;
            normals.push_back(str1);
             
                // segundo triangulo
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            
            myfile << str1;
            normals.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            myfile << str1;
            normals.push_back(str1);
             

            }
        }

    for(int k = 0;k < normals.size(); k++){
        //fprintf(fd,"%s",normals[k].c_str());
        myfile << normals[k].c_str();
    }
    myfile.close();
    //fclose(fd);

    } else cout << "Unable to open file";
}

void print_plane(float x,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        fprintf(fd, "12\n"); // numero total de pontos

        float l = x/2;
        // primeiro triangulo
        fprintf(fd,"%f 0 %f\n",-l,l); 
        fprintf(fd,"%f 0 %f\n",l,l);
        fprintf(fd,"%f 0 %f\n",l,-l);

        // segundo triangulo
        fprintf(fd,"%f 0 %f\n",l,-l);
        fprintf(fd,"%f 0 %f\n",-l,-l);
        fprintf(fd,"%f 0 %f\n",-l,l);

        fprintf(fd,"%f 0 %f\n",-l,l); 
        fprintf(fd,"%f 0 %f\n",l,-l);
        fprintf(fd,"%f 0 %f\n",l,l);

        // segundo triangulo
        fprintf(fd,"%f 0 %f\n",l,-l);
        fprintf(fd,"%f 0 %f\n",-l,l);
        fprintf(fd,"%f 0 %f\n",-l,-l);

        for(int i = 0; i < 12; i++){
            if(i<6){
                fprintf(fd,"0 1 0\n");
            }
            else fprintf(fd,"0 -1 0\n");
        }
    }
    fclose(fd);
}


void print_box(float x,float y,float z,float divisions,char* file_name){
    vector<string> normals;
    FILE *fd = fopen(file_name,"w");

    float div_x = x/divisions;
    float div_y = y/divisions;
    float div_z = z/divisions;
    float x_div = x/2;
    float y_div = y/2;
    float z_div = z/2;

    if (fd){

        fprintf(fd, "%d\n", (int)(divisions*divisions*3*12)); // total de pontos

        int i, j = 0;
        for(j = 0; j< divisions;j++){
            for(i = 0;i< divisions;i++){

                    // calcular pontos e depois reutilizar 3/2 vezes
            	//XY +Z axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");

                //XY -Z axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");

				//XZ +Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");

                //XZ -Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");

                //YZ+X
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");

                //YZ-X
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");

                //segundo triangulo
            
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");
            }
        }
    }
    for(int k = 0;k < normals.size(); k++){
        fprintf(fd,"%s\n",normals[k].c_str());
    }
    fclose(fd);
}

void print_cone(float radius,float height,float slices,float stacks,char* file_name){
    vector<char*> normals;
    /*
    float hip =sqrt((height*height)+(radius*radius)); // Calculo da Hipotenusa
    float n_x = height / hip;
    float n_y = radius / hip;
    */
    FILE *fd = fopen(file_name,"w");
    if (fd){
          // 1 slice vai dividir o cone em 8 partes 
        float alfa = 2*M_PI / (slices*3);
        float rR = 1/stacks ; // multipicador do raio

        float d = 1; // d é o tamanho de cada divisao
        if(stacks > 1) d = height/stacks;  // caso 1 stack a divisao por 0

        fprintf(fd, "%d\n", (int)(slices*3*6+(stacks-1)*slices*3*6));

        ///////////////////////////////////////  Criação da base ////////////////////////////////////////////

        // desenhar a base 
        // iniciar no eixo do Z
        for(int i=0;i<(slices*3);i++){

            fprintf(fd, "%f 0 %f\n", sin(alfa*i)*radius, cos(alfa*i)*radius);
            //ponto fixo do centro
            fprintf(fd, "0 0 0\n");
            fprintf(fd, "%f 0 %f\n", sin(alfa*(i+1))*radius, cos(alfa*(i+1))*radius);

            fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*rR, height - d, cos(alfa*i)*radius*rR);
            fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*rR, height - d , cos(alfa*(i+1))*radius*rR);
            //ponto fixo do centro
            fprintf(fd, "0 %f 0\n",height);
            for(int k=0;k<6;k++){
                //normals.push_back("0 -1 0");
            }
        }
        

        /////////////////////////////////////// Criação do corpo //////////////////////////////////////////////
        for(int j=0; j < stacks-1; j++){ // camadas 
            //começamos da base e vamos subindo i é a camada inferior
            for(int i=0;i < slices*3;i++) { // circunferencia
                                                                          // 1-(rR*0) = 1 primeira camada
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*j))  ,j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*j)),j*d, cos(alfa*(i+1))*radius*(1-(rR*j))); // canto inferior direito
                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito

                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*i)*radius*(1-(rR*(j+1)))); // canto superior esquerdo
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*j)),j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
            }
        }


    }
    fclose(fd);
}

/*
void cross(float *a, float *b, float *res) {

    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}
*/

void crossProduct(Point *A, Point *B, Point *R) {
    R->x = A->y * B->z - A->z * B->y;
    R->y = -(A->x * B->z - A->z * B->x);
    R->z = A->x * B->y - A->y * B->x;
}

void normalize(float *a) {
    float l = sqrt(a->x*a->x + a->y * a->y + a->z * a->z);
    a->x = a->x/l;
    a->y = a->y/l;
    a->z = a->z/l;
}

float length(Point *a) {
    float res = sqrt(a->x*a->x + a->y * a->y + a->z * a->z);
    return res;
}

void evalBezierCurve(const Point *P, const float &t, Point *res) { 
    float b0 = (1 - t) * (1 - t) * (1 - t); 
    float b1 = 3 * t * (1 - t) * (1 - t); 
    float b2 = 3 * t * t * (1 - t); 
    float b3 = t * t * t; 

    float x = P[0].x * b0 + P[1].x * b1 + P[2].x * b2 + P[3].x * b3;
    float y = P[0].y * b0 + P[1].y * b1 + P[2].y * b2 + P[3].y * b3; 
    float z = P[0].z * b0 + P[1].z * b1 + P[2].z * b2 + P[3].z * b3; 

    res->x = x;
    res->y = y;
    res->z = z;
} 

void evalBezierPatch(const Point *controlPoints, const float &u, const float &v , Point *res) { 
	Point uCurve[4];
    for (int i = 0; i < 4; ++i) {
    	evalBezierCurve(controlPoints + 4 * i, u, &uCurve[i]);
    }
    evalBezierCurve(uCurve, v, res); 
}

void dUBezier(const Point *controlPoints, const float &u, const float &v, Point *du) { 
   Point *P = new Point[4];
   Point *vCurve = new Point[4];
   for (int i = 0; i < 4; ++i) { 
       P[0] = controlPoints[i]; 
       P[1] = controlPoints[4 + i]; 
       P[2] = controlPoints[8 + i]; 
       P[3] = controlPoints[12 + i]; 
       evalBezierCurve(P, v, &vCurve[i]); 
   }

   float b0 = -3 * (1 - u) * (1 - u);
   float b1 = (3 * (1 - u) * (1 - u) - 6 * u * (1 - u)); 
   float b2 = (6 * u * (1 - u) - 3 * u * u);
   float b3 = 3 * u * u;

   float x = vCurve[0].x * b0 + vCurve[1].x * b1 + vCurve[2].x * b2 + vCurve[3].x * b3;
   float y = vCurve[0].y * b0 + vCurve[1].y * b1 + vCurve[2].y * b2 + vCurve[3].y * b3; 
   float z = vCurve[0].z * b0 + vCurve[1].z * b1 + vCurve[2].z * b2 + vCurve[3].z * b3; 

   du->x = x;
   du->y = y;
   du->z = z;
}

void dVBezier(const Point *controlPoints, const float &u, const float &v, Point *dv) { 
   Point uCurve[4]; 
   for (int i = 0; i < 4; ++i) { 
       evalBezierCurve(controlPoints + 4 * i, u, &uCurve[i]);
   }

   float b0 = -3 * (1 - v) * (1 - v);
   float b1 = (3 * (1 - v) * (1 - v) - 6 * v * (1 - v)); 
   float b2 = (6 * v * (1 - v) - 3 * v * v);
   float b3 = 3 * v * v;

   float x = uCurve[0].x * b0 + uCurve[1].x * b1 + uCurve[2].x * b2 + uCurve[3].x * b3;
   float y = uCurve[0].y * b0 + uCurve[1].y * b1 + uCurve[2].y * b2 + uCurve[3].y * b3; 
   float z = uCurve[0].z * b0 + uCurve[1].z * b1 + uCurve[2].z * b2 + uCurve[3].z * b3; 

   dv->x = x;
   dv->y = y;
   dv->z = z;
} 

void calculate_surface(int tesselation) { 
	vectorFinal.clear();
	int divs = tesselation;

    int vertices[divs * divs * 4];
    int nvertices = (divs) * (divs) * 4;

    Point *P = new Point[(divs + 1) * (divs + 1)];

    Point *controlPoints = new Point[16]; // matriz de 16 pontos de controlo


    for (int np=0; np < patches.size(); ++np) {
        // set the control points for the current patch
        for (int i =0 ; i < 16 ; ++i) {
        	controlPoints[i] = *points[patches[np][i]];
        }
        // generate grid     
        for (int j=0, k=0; j <= divs; ++j){                 
        	for (int i=0 ; i <= divs; ++i, ++k ){
                float u = i / (float)divs; 
                float v = j / (float)divs;

                evalBezierPatch(controlPoints, u, v, &P[k]);
                Point *du = new Point;
                dUBezier(controlPoints, u, v, du);
                Point *dv = new Point;
                dVBezier(controlPoints, u, v, dv); 
                //N[k] = dU.crossProduct(dV).normalize(); 
                //st[k].x = u; 
                //st[k].y = v; 
            } 
        }
        int o=0;

        for (int j=0, k=0; j < divs; ++j) {
        	for (int i=0; i < divs; ++i, ++k) {
        		/*
        			v3______v0
					|        |
					|        |
					|		 |
					|		 |
					v2------v1
				
				*/
        		o +=4;
                vertices[k * 4] = (divs + 1) * j + i;
                vertices[k * 4 + 1] = (divs + 1) * (j + 1) + i;
                vertices[k * 4 + 2] = (divs + 1) * (j + 1) + i + 1; 
                vertices[k * 4 + 3] = (divs + 1) * j + i + 1; 
            }
        }

        vector<struct point*> temp1;
        temp1.clear();

        for(int i=0; i < nvertices; ++i){
        	Point *a = (struct point*) malloc(sizeof(struct point));
        	a->x = P[vertices[i]].x;
        	a->y = P[vertices[i]].y;
        	a->z = P[vertices[i]].z;
        	temp1.push_back(a);
        }

        // bezier to normal
        for(int size=0; size < temp1.size()-3; size+=4){
        	vectorFinal.push_back(temp1[size]); // v0
        	vectorFinal.push_back(temp1[size+3]); // v3
        	vectorFinal.push_back(temp1[size+1]); // v1

        	vectorFinal.push_back(temp1[size+1]); // v1
        	vectorFinal.push_back(temp1[size+3]); // v3
        	vectorFinal.push_back(temp1[size+2]); // v2
        }
    } 
} 

void process_patch(char *filename, int tesselation){
    ifstream infile(filename);
    int cp; 
    int np;

    vector<float> temp;

    int contador = 0;
    int total_points = 0;

    infile >> cp; // guardar numero de patches
    //cout << cp << endl;

    // ler os patch's
    while(infile.good() && contador <= cp){
        string s;
        if(!getline(infile,s)) break;
        istringstream ss(s); // guardar linha
        patch.clear();
        while(ss){
            string f;
            if (!getline(ss, f, ',' )) break;
            stringstream p(f);
            int x;
            p >> x;
            patch.push_back(x);
        }
        if (patch.size() >= 3){
            total_points += (patch.size()-2)*3;
            patches.push_back(patch);
        }
        contador ++;
    }
    infile >> np; // guardar o numero de pontos
    
    // ler os pontos
    while(infile.good()){
        temp.clear();
        string s;
        if(!getline(infile,s)) break;
        istringstream ss(s); 
        while(ss){
            string f;
            if (!getline(ss, f, ',' )) break;     
            stringstream p(f);
            float x;
            p >> x;
            temp.push_back(x);
        }
        Point *a = (struct point*) malloc(sizeof(struct point));
        if(temp.size()>=3){
            a->x = temp[0];
            a->y = temp[1];
            a->z = temp[2];
            points.push_back(a);
        }
    }

    calculate_surface(tesselation);

    char a[30];
    sprintf(a,"%s.3d",filename);
    FILE *fd = fopen(a,"w");

    if(fd){
        fprintf(fd,"%ld\n",vectorFinal.size());
        for(int i=0; i < vectorFinal.size() ; i++){
        	fprintf(fd,"%f %f %f\n",vectorFinal[i]->x,vectorFinal[i]->y,vectorFinal[i]->z);
        }
             
    }
        
    fclose(fd);
}    
