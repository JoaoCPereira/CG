#include "generator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

vector< vector<int> > patches;
vector<int> patch;
float pos[4],deriv[4];

vector<struct point*> bezier_points_to_calculus;
vector<struct point*> points;
vector<struct point*> vector_pos;
vector<struct point*> vector_final;

vector< vector<struct point*> > final;

int total_de_pontos;

void evalBezierCurve(const Point *P, const float &t, Point *res) 
//Vec3f evalBezierCurve(const Vec3f *P, const float &t) 
{ 
    float b0 = (1 - t) * (1 - t) * (1 - t); 
    float b1 = 3 * t * (1 - t) * (1 - t); 
    float b2 = 3 * t * t * (1 - t); 
    float b3 = t * t * t; 

    //Point *res = (struct point*) malloc(sizeof(struct point));

    //cout << "Ponto (" << res->x << "," << res->y << "," << res->z << ")" << "!="; 

    //cout << "Ponto (" << P[0].x * b0 + P[1].x * b1 + P[2].x * b2 + P[3].x * b3 << "," << P[0].y * b0 + P[1].y * b1 + P[2].y * b2 + P[3].y * b3 << "," << P[0].z * b0 + P[1].z * b1 + P[2].z * b2 + P[3].z * b3 << ")" << "==";

    float x = P[0].x * b0 + P[1].x * b1 + P[2].x * b2 + P[3].x * b3;
    float y = P[0].y * b0 + P[1].y * b1 + P[2].y * b2 + P[3].y * b3; 
    float z = P[0].z * b0 + P[1].z * b1 + P[2].z * b2 + P[3].z * b3; 

    res->x = x;
    res->y = y;
    res->z = z;

    //cout << "Ponto (" << res->x << "," << res->y << "," << res->z << ")" << "=="; 

    //return res; 
    //  return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3; 
} 


void evalBezierPatch(const Point *controlPoints, const float &u, const float &v , Point *res)
//Vec3f evalBezierPatch(const Vec3f *controlPoints, const float &u, const float &v) 
{ 
	Point uCurve[4];
    //Vec3f uCurve[4];
    for (int i = 0; i < 4; ++i) {
    	evalBezierCurve(controlPoints + 4 * i, u, &uCurve[i]);
    	//cout << "Ponto (" << uCurve[i].x << "," << uCurve[i].y << "," << uCurve[i].z << ")" << endl;
    }

    evalBezierCurve(uCurve, v, res); 
    //cout << endl;
} 


/*
float norm(struct point a){
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

struct point normalize(struct point a) {

	float l = norm(a);
	a.x = a.x/l;
    a.y = a.y/l;      
	a.z = a.z/l;
    return a;
}

Point cross_prod(Point a, Point b){
    Point ret = (struct point*) malloc(sizeof(struct point));
    ret->x = (a->y * b->z) - (a->z * b->y);
    ret->y = ( a->z * b->x) - (a->x * b->z);
    ret ->z = (a->x * b->y) - (a->y * b->x);

    return ret;

}

void multMatrixVector(float *m, float *v, float *res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

*/

void print_sphere(float radius,float slices,float stacks,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        float alfa = 2*M_PI / (slices*4);
        float beta = (M_PI/2) / stacks;

        fprintf(fd, "%d\n", (int)(slices*4*6+(stacks-1)*slices*4*12)); // numero total de pontos

        // desenhar topo e base
        for(int i=0; i< (slices*4) ; i++){

            fprintf(fd, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            fprintf(fd, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            fprintf(fd, "0 %f 0\n",radius); // topo da esfera

            fprintf(fd, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            fprintf(fd, "0 %f 0\n",-radius); // base da esfera
            fprintf(fd, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
        }

        if (stacks<2) stacks = 2;

        for(int j = 0; j< stacks-1;j++){ // ciclo externo beta
            for(int i = 0;i< (slices*4);i++){ // ciclo interno alfa
            // metade superior
                // primeiro trinagulo
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
                // segundo triangulo

            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero

            //metade inferior
                // primeiro trinagulo
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
                // segundo triangulo
            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            fprintf(fd, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            fprintf(fd, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero

                
            }
        }
    }
    fclose(fd);
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
        

        

    }
    fclose(fd);
}


void print_box(float x,float y,float z,float divisions,char* file_name){
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

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);

                //XY -Z axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);

				//XZ +Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);

                //XZ -Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);

                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);

                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
            
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
            }
        }
    }
    fclose(fd);
}

void print_cone(float radius,float height,float slices,float stacks,char* file_name){
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
struct point operator* (float s,struct point A){
    struct point ret;
    
    ret.x = A.x*s;
    ret.y = A.y*s;
    ret.z = A.z*s;
    return ret;
}

struct point operator+ (struct point A,struct point B){
    struct point ret;
    
    ret.x = A.x+B.x;
    ret.y = A.y+B.y;
    ret.z = A.z+B.z;
    return ret;
}


static struct point mat_dot_product_MP (const float M[4][4], const Point P[4][4], unsigned i, unsigned j)
{
    struct point ret;
    for (unsigned I = 0; I < 4; I++)
        ret = ret + M[i][I] * *(P[I][j]);
    return ret;
}

static struct point mat_dot_product_PM (const struct point P[4][4], const float M[4][4], unsigned i, unsigned j)
{
    struct point ret;
    for (unsigned I = 0; I < 4; I++)
        ret = ret + M[I][j] * P[i][I];
    return ret;
}

static void mult_MP (const float M[4][4], const Point P[4][4], struct point r[4][4])
{
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 4; j++)
            r[i][j] = mat_dot_product_MP(M, P, i, j);
}

static void mult_PM (const struct point P[4][4], const float M[4][4], struct point r[4][4])
{
    for (unsigned i = 0; i < 4; i++)
        for (unsigned j = 0; j < 4; j++)
            r[i][j] = mat_dot_product_PM(P, M, i, j);
}

static void mult_MPM (const float M[4][4], const Point P[4][4], struct point r[4][4])
{
    struct point tmp[4][4];

    mult_MP(M, P, tmp);
    mult_PM(tmp, M, r);
}

struct point normal (struct point p1, struct point p2)
{
    struct point tmp;
    tmp.x = p1.y * p2.z;
    tmp.y = p1.z * p2.x; 
    tmp.z = p1.x * p2.y;
    return normalize(tmp);
}

struct point gen_bezier_get_single_point (const struct point MPM[4][4], float u, float v, Point N)
{
    struct point tmp[4];


    for (unsigned j = 0; j < 4; j++)
        tmp[j] = (3 * u * u * MPM[j][0])
            + (2 * u * MPM[j][1])
            + MPM[j][2];
    struct point Nu = (v * v * v * tmp[0])
        + (v * v * tmp[1])
        + (v * tmp[2])
        + tmp[3];

    for (unsigned j = 0; j < 4; j++)
        tmp[j] = (u * u * u * MPM[j][0])
            + (u * u * MPM[j][1])
            + (u * MPM[j][2])
            + MPM[j][3];
    struct point Nv = (3 * v * v * tmp[0])
        + (2 * v * tmp[1])
        + tmp[2];

    *N = normal(Nu, Nv);

    for (unsigned j = 0; j < 4; j++)
        tmp[j] = u * u * u * MPM[j][0]
            + (u * u * MPM[j][1])
            + (u * MPM[j][2])
            + MPM[j][3];

    return (v * v * v * tmp[0])
        + (v * v * tmp[1])
        + (v * tmp[2])
        + tmp[3];
}


//ainda nao sei o que retorna
void calculate_surface(vector<Point> points,vector<int> patch, float tessellation){
    
    const float M[4][4] =   {   { -1,  3, -3, 1, },
                                {  3, -6,  3, 0, },
                                { -3,  3,  0, 0, },
                                {  1,  0,  0, 0, }};

    Point P[4][4] = {   {points[patch[0]],points[patch[1]],points[patch[2]],points[patch[3]]},
                        {points[patch[4]],points[patch[5]],points[patch[6]],points[patch[7]]},
                        {points[patch[8]],points[patch[9]],points[patch[10]],points[patch[11]]},
                        {points[patch[12]],points[patch[13]],points[patch[14]],points[patch[15]]}};

    struct point MPM[4][4];
    mult_MPM(M,P,MPM);

    for (unsigned i = 1; i <= 4 * tessellation; i++) {
        float u  = ((float) i)     / (4.0 * tessellation);
        float u_ = ((float) i - 1) / (4.0 * tessellation);

        for (unsigned j = 1; j <= 4 * tessellation; j++) {
            float v  = ((float) j)     / (4.0 * tessellation);
            float v_ = ((float) j - 1) / (4.0 * tessellation);

            struct point ret_1,ret_2,ret_3,ret_4;

            struct point N1,N2,N3,N4;

            ret_1 = gen_bezier_get_single_point(MPM, u,  v_, &N1);
            ret_2 = gen_bezier_get_single_point(MPM, u,  v, &N2);
            ret_3 = gen_bezier_get_single_point(MPM, u_,  v_, &N3);
            ret_4 = gen_bezier_get_single_point(MPM, u_,  v, &N4);

            cout << ret_1.x << ',' << ret_1.y << ',' << ret_1.z << endl;
            cout << ret_2.x << ',' << ret_2.y << ',' << ret_2.z << endl;
            cout << ret_3.x << ',' << ret_3.y << ',' << ret_3.z << endl;
            cout << ret_4.x << ',' << ret_4.y << ',' << ret_4.z << endl;
            cout << endl;

            vector_final.push_back(N1);
            vector_final.push_back(N2);
            vector_final.push_back(N3);
            vector_final.push_back(N3);
            vector_final.push_back(N2);
            vector_final.push_back(N4);
        }
    }
}

*/
void generatePolyTeapot(int tesselation) 
//void generatePolyTeapot(const Matrix44f &o2w, std::vector<Object *> &objects) 
{ 

	final.clear();
	int divs = tesselation;
    //uint32_t divs = 16;

	//Point P[(divs+1)*(divs +1)];

	//Point P = (struct point*) malloc((divs+1)*(divs +1) * sizeof(struct point));

	Point *P = new Point[(divs + 1) * (divs + 1)];

	//Point P = (struct point*) malloc(sizeof(struct point));

    //Vec3f *P = new Vec3f[(divs + 1) * (divs + 1)]; 

	int nvertices[divs * divs];
    //uint32_t *nvertices = new uint32_t[divs * divs]; 

    int vertices[divs * divs * 4];
    //uint32_t *vertices = new uint32_t[divs * divs * 4]; 

    Point *controlPoints = new Point[16]; // matriz de 16 pontos de controlo



    //Vec3f controlPoints[16];
    for (int np=0; np < patches.size(); ++np) {

    //for (int np = 0; np < kTeapotNumPatches; ++np) { 
        // set the control points for the current patch
        for (int i =0 ; i < 16 ; ++i) {                                                                                                                                           
        //for (uint32_t i = 0; i < 16; ++i) 

        	/*
        	cout << endl;
        	cout << i << " " << points[patches[np][i]]->z << " points_Size=" << points.size() << " patches_Size=" << patches[np][i] <<endl;

        	cout << "Point(" << points[patches[np][i]]->x << ", " << points[patches[np][i]]->y << ", " << points[patches[np][i]]->z  << ")" << endl;
        	cout << "Point(" << controlPoints[i].x << ", " << controlPoints[i].y << ", " << controlPoints[i].z  << ")" << endl;

        	cout << "-------------------" <<endl;
        	*/
        	//cout << "np=" << np <<  " i=" << i << "" << patches[np][i] <<endl; 
        	//cout << points[patches[np][i]]->x << endl;

        	controlPoints[i].x = points[patches[np][i]]->x;
        	controlPoints[i].y = points[patches[np][i]]->y;
        	controlPoints[i].z = points[patches[np][i]]->z;


        	/*
        	Point *a = (struct point*) malloc(sizeof(struct point));

        	cout << "------------------------" << endl;

        	cout << "Ponto (" << a->x << "," << a->y << "," << a->z << ")" << endl; 

        	evalBezierCurve(controlPoints + 4 * 1, 1, a);

        	cout << "Ponto (" << a->x << "," << a->y << "," << a->z << ")" << endl; 

        	cout << "------------------------" << endl;

        	*/

        	/*
        	cout << "Point(" << points[patches[np][i]]->x << ", " << points[patches[np][i]]->y << ", " << points[patches[np][i]]->z  << ")" << endl;
        	cout << "Point(" << controlPoints[i].x << ", " << controlPoints[i].y << ", " << controlPoints[i].z  << ")" << endl;

        	cout << endl;
        	*/

        	//cout << points[patches[np][i]]->x << " " << controlPoints[i]->x << endl;

        	//controlPoints[i] = points[patch[np][i]-1];
            // controlPoints[i][0] = teapotVertices[teapotPatches[np][i] - 1][0], 
            // controlPoints[i][1] = teapotVertices[teapotPatches[np][i] - 1][1], 
            // controlPoints[i][2] = teapotVertices[teapotPatches[np][i] - 1][2]; 
        }
        // generate grid     
        for (int j=0, k=0; j <= divs; ++j){                                                                                                                                                                              
        //for (uint16_t j = 0, k = 0; j <= divs; ++j) { 
        	for (int i=0 ; i <= divs; ++i, ++k ){
            //for (uint16_t i = 0; i <= divs; ++i, ++k) { 
            	//cout << P[k].x << "!=";
                evalBezierPatch(controlPoints, i / (float)divs, j / (float)divs , &P[k]);
                //cout << P[k].x << endl;
            } 
        } 
        // face connectivity
        for (int j=0, k=0; j < divs; ++j) {
        //for (uint16_t j = 0, k = 0; j < divs; ++j) { 
        	for (int i=0; i < divs; ++i, ++k) {
            //for (uint16_t i = 0; i < divs; ++i, ++k) { 
                nvertices[k] = 4; 
                vertices[k * 4] = (divs + 1) * j + i; 
                vertices[k * 4 + 1] = (divs + 1) * (j + 1) + i; 
                vertices[k * 4 + 2] = (divs + 1) * (j + 1) + i + 1; 
                vertices[k * 4 + 3] = (divs + 1) * j + i + 1; 
            } 
        } 

        vector<struct point*> temp1;
        temp1.clear();

        for(int i=0; i < (divs * divs)*4 ; i++){
        	//cout << vertices[i] << endl;
        	//cout << "(" << P[vertices[i]].x << "," << P[vertices[i]].y << "," << P[vertices[i]].y << "" <<endl;
        	Point *a = (struct point*) malloc(sizeof(struct point));
        	a->x = P[vertices[i]].x;
        	a->y = P[vertices[i]].y;
        	a->z = P[vertices[i]].z;
        	temp1.push_back(a);
        }
        //cout << vector_final.size() << endl;

        // bezier to normal
        vector<struct point*> temp2;
        temp2.clear();

        for(int size=0; size < temp1.size()-3; size+=2){
        	temp2.push_back(temp1[size]); // v0
        	temp2.push_back(temp1[size+1]); // v1
        	temp2.push_back(temp1[size+3]); // v3

        	temp2.push_back(temp1[size+1]); // v1
        	temp2.push_back(temp1[size+2]); // v2
        	temp2.push_back(temp1[size+3]); // v3
        }

        //cout << total_de_pontos << endl;
		total_de_pontos +=temp2.size();
        final.push_back(temp2);
 		
        //objects.push_back(new PolygonMesh(o2w, divs * divs, nvertices, vertices, P)); 
    } 
 
    //delete [] P; 
    //delete [] nvertices; 
    //delete [] vertices; 
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
    //cout << "acabou os cp" << endl;
    infile >> np; // guardar o numero de pontos
    //cout << np << endl;
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

    /*
    for(vector<int> patch : patches){
        calculate_surface(points,patch,tesselation);
    }
	*/

	total_de_pontos=0;

    generatePolyTeapot(tesselation);

    char a[30];
    sprintf(a,"%s.3d",filename);
    FILE *fd = fopen(a,"w");

    if(fd){
        fprintf(fd,"%d\n",total_de_pontos);

        for(int i=0; i < final.size(); i++){
        	for(int j=0; j < final[i].size() ; j++){
        		fprintf(fd,"%f %f %f\n",final[i][j]->x,final[i][j]->y,final[i][j]->z);
        	}
        }
        /*
        for(struct point p : vector_final){
            fprintf(fd,"%f %f %f\n",p.x,p.y,p.z);
        }*/
             
    }
        
    fclose(fd);
}    
