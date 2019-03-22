// (parts of) most code taken from:
// https://profs.info.uaic.ro/~ghirvu/gpc/tema5/t5p1.cpp
// https://profs.info.uaic.ro/~ghirvu/gpc/tema5/t5p3.cpp

#define _CRT_SECURE_NO_WARNINGS

#define dim 500
// max nr of iterations for testing apartenence to Mandelbrot set
#define NRITER_M 5000
// max mod for testing apartenence to Mandelbrot set
#define MODMAX_M 10000000
// raions for CMandelbrot
#define RX_M 0.01
#define RY_M 0.01

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include <GL/glut.h>

unsigned char prevKey;
int nivel = 0;

class CComplex {
public:
  CComplex() : re(0.0), im(0.0) {}
  CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
  CComplex(const CComplex &c) : re(c.re), im(c.im) {}
  ~CComplex() {}

  CComplex &operator=(const CComplex &c) 
  {
    re = c.re;
    im = c.im;
    return *this;
  }

  double getRe() {return re;}
  void setRe(double re1) {re = re1;}
  
  double getIm() {return im;}
  void setIm(double im1) {im = im1;}

  double getModul() {return sqrt(re * re + im * im);}

  int operator==(CComplex &c1)
  {
    return ((re == c1.re) && (im == c1.im));
  }

  int operator>(CComplex &c2){
  if ((re==c2.re && im>c2.im) || re>c2.re){
    return 1;
  }
  else
    return 0;
}

  CComplex pow2() 
  {
    CComplex rez;
    rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
    rez.im = 2.0 * re * im;
    return rez;
  }

  friend CComplex operator+(const CComplex &c1, const CComplex &c2);
  friend CComplex operator*(CComplex &c1, CComplex &c2);

  void print(FILE *f) 
  {
    fprintf(f, "%.20f%+.20f i", re, im);
  }

private:
  double re, im;
};

CComplex operator+(const CComplex &c1, const CComplex &c2) 
{
  CComplex rez(c1.re + c2.re, c1.im + c2.im);
  return rez;
}

CComplex operator*(CComplex &c1, CComplex &c2) 
{
  CComplex rez(c1.re * c2.re - c1.im * c2.im,
                c1.re * c2.im + c1.im * c2.re);
  return rez;
}

class CMandelbrot{
public:
  CMandelbrot(){
    m.nriter = NRITER_M;
    m.modmax = MODMAX_M;
  }
  CMandelbrot(CComplex &c){
    m.c = c;
    m.nriter = NRITER_M;
    int rez=0;
    // table for the iterative process z_n+1 = z_n * z_n + c;
    CComplex z0(0,0);
    CComplex z1;
    m.modmax = MODMAX_M;
  }
  ~CMandelbrot(){}

  void setc(CComplex v){m.c=v;}

  void setmodmax(double v){assert(v <= MODMAX_M); m.modmax = v;}
  double getmax(){return m.modmax;}

  void setnriter(int v){assert(v <= NRITER_M); m.nriter = v;}
  int getnriter(){return m.nriter;}

  // test if x is in Mandelbrot set
  // returns 0 if does, else it returns the nr. of iterations;
  int isIn(CComplex &x){
    int rez=0;
    // table for the iterative process z_n+1 = z_n * z_n + c;
    CComplex z0(0,0);
    CComplex z1;
    
    for (int i = 1; i < m.nriter; i++){
      z1 = z0*z0 + x;

      if (z1.getModul() > 2){
        rez = i;
        fprintf(stdout, "Iteration %d: %d + %di\n",i,z1.getRe(),z1.getIm());
        break;
      }
      /*else if (z1.getModul() > m.modmax){
        rez = i;
        break;
      }*/
      z0 = z1;
    }
    return rez;
  }

  void display(double xmin, double ymin, double xmax, double ymax)
  {
    glPushMatrix();
    glLoadIdentity();

   glTranslated((xmin + xmax) * 1.0 / (xmin - xmax), (ymin + ymax)  * 1.0 / (ymin - ymax), 0);
   glScaled(1.0 / (xmax - xmin), 1.0 / (ymax - ymin), 1);
    // display
    glBegin(GL_POINTS);
    for(double x = xmin; x <= xmax; x+=RX_M)
      for(double y = ymin; y <= ymax; y+=RY_M) 
      {
        CComplex z(x, y);
        int r = isIn(z);
//        z.print(stdout);
        if (r != 0) 
        {
          glColor3f((r%3)/3.0,(r%5)/5.0,(r%2)/2.0);
          glVertex3d(x,y,0);
        }
      }
    fprintf(stdout, "STOP\n");
    glEnd();

    glPopMatrix();
  }

private:
  struct SDate
  {
    CComplex c;
    // nr of iterations;
    int nriter;
    double modmax;
  } m;
};

// points that are not part of Mandelbrot set for c = -2-2i
void Display1() {

  CMandelbrot cm;

  cm.setnriter(30);
  cm.display(-2, -2, 2, 2);
}

// points that are not part of Mandelbrot set for c = -0.012+0.74i

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
  switch(prevKey) {
  case '1':
    glClear(GL_COLOR_BUFFER_BIT);
    Display1();
    break;
 
  default:
    break;
  }

  glFlush();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}
