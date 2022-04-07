//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit3.h"

double getDet(double** m)
{
return (m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[0][2]*m[1][0]*m[2][1])-(m[0][2]*m[1][1]*m[2][0]+m[0][0]*m[1][2]*m[2][1]+m[2][2]*m[1][0]*m[0][1]);
}

faceCont::faceCont(void)
{
ob=new face[5];
for(int i=0;i<5;i++)
{
 /*for(int j=0;j<3;j++)
 {ob[i].f[j]=0;} */
 ob[i].A=ob[i].B=ob[i].C=ob[i].D=0.0;
 ob[i].toPrint=false;
}
}

void faceCont::init(double** pr)
{  //汤蜒嚷 闲闻手热
for(int k=0;k<2;k++)
{
for(int i=0;i<3;i++)
{
for(int j=0;j<3;j++)
{
  ob[k].f[i][j]=pr[i+k*3][j];
}
}
}

for(int k=2;k<5;k++)
{
 for(int j=0;j<4;j++)
 {ob[k].f[0][j]=pr[k-2][j];}
 for(int i=1;i<3;i++)
 {
 for(int j=0;j<4;j++)
 {if(k==4&&i==2)
  {ob[k].f[i][j]=pr[3][j];}
 else
 {ob[k].f[i][j]=pr[k+i][j];}
 }
 }
 }
}

void faceCont::countCoeff(void)
{
double mDet;
double** matr1=new double*[3];
double** matr2=new double*[3];
double** matr3=new double*[3];
double** matr4=new double*[3];
for(int i=0;i<3;i++)
{
 matr1[i]=new double[3];
 matr2[i]=new double[3];
 matr3[i]=new double[3];
 matr4[i]=new double[3];
}
for(int k=0;k<5;k++)
{
for(int i=0;i<3;i++)
{
 for(int j=0;j<3;j++)
 {
 matr1[i][j]=ob[k].f[i][j];
 matr2[i][j]=ob[k].f[i][j];
 matr3[i][j]=ob[k].f[i][j];
 matr4[i][j]=ob[k].f[i][j];
 }
}
for(int i=0;i<3;i++)
{
 matr2[i][0]=-1;
 matr3[i][1]=-1;
 matr4[i][2]=-1;
}
if((mDet=getDet(matr1))==0)
{ continue;}
ob[k].A=getDet(matr2)/mDet;
ob[k].B=getDet(matr3)/mDet;
ob[k].C=getDet(matr4)/mDet;
ob[k].D=1;

}
}

void faceCont::isPrint(double x, double y, double z, double d)
{
for(int i=0;i<5;i++)
{
if(ob[i].A*x+ob[i].B*y+ob[i].C*z+ob[i].D*d>=0)
{
ob[i].toPrint=false;
}
else
{
ob[i].toPrint=true;
}
//int r=0;
}
}

void faceCont::print(TImage* Image1, MyPoint* pList, int d)
{
 int oi=0;
 for(int i=0;i<5;i++)
 {
 if(ob[i].toPrint)
 {
 oi=search(pList,ob[i], d);
 Image1->Canvas->MoveTo(ob[i].f[1][0]+d,ob[i].f[1][1]+d);
 Image1->Canvas->LineTo(ob[i].f[0][0]+d,ob[i].f[0][1]+d);
 Image1->Canvas->MoveTo(ob[i].f[1][0]+d,ob[i].f[1][1]+d);
 Image1->Canvas->LineTo(ob[i].f[2][0]+d,ob[i].f[2][1]+d);
 if(oi!=-1)
 {Image1->Canvas->LineTo(pList[oi].get_x()+d,pList[oi].get_y()+d);}
 //抢市姥世
 }
 }

/*
bool out_f=true;
TColor* line;
for(int j=0; j<441; j++)
{
int f_pix=-1;
line=(TColor*)Image1->Picture->Bitmap->ScanLine[j];
for(int i=0; i<537; i++)
{
   if((line[i]==clBlack)&&(f_pix==-1))
   {
    f_pix=i;
   }
   else if((f_pix!=-1)&&(line[i]==clBlack)&&(line[i-1]!=clBlack))
   {
        Image1->Canvas->Pen->Color=clRed;
        Image1->Canvas->MoveTo(i-1,j);
        Image1->Canvas->LineTo(f_pix, j);
        Image1->Canvas->Pen->Color=clBlack;
      break;
   }
}
}*/
}

int faceCont::search(MyPoint* o, face& flat, int d)
{
  for(int i=0;i<6;i++)
  {
  double a=o[i].get_x()*flat.A+o[i].get_y()*flat.B+o[i].get_z()*flat.C+flat.D;
  if((a<0.1&&a>-0.1)&&(o[i].get_x()!=)
  {return i;}
  }
  return -1;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)


