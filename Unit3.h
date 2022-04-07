//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
#include "Unit2.h"
#include "Unit1.h"
struct face
{
  double f[3][4];
  double A;
  double B;
  double C;
  double D;
  bool toPrint;
};

class faceCont
{
 private:
 face* ob;
 int search(MyPoint* o, face& f, int d);
 public:
 faceCont(void);
 void init(double** pr);
 void countCoeff(void);
 void isPrint(double x, double y, double z, double d);
 void print(TImage* Image1, MyPoint* ob, int d);


};

//---------------------------------------------------------------------------
#endif
 