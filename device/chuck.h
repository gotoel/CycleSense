#ifndef CHUCK_H
#define CHUCK_H

struct ChuckData {
  int acclX;
  int acclY;
  int acclZ;
  int axisX;
  int axisY;
  bool buttonC;
  bool buttonZ;
};

class Chuck {
  public:
    void initialize();
    void process(); 
  private:
    ChuckData currentData;
    ChuckData lastData;
  
    ChuckData collectData();
    void debugPrint();
};


#endif