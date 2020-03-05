#include "generator.hpp"


int main(int argc, char* argv[]){

    if(argc>1){
        if (!strcmp(argv[1],"sphere")){
            if(argc < 6) {
                printf("Invalid arguments!! : \'./Generator shpere \"radius\" \"slices\" \"stacks\" \"name_file\"\' \n ");
                return -1;
            }
            print_sphere(atof(argv[2]),atof(argv[3]),atof(argv[4]),argv[5]);
        }
        else if (!strcmp(argv[1],"plane")){
                if(argc < 4) {
                    printf("Invalid arguments!! : \'./Generator plane \"width\" \"name_file\"\' \n ");
                    return -1;
                }
                print_plane(atof(argv[2]),argv[3]);
        }
        else if (!strcmp(argv[1],"box")){
                if(argc < 6) {
                    printf("Invalid arguments!! : \'./Generator box \"X\" \"Y\" \"Z\" ?\"divisions\" \"name_file\"\' \n ");
                    return -1;
                }
                if(argc == 6){
                    print_box(atof(argv[2]),atof(argv[3]),atof(argv[4]),1,argv[5]);
                }
                else{
                    print_box(atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]),argv[6]);
                }
        }
        else if (!strcmp(argv[1],"cone")){
                if(argc < 7) {
                    printf("Invalid arguments!! : \'./Generator cone \"radius\" \"height\" \"slices\" \"stacks\" \"name_file\"\' \n ");
                    return -1;
                }
                print_cone(atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]),argv[6]);
        }
        else {
            printf("Invalid arguments\n");
        }
    }
    return 0;

}
