#ifndef __READXMLCLASS__
#define __READXMLCLASS__

#include "readXMLAUX.h"

class SysState {
  private:
   	static vector<struct modelo*> modelos;
	static vector<struct geo_transf*> geo_tr;
	static vector<struct translate*> tr;
	static vector<int> sequencia;
	static vector <float> preVBO;

  public:
    SysState(char *fileName);
    static void renderScene(void);

  private:
    static int read3D(char *filename, float, float, float );
    static void parserXML(TiXmlElement *element);
    static void readXML(char *pFilename);
    static void writeSeq();
};

#endif