#ifndef __READXMLCLASS__
#define __READXMLCLASS__

#include "engineSrcAUX.h"

class SysState {
  private:
    static vector<struct modelo*> modelos;
  static vector<struct geo_transf*> geo_tr;
  static vector<struct translate*> tr;
  static vector<struct light*> lights;
  static vector<int> sequencia;
  static vector <float> preVBO;
  static vector<float> preNormal;
  static vector<float> preTextCoord;

  public:
    SysState(char *fileName);
    static void renderScene(void);

  private:
    static int read3D(char *filename, float, float, float ,float ,float ,float, char *);
    static void parserXML(TiXmlElement *element);
    static void readXML(char *pFilename);
    static void writeSeq();
};

#endif