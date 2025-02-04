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
vector<struct point*> vectorNorm;
vector<string> texCoord;


void print_sphere(float radius,float slices,float stacks,char* file_name){
    vector<string> normals;
    vector<string> texCoord;
    texCoord.clear();
    normals.clear();

    char str1[100];
    ofstream myfile;
    myfile.open (file_name);

    if (myfile.is_open()){
        float alfa = 2*M_PI / (slices*4);
        float beta = (M_PI/2) / stacks;

        int numT = (int)(slices*4*6+(stacks-1)*slices*4*12); // numero total de pontos
        myfile << numT << "\n";

        float lat = 1/ (float) (slices*4);
        float lon = 1/ (float) (stacks*2);

        // desenhar topo e base
        for(int i=0; i< (slices*4) ; i++){

            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i, 1-lon);
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1), 1-lon);
            texCoord.push_back(str1);
             
            sprintf(str1, "0 %f 0\n",radius); // topo da esfera
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f 1\n",lat*(i+1));
            texCoord.push_back(str1);
             

            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*i),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*i));
            myfile << str1;
            normals.push_back(str1);

            sprintf(str1, "%f %f\n",lat*i, lon);
            texCoord.push_back(str1);
             
            sprintf(str1, "0 %f 0\n",-radius); // base da esfera
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f 0\n",lat*(i+1));
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(stacks-1))*radius*sin(alfa*(i+1)),-sin(beta*(stacks-1))*radius,cos(beta*(stacks-1))*radius*cos(alfa*(i+1)));
            myfile << str1;
            normals.push_back(str1);

            sprintf(str1, "%f %f\n",lat*(i+1), lon);
            texCoord.push_back(str1);
             
        }

        if (stacks<2) stacks = 2;

        for(int j = 0; j< stacks-1;j++){ // ciclo externo beta
            for(int i = 0;i< (slices*4);i++){ // ciclo interno alfa
            // metade superior
                // primeiro trinagulo
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);

            sprintf(str1, "%f %f\n",lat*i,0.5 + lon*j);
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 + lon*j);
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1); 
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 + lon*(j+1));
            texCoord.push_back(str1);
                
            // segundo triangulo

            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 +  lon*(j+1));
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i,0.5 +  lon*(j+1));
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i,0.5 +  lon*j);
            texCoord.push_back(str1);

            //metade inferior
                // primeiro trinagulo
            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i,0.5 - lon*j);
            texCoord.push_back(str1);
             
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 - lon*(j+1));
            texCoord.push_back(str1);

            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*(i+1)),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*(i+1))); // canto inferior direito
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 - lon*j);
            texCoord.push_back(str1);
             
                // segundo triangulo
            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*(i+1)),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*(i+1))); // canto superior esquerdo
            
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*(i+1),0.5 - lon*(j+1));
            texCoord.push_back(str1);

            sprintf(str1, "%f %f %f\n",cos(beta*j)*radius*sin(alfa*i),-sin(beta*j)*radius,cos(beta*j)*radius*cos(alfa*i)); // canto inferior esquero
            
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i,0.5 - lon*j);
            texCoord.push_back(str1);

            sprintf(str1, "%f %f %f\n",cos(beta*(j+1))*radius*sin(alfa*i),-sin(beta*(j+1))*radius,cos(beta*(j+1))*radius*cos(alfa*i)); // canto superior esquero
            myfile << str1;
            normals.push_back(str1);
            sprintf(str1, "%f %f\n",lat*i,0.5 - lon*(j+1));
            texCoord.push_back(str1);
            }
        }

    for(int k = 0;k < normals.size(); k++){
        //fprintf(fd,"%s",normals[k].c_str());
        myfile << normals[k].c_str();
    }
    for(int k = 0;k < texCoord.size(); k++){
        //fprintf(fd,"%s",normals[k].c_str());
        myfile << texCoord[k].c_str();
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

        fprintf(fd,"0 0\n");
        fprintf(fd,"1 0\n");
        fprintf(fd,"1 1\n"); 

        fprintf(fd,"1 1\n");
        fprintf(fd,"0 1\n");
        fprintf(fd,"0 0\n");

        fprintf(fd,"0 0\n");
        fprintf(fd,"1 1\n");
        fprintf(fd,"1 0\n"); 

        fprintf(fd,"1 1\n");
        fprintf(fd,"0 0\n");
        fprintf(fd,"0 1\n");

    }
    fclose(fd);
}


void print_box(float x,float y,float z,float divisions,char* file_name){
    vector<string> normals;
    vector<string> texCoord;
    normals.clear();
    texCoord.clear();

    float face = 1/ (float) divisions;
    char str1[100];

    FILE *fd = fopen(file_name,"w");

    float div_x = x/divisions;
    float div_y = y/divisions;
    float div_z = z/divisions;
    float x_div = x/2;
    float y_div = y/2;
    float z_div = z/2;

    int total_de_pontos = (int)(divisions*divisions*3*12);

    if (fd){

        fprintf(fd, "%d\n", total_de_pontos); // total de pontos

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

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");
                normals.push_back("0 0 1");

                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*i,face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*i,face*j);
                texCoord.push_back(str1);

                //XY -Z axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(x_div-div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(x_div-div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(x_div-div_x*i),(-y_div+div_y*(j+1)),-z_div);
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(x_div-div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f %f %f\n",(x_div-div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f %f %f\n",(x_div-div_x*(i+1)),(-y_div+div_y*j),-z_div);
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");
                normals.push_back("0 0 -1");

                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j));
                texCoord.push_back(str1);

                //XZ +Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");
                normals.push_back("0 1 0");

                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);

                //XZ -Y axis
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,-z_div+div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,-z_div+div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,-z_div+div_z*(j+1));
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,-z_div+div_z*(j+1));
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*i),-y_div,-z_div+div_z*j);
                fprintf(fd, "%f %f %f\n",(-x_div+div_x*(i+1)),-y_div,-z_div+div_z*j);
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");
                normals.push_back("0 -1 0");

                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j));
                texCoord.push_back(str1);

                //YZ+X
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f %f %f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");
                normals.push_back("1 0 0");

                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);

                //YZ-X
                //primeiro triangulo
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),(-z_div)+div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),(-z_div)+div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),(-z_div)+div_z*(i+1));
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*j);
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);

                //segundo triangulo
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*j),(-z_div)+div_z*i);
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),(-z_div)+div_z*(i+1));
                fprintf(fd, "%f %f %f\n",(-x_div),(-y_div+div_y*(j+1)),(-z_div)+div_z*(i));
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");
                normals.push_back("-1 0 0");

                sprintf(str1, "%f %f\n",face*(i),face*(j));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i+1),face*(j+1));
                texCoord.push_back(str1);
                sprintf(str1, "%f %f\n",face*(i),face*(j+1));
                texCoord.push_back(str1);
            }
        }
    }
    for(int k = 0;k < total_de_pontos; k++){
        fprintf(fd,"%s\n",normals[k].c_str());
    }
    for(int k = 0;k < total_de_pontos; k++){
        fprintf(fd,"%s",texCoord[k].c_str());
    }

    fclose(fd);
}

void print_cone(float radius,float height,float slices,float stacks,char* file_name){
    vector<string> normals;
    vector<string> texture_vec;
    float normal_x;
    float normal_z;
    float norm;
    float faces = 1/(slices*3);
    float height_divisions = 1/stacks;

    char to_normals[128];
    char to_texture[128];
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
            sprintf(to_texture,"%f %f\n",(0.5+0.5*sin(alfa*(i))),(0.5+0.5*cos(alfa*(i))));
            texture_vec.push_back(to_texture);
            //ponto fixo do centro
            fprintf(fd, "0 0 0\n");
            texture_vec.push_back("0.5 0.5\n");

            fprintf(fd, "%f 0 %f\n", sin(alfa*(i+1))*radius, cos(alfa*(i+1))*radius);
            sprintf(to_texture,"%f %f\n",(0.5+0.5*sin(alfa*(i+1))),(0.5+0.5*cos(alfa*(i+1))));
            texture_vec.push_back(to_texture);

            fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*rR, height - d, cos(alfa*i)*radius*rR);
            sprintf(to_texture,"%f %f\n",(0.5+0.5*sin(alfa*i)),(0.5+0.5*cos(alfa*i)));
            texture_vec.push_back(to_texture);


            fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*rR, height - d , cos(alfa*(i+1))*radius*rR);
            sprintf(to_texture,"%f %f\n",(0.5+0.5*sin(alfa*(i+1))),(0.5+0.5*cos(alfa*(i+1))));
            texture_vec.push_back(to_texture);
            //ponto fixo do centro
            fprintf(fd, "0 %f 0\n",height);
            texture_vec.push_back("0.5 0.5\n");

            for(int k=0;k<6;k++){
                normals.push_back("0 -1 0\n");
            }
        }
        

        /////////////////////////////////////// Criação do corpo //////////////////////////////////////////////
        for(int j=0; j < stacks-1; j++){ // camadas 
            //começamos da base e vamos subindo i é a camada inferior
            for(int i=0;i < slices*3;i++) { // circunferencia
                // 1-(rR*0) = 1 primeira camada
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*j))  ,j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
                normal_x = sin(alfa*i)*radius*(1-(rR*j));
                normal_z = cos(alfa*i)*radius*(1-(rR*j));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                //normals
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);
                //textures
                sprintf(to_texture,"%f %f\n",i*faces,j*height_divisions);
                texture_vec.push_back(to_texture);


                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*j)),j*d, cos(alfa*(i+1))*radius*(1-(rR*j))); // canto inferior direito
                normal_x = sin(alfa*(i+1))*radius*(1-(rR*j));
                normal_z = cos(alfa*(i+1))*radius*(1-(rR*j));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);

                sprintf(to_texture,"%f %f\n",(i+1)*faces,j*height_divisions);
                texture_vec.push_back(to_texture);
               
                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito
                normal_x = sin(alfa*(i+1))*radius*(1-(rR*(j+1)));
                normal_z = cos(alfa*(i+1))*radius*(1-(rR*(j+1)));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);

                sprintf(to_texture,"%f %f\n",(i+1)*faces,(j+1)*height_divisions);
                texture_vec.push_back(to_texture);

                fprintf(fd, "%f %f %f\n", sin(alfa*(i+1))*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*(i+1))*radius*(1-(rR*(j+1)))); // canto superior direito
                normal_x = sin(alfa*(i+1))*radius*(1-(rR*(j+1)));
                normal_z = cos(alfa*(i+1))*radius*(1-(rR*(j+1)));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);

                sprintf(to_texture,"%f %f\n",(i+1)*faces,(j+1)*height_divisions);
                texture_vec.push_back(to_texture);
                
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*(j+1))),(j+1)*d, cos(alfa*i)*radius*(1-(rR*(j+1)))); // canto superior esquerdo
                normal_x = sin(alfa*i)*radius*(1-(rR*(j+1)));
                normal_z = cos(alfa*i)*radius*(1-(rR*(j+1)));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);

                sprintf(to_texture,"%f %f\n",(i)*faces,(j+1)*height_divisions);
                texture_vec.push_back(to_texture);
                
                
                fprintf(fd, "%f %f %f\n", sin(alfa*i)*radius*(1-(rR*j)),j*d, cos(alfa*i)*radius*(1-(rR*j))); // canto inferior esquerdo
                normal_x = sin(alfa*i)*radius*(1-(rR*j));
                normal_z = cos(alfa*i)*radius*(1-(rR*j));
                norm = sqrt(normal_x*normal_x+normal_z*normal_z);
                normal_x /= norm;
                normal_z /= norm;
                normal_x *= (height/rR);
                normal_z *= (height/rR);
                sprintf(to_normals,"%f 0 %f\n",normal_x,normal_z);
                normals.push_back(to_normals);

                sprintf(to_texture,"%f %f\n",(i)*faces,(j)*height_divisions);
                texture_vec.push_back(to_texture);


            }
        }
    for(int k = 0;k < normals.size(); k++){
        fprintf(fd,"%s",normals[k].c_str());
    }

    for(int k = 0;k < texture_vec.size(); k++){
        fprintf(fd,"%s",texture_vec[k].c_str());
    }


    fclose(fd);
    }
}

void crossProduct(Point *A, Point *B, Point *R) {
    R->x = A->y * B->z - A->z * B->y;
    R->y = -(A->x * B->z - A->z * B->x);
    R->z = A->x * B->y - A->y * B->x;
}

void normalize(Point *a, Point *n) {
    float l = sqrt(a->x*a->x + a->y * a->y + a->z * a->z);
    n->x = a->x/l;
    n->y = a->y/l;
    n->z = a->z/l;
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
    vectorNorm.clear();
    texCoord.clear();

    vector<string> texCoordTemp;
    char str[100];

    int divs = tesselation;

    int vertices[divs * divs * 4];
    int nvertices = (divs) * (divs) * 4;

    Point *P = new Point[(divs + 1) * (divs + 1)];

    Point *N = new Point[(divs + 1) * (divs + 1)];

    Point *controlPoints = new Point[16]; // matriz de 16 pontos de controlo


    for (int np=0; np < patches.size(); ++np) {
        texCoordTemp.clear();
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

                Point *n = new Point;
                crossProduct(du,dv,n);
                normalize(n, &N[k]);

                sprintf(str,"%f %f\n",1-u,1-v);

                texCoordTemp.push_back(str);
            } 
        }
        int o=0;

        for (int j=0, k=0; j < divs; ++j) {
            for (int i=0; i < divs; ++i, ++k) {
                /*
                    v3______v0
                    |        |
                    |        |
                    |        |
                    |        |
                    v2------v1
                
                */
                o +=4;
                vertices[k * 4] = (divs + 1) * j + i;
                vertices[k * 4 + 1] = (divs + 1) * (j + 1) + i;
                vertices[k * 4 + 2] = (divs + 1) * (j + 1) + i + 1; 
                vertices[k * 4 + 3] = (divs + 1) * j + i + 1; 
            }
        }

        vector<struct point*> tempVer;
        vector<struct point*> tempNor;
        tempVer.clear();
        tempNor.clear();

        for(int i=0; i < nvertices; ++i){
            Point *a = (struct point*) malloc(sizeof(struct point));
            a->x = P[vertices[i]].x;
            a->y = P[vertices[i]].y;
            a->z = P[vertices[i]].z;
            
            Point *n = (struct point*) malloc(sizeof(struct point));
            n->x = N[vertices[i]].x;
            n->y = N[vertices[i]].y;
            n->z = N[vertices[i]].z;

            tempVer.push_back(a);
            tempNor.push_back(n);
        }

        // bezier to normal
        for(int size=0; size < tempVer.size()-3; size+=4){
            vectorFinal.push_back(tempVer[size]); // v0
            vectorFinal.push_back(tempVer[size+3]); // v3
            vectorFinal.push_back(tempVer[size+1]); // v1

            vectorFinal.push_back(tempVer[size+1]); // v1
            vectorFinal.push_back(tempVer[size+3]); // v3
            vectorFinal.push_back(tempVer[size+2]); // v2

            vectorNorm.push_back(tempNor[size]); // v0
            vectorNorm.push_back(tempNor[size+3]); // v3
            vectorNorm.push_back(tempNor[size+1]); // v1

            vectorNorm.push_back(tempNor[size+1]); // v1
            vectorNorm.push_back(tempNor[size+3]); // v3
            vectorNorm.push_back(tempNor[size+2]); // v2
            
        }

        float face = 1/ (float) divs;

        for(int i=0; i < divs; i++){
            for(int j=0; j <divs; j++){
                sprintf(str, "%f %f\n",1-(j*face), 1-(i*face));
                texCoord.push_back(str);
                sprintf(str, "%f %f\n",1-((j+1)*face), 1-(i*face));
                texCoord.push_back(str);
                sprintf(str, "%f %f\n",1-((j)*face), 1-((i+1)*face));
                texCoord.push_back(str);


                sprintf(str, "%f %f\n",1-((j)*face), 1-((i+1)*face));
                texCoord.push_back(str);
                sprintf(str, "%f %f\n",1-((j+1)*face), 1-((i)*face));
                texCoord.push_back(str);
                sprintf(str, "%f %f\n",1-((j+1)*face), 1-((i+1)*face));
                texCoord.push_back(str);
            }
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

        for(int i=0; i < vectorNorm.size() ; i++){
            fprintf(fd,"%f %f %f\n",vectorNorm[i]->x,vectorNorm[i]->y,vectorNorm[i]->z);
        }
        for(int k = 0;k < texCoord.size(); k++){
            fprintf(fd,"%s",texCoord[k].c_str());
        }
             
    }
        
    fclose(fd);
}    
