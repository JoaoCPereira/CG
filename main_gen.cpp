#include "generator.h"


int main(int argc, char* argv[]){
    if (!strcmp(argv[1],"sphere")){
        if(argc < 6) return -1;
        print_sphere(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),argv[5]);
    }
    if (!strcmp(argv[1],"plane")){
        if(argc < 4) return -1;
        print_plane(atoi(argv[2]),argv[3]);
    }
    
    if (!strcmp(argv[1],"box")){
        if(argc < 6) return -1;
        if(argc == 6){
            print_box(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),-1,argv[5]);
        }
        else{
            print_box(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
        }
    }

    if (!strcmp(argv[1],"cone")){
        if(argc < 7) return -1;
        print_cone(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),argv[6]);
    }

    return 0;

}
