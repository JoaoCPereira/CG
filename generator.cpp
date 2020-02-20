#include "generator.h"

void print_sphere(float radius,float slices,float stacks,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        //definir xml
    }
    fclose(fd);
}

void print_plane(float x,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        float l = x/2;
        // primeiro triangulo
        fprintf(fd,"%f,0,%f\n",-l,l); 
        fprintf(fd,"%f,0,%f\n",l,l);
        fprintf(fd,"%f,0,%f\n",l,-l);

        // segundo triangulo
        fprintf(fd,"%f,0,%f\n",l,-l);
        fprintf(fd,"%f,0,%f\n",-l,-l);
        fprintf(fd,"%f,0,%f\n",-l,l);
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
        
        //XY +Z axis

        int i, j = 0;
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0,1,0);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),z_div);
            }
        }

        //XY -Z axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0,1,1);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*(j+1)),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),(-y_div+div_y*j),-z_div);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),(-y_div+div_y*j),-z_div);
            }
        }

        //XZ +Y axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(1,1,0);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),y_div,z_div-div_z*j);
            }
        }

        //XZ -Y axis
        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0.5,0,0.5);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));

                //segundo triangulo
                
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*(j+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*(i+1)),-y_div,z_div-div_z*j);
                fprintf(fd, "%f,%f,f%f\n",(-x_div+div_x*i),-y_div,z_div-div_z*j);
            }
        }


        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(0.2,0.2,0.6);\n");
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(x_div),(-y_div+div_y*j),z_div-div_z*i);
            }
        }

        for(j = 0; j< divisions;j++){
            for(i = 0;i<divisions;i++){
                //primeiro triangulo
                fprintf(fd, "glColor3f(1,1,1);\n");
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*i);
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));

                //segundo triangulo
            
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*(j+1)),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*(i+1));
                fprintf(fd, "%f,%f,f%f\n",(-x_div),(-y_div+div_y*j),z_div-div_z*i);
            }
        }


        


    }
    fclose(fd);
}

void print_cone(float radius,float height,float slices,float stacks,char* file_name){
    FILE *fd = fopen(file_name,"w");
    if (fd){
        //definir xml
    }
    fclose(fd);

}



