#include "readXMLAUX.hpp"


int main(int argc, char* argv[]){

	Modelo *model=NULL;

    //Point *vectorP=NULL;

    model = read3d(argv[1]);

    write3d(model);
    return 0;

}
