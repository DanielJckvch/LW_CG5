//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//#include "Unit3.h"
#include <math.h>
//#include <Graphics.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//Призма. Поворот и перенос вокруг всех осей, масштабирование, закраска и удаление невидимых поверхностей
struct face
{
  double* f[4];
  double A;
  double B;
  double C;
  double D;
  bool toPrint;
  bool trian;
};
void print(MyPoint* o, TImage* Image1);
void rotandscale(MyPoint* o, bool sw, bool sign);
void prisminit(MyPoint* o);
void bresline(TColor*, int, int, int, int, int);
void fillFaceList(void);
void countCoeffs(void);

MyPoint* prism=new MyPoint[6];
face* samePrism=new face[6];
double prismProj[6][4];

TColor* buff;
BITMAPINFO info;

double h=50.0;
int d=200;
double z_plane=100.0;
double z=500.0;

double a_step=10;
int sc_step=2;
int mov_step=10;

int axis_mode=0;
int proj_mode=0;

bool f1=1;
bool f2=1;
bool f3=1;
bool f4=1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Form1Create(TObject *Sender)
{
//Инициализация и вывод шестиугольнка
prisminit(prism);

TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);

//BITMAPINFOHEADER info;

//setmem(&info,sizeof(info),0);
info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER );
info.bmiHeader.biBitCount=0;
info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
/*
info.bmiHeader.biHeight=Image1->Height;
info.bmiHeader.biWidth=Image1->Width;
info.bmiHeader.biPlanes = 1;
info.bmiHeader.biBitCount = 32;
info.bmiHeader.biCompression = BI_RGB;
info.bmiHeader.biSizeImage = 0;
info.bmiHeader.biXPelsPerMeter = 0;
info.bmiHeader.biYPelsPerMeter = 0;
 info.bmiHeader.biClrUsed = 3;
info.bmiHeader.biClrImportant = 0;
info.bmiColors[0].rgbRed=(BYTE)clRed;
info.bmiColors[0].rgbGreen=(BYTE)(clGreen>>8);
info.bmiColors[0].rgbBlue=(BYTE)(clBlue>>16);
*/
//GetObject(Image1->Picture->Bitmap, sizeof(info), &info);
//info.bmiHeader.biHeight=-Image1->Picture->Bitmap->Height;
int ret=GetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Picture->Bitmap->Height,NULL,(BITMAPINFO*)&info,DIB_RGB_COLORS);
buff=new TColor[info.bmiHeader.biHeight*info.bmiHeader.biWidth];
info.bmiHeader.biCompression=BI_RGB;
info.bmiHeader.biHeight*=-1;
int ret1=GetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Picture->Bitmap->Height,buff,(BITMAPINFO*)&info,DIB_RGB_COLORS);
//info.bmiHeader.biCompression=BI_RGB;
//info.bmiHeader.biHeight*=-1;
//bresline(buff,Image1->Width,100,100,400,100);
print(prism, Image1);
 /*
for(int i=0;i<Image1->Width;i++)
{
 bool f=false;
 for(int j=0;j<Image1->Height;j++)
 {
   if(buff[j*Image1->Width+i]==clBlack)
   {
   f=true;
   }
   else if(f)
   {
   buff[j*Image1->Width+i]=0x00FFFF00;
   }

 }

}
*/



Edit1->Text="X";
Edit2->Text="Isometric";

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
//Поворот против часовой стрелки
rotandscale(prism, 0, 1);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
//Поворот по часовой стрелке
rotandscale(prism, 0, 0);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
//Масштаб -
rotandscale(prism, 1, 1);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
//Масштаб +
rotandscale(prism, 1, 0);
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
 //Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
//Сдвиг против оси
switch(axis_mode)
{
case 0:
for(int i=0; i<6; i++)
{
 prism[i].set_x(prism[i].get_x()-mov_step);
}
break;
case 1:
for(int i=0; i<6; i++)
{
 prism[i].set_y(prism[i].get_y()-mov_step);
}
break;
case 2:
for(int i=0; i<6; i++)
{
 prism[i].set_z(prism[i].get_z()-mov_step);
}
break;
}

//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
//Сдвиг по оси
switch(axis_mode)
{
case 0:
for(int i=0; i<6; i++)
{
 prism[i].set_x(prism[i].get_x()+mov_step);
}
break;
case 1:
for(int i=0; i<6; i++)
{
 prism[i].set_y(prism[i].get_y()+mov_step);
}
break;
case 2:
for(int i=0; i<6; i++)
{
 prism[i].set_z(prism[i].get_z()+mov_step);
}
break;
}
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
//Выбор оси
if(f1)
{
        if(f2)
        {
        axis_mode=1;
        f2=0;
        Edit1->Text="Y";
        }
        else
        {
        axis_mode=2;
        f1=0;
        f2=1;
        Edit1->Text="Z";
        }
}
else
{
axis_mode=0;
f1=1;
Edit1->Text="X";
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
//Выбор проекции
if(f3)
{
        if(f4)
        {
        proj_mode=1;
        f4=0;
        Edit2->Text="Cavalier";
        }
        else
        {
        proj_mode=2;
        f3=0;
        f4=1;
        Edit2->Text="Perspective";
        }
}
else
{
proj_mode=0;
f3=1;
Edit2->Text="Isometric";
}
//Очистка холста
TRect rct;
rct = Rect(0,0,Image1->Width,Image1->Height);
Image1->Canvas->Brush->Style=bsSolid;
Image1->Canvas->FillRect(rct);
//Отрисовка
print(prism, Image1);
}
//---------------------------------------------------------------------------

void print(MyPoint* o, TImage* Image1)
{
int k;
double pi = 3.1415926;
double a; //x
double b; //y
double proj[4][4];
double per[4][4];
per[0][0]=1;per[0][1]=0;per[0][2]=0;per[0][3]=0;
per[1][0]=0;per[1][1]=1;per[1][2]=0;per[1][3]=0;
per[2][0]=0;per[2][1]=0;per[2][2]=1;per[2][3]=-z_plane;
per[3][0]=0;per[3][1]=0;per[3][2]=0;per[3][3]=1;
switch(proj_mode)
{
case 0:
a=45.0*pi/180; //x
b=35.26*pi/180; //y
proj[0][0]=cos(a);proj[0][1]=sin(a)*cos(b);proj[0][2]=sin(a)*sin(b);proj[0][3]=0;
proj[1][0]=-sin(a);proj[1][1]=cos(a)*cos(b);proj[1][2]=sin(b)*cos(a);proj[1][3]=0;
proj[2][0]=0;proj[2][1]=-sin(b);proj[2][2]=cos(b);proj[2][3]=0;
proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
break;
case 1:
a=45.0*pi/180; //x
//b=35.26*pi/180; //y
proj[0][0]=1;proj[0][1]=0;proj[0][2]=0;proj[0][3]=0;
proj[1][0]=0;proj[1][1]=1;proj[1][2]=0;proj[1][3]=0;
proj[2][0]=-cos(a);proj[2][1]=-sin(a);proj[2][2]=1;proj[2][3]=0;
proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
break;
case 2:
a=45.0*pi/180; //x
b=35.26*pi/180; //y
proj[0][0]=cos(a);proj[0][1]=sin(a)*cos(b);proj[0][2]=sin(a)*sin(b);proj[0][3]=0;
proj[1][0]=-sin(a);proj[1][1]=cos(a)*cos(b);proj[1][2]=cos(a)*sin(b);proj[1][3]=0;
proj[2][0]=0;proj[2][1]=-sin(b);proj[2][2]=cos(b);proj[2][3]=-z_plane;
proj[3][0]=0;proj[3][1]=0;proj[3][2]=0;proj[3][3]=1;
 break;
}
/*
double** sp=new double*[6];
for(int i=0; i<6;i++)
{
 sp[i]=new double[4];
 /*sp[i][0]= o[i].get_x();
sp[i][1]= o[i].get_y();
sp[i][2]= o[i].get_z();
sp[i][3]= 1; 
}*/
//samePrism.init(sp);

for(k=0;k<6;k++)
{
int i, j;

double v1[4]={o[k].get_x(),o[k].get_y(),o[k].get_z(),1};
double v2[4]={0, 0, 0, 1};

for (i = 0;i < 4;i++)
{
        double sum = 0;
        for (j = 0;j < 4;j++)
        {
                sum= sum+ (proj[j][i] * v1[j]);
        }

        v2[i] = sum;
}
if(proj_mode==2)
{
int i, j;

//double v[4]={v2[0],v2[1],v2[2],1};
//double v1[4]={0, 0, 0, 1};
for(i=0;i<2;i++)
{
per[i][i]=(z-z_plane)/(z-o[k].get_z());
}
for (i = 0;i < 4;i++)
{
        double sum = 0;
        for (j = 0;j < 4;j++)
        {
                sum= sum+ (per[j][i] * v2[j]);
        }

        v2[i] = sum;
}
}
 int a=v2[1];

prismProj[k][0]= v2[0];
prismProj[k][1]= v2[1];
prismProj[k][2]= v2[2];
prismProj[k][3]= v2[3];


}

/*
 //Проверка на выход точки за холст
v1[0]=v2[0];
v1[1]=v2[1];
if(v2[0]+hexagon[0].get_x()<=0||v2[1]+hexagon[0].get_y()<=0)
{
hexagon[0].set_x();
hexagon[0].set_y();
hexagon[1].set_x();
hexagon[1].set_y();
out_f=true;
break;
 }    */

 fillFaceList();
 countCoeffs();
 for(int i=0;i<5;i++)
 {
 if(samePrism[i].toPrint)
 {

 if(samePrism[i].trian)
 {
 bresline(buff,Image1->Width,samePrism[i].f[0][0]+d,samePrism[i].f[0][1]+d,samePrism[i].f[1][0]+d,samePrism[i].f[1][1]+d);
 bresline(buff,Image1->Width,samePrism[i].f[2][0]+d,samePrism[i].f[2][1]+d,samePrism[i].f[3][0]+d,samePrism[i].f[3][1]+d);
 bresline(buff,Image1->Width,samePrism[i].f[0][0]+d,samePrism[i].f[0][1]+d,samePrism[i].f[2][0]+d,samePrism[i].f[2][1]+d);
 bresline(buff,Image1->Width,samePrism[i].f[1][0]+d,samePrism[i].f[1][1]+d,samePrism[i].f[3][0]+d,samePrism[i].f[3][1]+d);

 }
 else
 {

 bresline(buff,Image1->Width,samePrism[i].f[0][0]+d,samePrism[i].f[0][1]+d,samePrism[i].f[1][0]+d,samePrism[i].f[1][1]+d);
 bresline(buff,Image1->Width,samePrism[i].f[1][0]+d,samePrism[i].f[1][1]+d,samePrism[i].f[2][0]+d,samePrism[i].f[2][1]+d);
 bresline(buff,Image1->Width,samePrism[i].f[2][0]+d,samePrism[i].f[2][1]+d,samePrism[i].f[0][0]+d,samePrism[i].f[0][1]+d);

 }
 samePrism[i].trian=false;
 samePrism[i].toPrint=false;
 samePrism[i].A=0.0;
 samePrism[i].B=0.0;
 samePrism[i].C=0.0;
 samePrism[i].D=0.0;
 //ЗАКРАСКА
 }
 }
 SetDIBits(Image1->Picture->Bitmap->Canvas->Handle,Image1->Picture->Bitmap->Handle,0,Image1->Height,buff,&info,DIB_RGB_COLORS);
 /*
samePrism.init(prismProj);
samePrism.countCoeff(prismProj);
samePrism.isPrint(1000,1000,1000,1.0);
samePrism.print(Image1, prism, d);
//Отрисовка контура
/*
int off_x, off_y;
for(k=0; k<6;k++)
{
Image1->Canvas->MoveTo(sp[k][0]+d,sp[k][1]+d);
if(k%3!=2)
{
Image1->Canvas->LineTo(sp[k+1][0]+d,sp[k+1][1]+d);
}
else
{
 Image1->Canvas->LineTo(sp[k-2][0]+d,sp[k-2][1]+d);
}
Image1->Canvas->TextOutA(sp[k][0]+d+1,sp[k][1]+d+3,o[k].get_let());
}
for(k=0; k<3;k++)
{
Image1->Canvas->MoveTo(sp[k][0]+d,sp[k][1]+d);
Image1->Canvas->LineTo(sp[k+3][0]+d,sp[k+3][1]+d);

}
/*for(++k; k<6; k++)
{
Image1->Canvas->MoveTo(sp[k][0]+200,sp[k][1]+200);
Image1->Canvas->LineTo(sp[k+1][0]+200,sp[k+1][1]+200);
}
Image1->Canvas->MoveTo(sp[k-1][0]+200,sp[k][k-1]+200);
Image1->Canvas->LineTo(sp[k-3][0]+200,sp[k-3][1]+200);
*/
/*
//Расставление букв точек
for(k=0; k<3; k++)
{
//Определение значения отступа для буквы
off_x=(o[k].get_x()<0)?-6:4;
off_y=(o[k].get_y()<0)?-12:2;

Image1->Canvas->MoveTo(o[k].get_x(),o[k].get_y());
Image1->Canvas->TextOutA(o[k].get_x()+off_x,o[k].get_y()+off_y,o[k].get_let());
}
 */
}

void rotandscale(MyPoint* o, bool sw, bool sign)
{
double s=sc_step;
double a=a_step;
if(sign)
{ a=a*(-1);
s=1/s;
}
double r[4][4];
// Выбор способа преобразования
if(sw)
{
r[0][0]=s; r[0][1]=0; r[0][2]=0; r[0][3]=0;
r[1][0]=0; r[1][1]=s; r[1][2]=0; r[1][3]=0;
r[2][0]=0; r[2][1]=0; r[2][2]=s; r[2][3]=0;
r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
 }
 else
 {
 switch(axis_mode)
 {
 case 0:
 r[0][0]=1; r[0][1]=0; r[0][2]=0; r[0][3]=0;
 r[1][0]=0; r[1][1]=cos(a*3.14/180); r[1][2]=sin(a*3.14/180); r[1][3]=0;
 r[2][0]=0; r[2][1]=-sin(a*3.14/180); r[2][2]=cos(a*3.14/180); r[3][3]=0;
 r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
 break;
 case 1:
 r[0][0]=cos(a*3.14/180); r[0][1]=0; r[0][2]=-sin(a*3.14/180); r[0][3]=0;
 r[1][0]=0; r[1][1]=1; r[1][2]=0; r[1][3]=0;
 r[2][0]=sin(a*3.14/180); r[2][1]=0; r[2][2]=cos(a*3.14/180); r[3][3]=0;
 r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
 break;
 case 2:
 r[0][0]=cos(a*3.14/180); r[0][1]=sin(a*3.14/180); r[0][2]=0; r[0][3]=0;
 r[1][0]=-sin(a*3.14/180); r[1][1]=cos(a*3.14/180); r[1][2]=0; r[1][3]=0;
 r[2][0]=0; r[2][1]=0; r[2][2]=1; r[3][3]=0;
 r[3][0]=0; r[3][1]=0; r[3][2]=0; r[3][3]=1;
 break;

 }
 }
 int k=0;
// Преобразование точки V
for(k=0;k<6;k++)
{
double v1[4]={o[k].get_x(),o[k].get_y(),o[k].get_z(),1};
double v2[4]={0, 0, 0, 1};
int i, j;
for (i = 0;i < 4;i++)
{
        double sum = 0;
        for (j = 0;j < 4;j++)
        {
                sum= sum+ (r[j][i] * v1[j]);
        }

        v2[i] = sum;
}
o[k].set_x(v2[0]);
o[k].set_y(v2[1]);
o[k].set_z(v2[2]);
}
}
void prisminit(MyPoint* o)
{
  o[0]=MyPoint('A',0.0,200.0,-h/2);
  o[1]=MyPoint(char('A'+1),0.0,0.0,-h/2);
  o[2]=MyPoint(char('A'+2),200.0,0.0,-h/2);
 for(int i=3; i<6; i++)
 {
 o[i]=MyPoint(char('A'+i),o[i-3].get_x(),o[i-3].get_y(),o[i-3].get_z()+h);
 }
 /*
 prismProj=new double*[6];
 for(int i=0;i<6;i++)
 {
    prismProj[i]=new double[3];
 }*/
}

 void bresline(TColor* Image, int w, int x0, int y0, int x1, int y1)
{
// ????????? ????? ?????????? ??????????
  int sx=x0<x1?1:-1;
  int dx=(x1-x0)*sx;
  int sy=y0<y1?1:-1;
  int dy=(y1-y0)*sy;
  int err1=(dx>dy?dx:-dy)/2;
  int err2=err1;
  for(;;)
  {
  Image[x0+y0*w]=clBlack;
  if(x0==x1&&y0==y1)
  {
  break;
  }
  err2=err1;
  if(err2>-dx)
  {
  err1-=dy;
  x0+=sx;
  }
  if(err2<dy)
  {
  err1+=dx;
  y0+=sy;
  }
  }
}
void fillFaceList(void)
{  //МАССИВ ПРОЕКЦИИ
for(int k=0;k<2;k++)
{
for(int i=0;i<3;i++)
{

  samePrism[k].f[i]=prismProj[i+k*3];
  double t=samePrism[k].f[i][0];
  int a=3;
}
}

for(int k=2;k<5;k++)
{
 for(int i=0;i<4;i++)
 {
  if(k==4)
  {int skip=(i==3||i==0)*((i==0)?-1:1);
  samePrism[k].f[i]=prismProj[k-3+i+skip];}
 else
 {bool skip=(i>=2)?1:0;
 samePrism[k].f[i]=prismProj[k-2+i+skip];}
 }
 samePrism[k].trian=true;
 }
}

void countCoeffs(void)
{
long int v1[3];
long int v2[3];
long int bariocenter[3];
bariocenter[0]=0;
bariocenter[1]=0;
bariocenter[2]=0;
 for(int j=0;j<6;j++)
 {
 bariocenter[0]=bariocenter[0]+prismProj[j][0];
 }
 bariocenter[0]=bariocenter[0]/6;
 for(int j=0;j<6;j++)
 {
 bariocenter[1]=bariocenter[1]+prismProj[j][1];
 }
 bariocenter[1]=bariocenter[1]/6;
 for(int j=0;j<6;j++)
 {
 bariocenter[2]=bariocenter[2]+prismProj[j][2];
 }
 bariocenter[2]=bariocenter[2]/6;
for(int i=0;i<5;i++)
{

for(int j=0;j<3;j++)
{
v1[j]=samePrism[i].f[1][j]-samePrism[i].f[0][j];
v2[j]=samePrism[i].f[2][j]-samePrism[i].f[0][j];
}
samePrism[i].A=v1[1]*v2[2]-v1[2]*v2[1];
samePrism[i].B=v2[0]*v1[2]-v1[0]*v2[2];
samePrism[i].C=v1[0]*v2[1]-v1[1]*v2[0];
samePrism[i].D=(-1*samePrism[i].A*samePrism[i].f[0][0]-samePrism[i].B*samePrism[i].f[0][1]-samePrism[i].C*samePrism[i].f[0][2]);
if(samePrism[i].A*bariocenter[0]+samePrism[i].B*bariocenter[1]+samePrism[i].C*bariocenter[2]+samePrism[i].D>0)
{
samePrism[i].A*=-1;
samePrism[i].B*=-1;
samePrism[i].C*=-1;
samePrism[i].D*=-1;
}
if(samePrism[i].C>0)
{samePrism[i].toPrint=true;}

}
 int r=3;
}

