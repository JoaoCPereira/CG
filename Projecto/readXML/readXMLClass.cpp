#include "readXMLClass.h"

vector<struct modelo*> SysState::modelos;
vector<struct geo_transf*> SysState::geo_tr;
vector<int> SysState::sequencia;
vector <float> SysState::preVBO;

extern float angleBeta,angleAlfa,distanciaCamera; // variaveis globais externas do ficheiro main.cpp
extern GLuint buffers; // variaveis globais externas do ficheiro main.cpp

SysState::SysState(char *fileName){
    readXML(fileName);

    //carregar os dados para o VBO
    glGenBuffers(1, &buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers);
    glVertexPointer(3,GL_FLOAT,0,0);
    glBufferData(GL_ARRAY_BUFFER,preVBO.size()*sizeof(float), preVBO.data(), GL_STATIC_DRAW);
}

int SysState::read3D(char *filename,float diffR, float diffG, float diffB){

  Modelo *mod =(struct modelo*) malloc(sizeof(struct modelo));

  mod->posInitVBO=0;

  int counterP = 0;
  int numPoints = 0;
  int validArgs = 0;

  bool erro = false;

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename,"r");
  if (fp) {

    // parse da primeira linha que contem os numeros dos pontos
    if ((read = getline(&line, &len, fp)) != -1) { 
      validArgs = sscanf(line, "%d", &numPoints);

      if (validArgs!=1){
        printf("Number of lines not defined!!\n");
        erro = true;
      }
      free(line);
      line=NULL;
    }

    if (!erro){

      mod->posInitVBO=preVBO.size()/3; // num pontos
    
      while ((counterP < numPoints) && (read = getline(&line, &len, fp)) != -1) {

        float x=0.0,y=0.0,z=0.0;
        validArgs = sscanf(line,"%f %f %f",&x,&y,&z);

        preVBO.push_back(x);
        preVBO.push_back(y);
        preVBO.push_back(z);

        if (validArgs!=3) {
          printf("Invalid point!! on line: %d (of %d)\n",counterP+1, numPoints+1);
          erro = true;
          break;
        }
        else {
          ++counterP;
          free(line);
          line=NULL;
        }
      }
    }
  }
  else{
    // não abriu o ficheiro
    cout << "File \"" << filename << "\" not found " << endl;
    return 0;
  }

  fclose(fp);

  mod->numPoints = numPoints;

  mod->diffR=diffR;
  mod->diffG=diffG;
  mod->diffB=diffB;

  //printf("PosInitVBO =%d, numPontos =%d\n", mod->posInitVBO, mod->numPoints);

  modelos.push_back(mod);

  return 1;
}

void SysState::parserXML(TiXmlElement *element){

    //if para verificar que filho é
        //primeiro verificar se n é nulo
        while (element){

          //caso model
          if(!strcmp(element->Value(), "model")){

                char *name3D = (char *) element->Attribute("file");

                float diffR=1,diffG=1,diffB=1;

                element->QueryFloatAttribute("diffR",&diffR);
                element->QueryFloatAttribute("diffG",&diffG);
                element->QueryFloatAttribute("diffB",&diffB);
        
                if(read3D(name3D,diffR,diffG,diffB)){
                  sequencia.push_back(3);
                }
            }

          //caso translate
          if(!strcmp(element->Value(), "translate")){
            Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
            geo->x=0;
            geo->y=0;
            geo->z=0;

            geo->tipo = 0;

            element->QueryFloatAttribute("X",&geo->x);
            element->QueryFloatAttribute("Y",&geo->y);
            element->QueryFloatAttribute("Z",&geo->z);

            geo_tr.push_back(geo);

            sequencia.push_back(2);
          }

          //caso rotate
          if(!strcmp(element->Value(), "rotate")){
            Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
            geo->x=0;
            geo->y=0;
            geo->z=0;

            geo->tipo = 1;

            element->QueryFloatAttribute("angle",&geo->angle);
            element->QueryFloatAttribute("axisX",&geo->x);
            element->QueryFloatAttribute("axisY",&geo->y);
            element->QueryFloatAttribute("axisZ",&geo->z);

            geo_tr.push_back(geo);

            sequencia.push_back(2);
          }

          //caso scale
          if(!strcmp(element->Value(), "scale")){

            Geo_Transf *geo =(struct geo_transf*) malloc(sizeof(struct geo_transf));
            geo->x=0;
            geo->y=0;
            geo->z=0;

            geo->tipo = 2;

            element->QueryFloatAttribute("X",&geo->x);
            element->QueryFloatAttribute("Y",&geo->y);
            element->QueryFloatAttribute("Z",&geo->z);

            geo_tr.push_back(geo);

            sequencia.push_back(2);
          }

          //caso group
          if(!strcmp(element->Value(), "group")){
            sequencia.push_back(0);

            parserXML(element->FirstChildElement());

            sequencia.push_back(1);
          }

          //pode n estar correto 
          if(!strcmp(element->Value(), "models")){

            parserXML(element->FirstChildElement());

          }

          //proximo filho

          //element != nullptr;

          if(element){
              element = element->NextSiblingElement();
          }
        }
}

void SysState::readXML(char *pFilename){
// abrir ficheiro xml
    TiXmlDocument doc( pFilename );
    bool loadOkay = doc.LoadFile();

    if (loadOkay) {
        TiXmlElement *l_pRootElement = doc.RootElement();

        TiXmlElement *element = l_pRootElement->FirstChildElement("group");

        if (element){
            parserXML(element->FirstChildElement());
        }

        //procuramos o proximo filho 
        element = element->FirstChildElement();  
    }
}

void SysState::renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluLookAt(sin(angleAlfa)*cos(angleBeta)*distanciaCamera,sin(angleBeta)*distanciaCamera,cos(angleAlfa)*(cos(angleBeta)*distanciaCamera),
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

// put the geometric transformations here
    
    //glTranslatef(x_x,0,z);
    //glRotatef(angle,0,1,0);
    //glScalef(scale,scale,scale);
 
// put drawing instructions here

    writeSeq();

    // End of frame
    glutSwapBuffers();
}

void SysState::writeSeq(){
  int numGeo = 0;
  int numModelo = 0;

  //percorrer vector sequencia
  for(int i=0;i<sequencia.size();i++){
    //verificar caso
    switch (sequencia[i]) {
      case 0:
        glPushMatrix();
        break;
      case 1:
        glPopMatrix();
        break;
      case 2:
        if(numGeo<geo_tr.size()) writeGeoTransf(geo_tr[numGeo++]);
        break;
      case 3:
        if(numModelo<modelos.size()) writeModelo3D(modelos[numModelo++]);
        break;
    }
  }
}