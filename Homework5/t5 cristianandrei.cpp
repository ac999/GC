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

// exercise 2, Mandelbrot set;
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
        fprintf(stdout, "Mandelbrot#Iteration %d: %d + %di\n",i,z1.getRe(),z1.getIm());
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

// exercise 4:

unsigned char prevKey;
int level = 0;

class C2coord
{
public:
  C2coord() 
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y) 
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p) 
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPoint : public C2coord
{
public:
  CPoint() : C2coord(0.0, 0.0)
  {}

  CPoint(double x, double y) : C2coord(x, y)
  {}

  CPoint &operator=(const CPoint &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void setxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  int operator==(CPoint &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void mark()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }
  
  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)", m.x, m.y);
  }

};

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalize();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalize();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPoint getDest(CPoint &orig, double length)
  {
    double x, y;
    orig.setxy(x, y);
    CPoint p(x + m.x * length, y + m.y * length);
    return p;
  }

  void rotation(double degrees)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1.0)) * degrees / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalize();
  }

  void draw(CPoint p, double length) 
  {
    double x, y;
    p.setxy(x, y);
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * length, y + m.y * length);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalize()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0) 
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};

class SquaresFractal
  {
public:
  void segmentSquare(double length, int level, CPoint &p, CVector v)
  {
    CPoint p1;
    if (level == 0) 
    {
      v.draw(p, length);
    }
    else
    {
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentSquare(length/3.0 , level - 1, p, v);
    
      p1 = v.getDest(p,length/3.0);
      v.rotation(-90);

      segmentSquare(length/3.0, level - 1, p1, v);

    }
  }

  void display(double length, int level)
  {
    CVector v1(-1.0, 0.0);
    CPoint p1(0.5, 0.5);

    CVector v2(1.0, 0.0);
    CPoint p2(-0.5,-0.5);

    CVector v3(0.0,1.0);
    CPoint p3(0.5,-0.5);

    CVector v4(0.0,-1.0);
    CPoint p4(-0.5,0.5);

    segmentSquare(length, level, p1, v1);
    segmentSquare(length, level, p2, v2);
    segmentSquare(length, level, p3, v3);
    segmentSquare(length, level, p4, v4);
  }
};


class CTree
{
public:
  void arborePerron(double length, 
                    int level, 
                    double factordiviziune, 
                    CPoint p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPoint p1, p2, p3;
    if (level == 0) 
    {
    }
    else
    {
      v.rotation(-45);
      v.draw(p, length);
      p1 = v.getDest(p, length);
      arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);

      v.rotation(90);
      v.draw(p, length);
      p1 = v.getDest(p, length);
      p2 = p1;


      v.rotation(-45);
      v.draw(p1, length);
      p1 = v.getDest(p1, length);
      p3 = p1;
      // arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);
      v.rotation(-90);
      v.draw(p1, length/2);
      p1=v.getDest(p3,length/2);
      arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);
      v.rotation(90);
      v.rotation(45);
      v.draw(p3, length/2);
      p1=v.getDest(p3,length/2);
      arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);


      v.rotation(-45);


      p1 = p2;
      v.rotation(75);
      v.draw(p1, length);
      p1 = v.getDest(p1, length);
      p2 = p1;
      arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);

      


      // v.rotation(-45);
      // v.draw(p3, length);
      // p1=v.getDest(p1, length);

      // v.rotation(45);
      // v.draw(p1, length/2.0);
      // p1 = v.getDest(p1, length/2.0);
      // arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);

      // p1 = p2;
      // v.rotation(-90);
      // v.draw(p1, length/2.0);
      // p1 = v.getDest(p1, length/2.0);
      // arborePerron(length * factordiviziune, level - 1, factordiviziune, p1, v);
    }
  }

  void display(double length, int level)
  {
    CVector v(0.0, -1.0);
    CPoint p(0.5, 2.0);

    v.draw(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(length, level, 0.4, p, v);
  }
};


// points that are not part of Mandelbrot set
void Display1() {

  CMandelbrot cm;

  cm.setnriter(30);
  cm.display(-2, -2, 2, 2);
}

// picture 1
void Display2() {
  SquaresFractal sqf;
  sqf.display(1, level);

  char c[3];
  sprintf(c, "%2d", level);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '1');

  level++;
}

void Display3() {
  CTree ct;

  char c[3];
  sprintf(c, "%2d", level);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '2');

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  ct.display(0.7, level);
  glPopMatrix();
  level++;
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
  switch(prevKey) {
  case '0':
    glClear(GL_COLOR_BUFFER_BIT);
    level = 0;
    fprintf(stderr, "ex4#level = %d\n", level);
    break;
  case '1':
    glClear(GL_COLOR_BUFFER_BIT);
    Display1();
    break;
  case '2':
    glClear(GL_COLOR_BUFFER_BIT);
    Display2();
    break;
  case '3':
    glClear(GL_COLOR_BUFFER_BIT);
    Display3();
    break;
  // case '4':
  //   glClear(GL_COLOR_BUFFER_BIT);
  //   Display4();
  //   break;
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
