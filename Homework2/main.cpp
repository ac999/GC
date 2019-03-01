#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
   double xmax, ymax, xmin, ymin;
   double a = 1, b = 2;
   double pi = 4 * atan(1);
   double ratia = 0.05;

   // calculul valorilor maxime/minime ptr. x si y
   // aceste valori vor fi folosite ulterior la scalare
   xmax = a - b - 1;
   xmin = a + b + 1;
   ymax = ymin = 0;
   for (double t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = a + b * cos(t);
      xmax = (xmax < x1) ? x1 : xmax;
      xmin = (xmin > x1) ? x1 : xmin;

      x2 = a - b * cos(t);
      xmax = (xmax < x2) ? x2 : xmax;
      xmin = (xmin > x2) ? x2 : xmin;

      y1 = a * tan(t) + b * sin(t);
      ymax = (ymax < y1) ? y1 : ymax;
      ymin = (ymin > y1) ? y1 : ymin;

      y2 = a * tan(t) - b * sin(t);
      ymax = (ymax < y2) ? y2 : ymax;
      ymin = (ymin > y2) ? y2 : ymin;
   }

   xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
   ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(0.1,0.1,1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();

   glBegin(GL_LINE_STRIP); 
   for (double t = - pi/2 + ratia; t < pi / 2; t += ratia) {
      double x1, y1, x2, y2;
      x1 = (a + b * cos(t)) / xmax;
      x2 = (a - b * cos(t)) / xmax;
      y1 = (a * tan(t) + b * sin(t)) / ymax;
      y2 = (a * tan(t) - b * sin(t)) / ymax;

      glVertex2f(x2,y2);
   }
   glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
   double pi = 4 * atan(1);
   double xmax = 8 * pi;
   double ymax = exp(1.1);
   double ratia = 0.05;

   // afisarea punctelor propriu-zise precedata de scalare
   glColor3f(0.1,0.1,1); // rosu
   glBegin(GL_LINE_STRIP); 
   for (double x = 0; x < xmax; x += ratia) {
      double x1, y1;
      x1 = x / xmax;
      y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

      glVertex2f(x1,y1);
   }
   glEnd();
}


//y same; distance = sqrt(pow((x1-x2),2));
void Display3(){
    double ratio=0.05;
    double x,y;
    x=y=0;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0,1);
    for (x=ratio;x<=100;x+=ratio){
	y=sqrt(pow(x-round(x),2))/x;
	glVertex2d(x/25,y);
    }
    glEnd();
}

void Display4(double a, double b){
    double pi=4*atan(1);
    double ratio=0.05;

    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
    for (double t=-pi+ratio;t<pi;t+=ratio){
	double x1=2*(a*cos(t)+b)*cos(t);
	double y1=2*(a*cos(t)+b)*sin(t);

	glVertex2f(x1,y1);
    }
    glEnd();
}

double LCx(double a, double t){
    return (a/(4*0.5*(cos(2*t)+1)-3));
}

double LCy(double a, double t){
    return ((a*tan(t))/(4*0.5*(cos(2*t)+1)-3));
}

void Display5(double a){
    double pi=4*atan(1);
    double ratio=0.05;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
    double pi6=pi/6;double mpi6=-pi6;
    for (double t=-pi/2+ratio;t<pi/2;t+=ratio){
	if (t!=mpi6&&t!=pi6){
	    glVertex2f(LCx(a,t),LCy(a,t));
	}
    }
    /*
    for (double t=mpi6+ratio;t<pi6;t+=ratio){
    
	glVertex2f(LCx(a,t),LCy(a,t));
    }

    for (double t=pi6+ratio;t<pi/2;t+=ratio){

	glVertex2f(LCx(a,t),LCy(a,t));
    }
    */
    glEnd();

}

void Display6(double a, double b){
	
	double ratio=0.05;
	double x,y;
	x=y=0;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
	for (double t=-10;t<=10;t+=ratio){
		x=a*t-b*sin(t);
		y=a-b*cos(t);
		glVertex2f(x,y);
	}
	glEnd();
}

void Display7(double R, double r){
	double ratio=0.05;
	double pi2=atan(1)*8;
	double x,y;	x=y=0;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
	for(double t=0;t<=pi2;t+=ratio){
		x=(R+r)*cos(r/R*t)-r*cos(t+r/R*t);
		y=(R+r)*sin(r/R*t)-r*sin(t+r/R*t);
		glVertex2f(x,y);
	}
	glEnd();	
}

void Display8(double R, double r){
	double ratio=0.05;
	double pi2=atan(1)*8;
	double x,y;	x=y=0;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
	for (double t=0;t<=pi2;t+=ratio){
		x=(R-r)*cos(r/R*t)-r*cos(t-r/R*t);
		y=(R-r)*sin(r/R*t)-r*sin(t-r/R*t);
		glVertex2f(x,y);
	}
	glEnd();
}

double TransformX(double r, double t){
	return r*cos(t);
}

double TransformY(double r, double t){
	return r*sin(t);
}

void Display9(double a){
	double ratio=0.01;
	double r,t; r=t=0;
	double pi4=atan(1);

    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
	for (t=-pi4+ratio;t<pi4;t+=ratio){
		r=a*sqrt(2*cos(2*t));
		glVertex2f(TransformX(r,t),TransformY(r,t));
	}
	glEnd();

    glBegin(GL_LINE_STRIP);
	glVertex2f(TransformX(a*sqrt(2*cos(2*(ratio-pi4))),-pi4),TransformY(a*sqrt(2*cos(2*(ratio-pi4))),-pi4));
	for (t=-pi4+ratio;t<pi4;t+=ratio){
		r=-a*sqrt(2*cos(2*t));
		glVertex2f(TransformX(r,t),TransformY(r,t));
	}
	glEnd();
}

void Display10(double a){
	double r,t; r=t=0;
	double ratio=0.05;
    glColor3f(0.1,0.1,1);
    glBegin(GL_LINE_STRIP);
	for (t=ratio;t<=99;t+=ratio){
		r=a*exp(1+t);
		glVertex2f(TransformX(r,t),TransformY(r,t));
	}
	glEnd();
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

//   glPointSize(4);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
   glClear(GL_COLOR_BUFFER_BIT);

   switch(prevKey) {
   case '1':
      Display1();
      break;
   case '2':
      Display2();
      break;
   case '3':
      Display3();
      break;
   case '4':
      Display4(0.3,0.2);
      break;
   case '5':
      Display5(0.2);
      break;
   case '6':
      Display6(0.1,0.2);
      break;
   case '7':
      Display7(0.1,0.3);
      break;
   case '8':
      Display8(0.1,0.3);
      break;
   case '9':
      Display9(0.4);
      break;
	case '0':
	  Display10(0.02);
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
