#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

#define POINTS 100
#define SCALE (1.0/10000.0)
double points[POINTS][2];
double velocity[POINTS][2];

double x=0.667,y=0.5;
double xmin,ymin,xmax,ymax;
int w=500,h=500;

int acc=1;
void step()
{
	int i;
	for (i = 0; i < POINTS; i++) {
		//random acceleration
		if (acc) {
			double theta = rand()/(double)RAND_MAX*2.0*M_PI;
			velocity[i][0]+=cos(theta)*SCALE;
			velocity[i][1]+=sin(theta)*SCALE;
		}
		//gravity
		velocity[i][0]-=(points[i][0]-0.5)*0.0001;
		velocity[i][1]-=(points[i][1]-0.5)*0.0001;
		//change position
		points[i][0]+=velocity[i][0];
		points[i][1]+=velocity[i][1];
		
	}
}
int clear = 3;
void drawString(char* s)
{
	int k;
	for(k=0;k<strlen(s);k++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[k]);
}
void display(void)
{
	double t;
	int i;
	if (clear) {
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearAccum(0.0,0.0,0.0,0.0);
		glClear(GL_ACCUM_BUFFER_BIT);
//		clear--;
	}
	glColor3f(1.0,1.0,1.0);
	glReadBuffer(GL_FRONT);
	glAccum(GL_ACCUM,0.9);
	glPointSize(2.0);
	glReadBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (i = 0; i < POINTS; i++) {
		//glColor3f(0.3*exp(0.1/SCALE*sqrt(velocity[i][0]*velocity[i][0]+velocity[i][1]*velocity[i][1])),0.0,0.0);
		glVertex2f(points[i][0],points[i][1]);
	}
	glEnd();
	glAccum(GL_ACCUM,1.0);
//	glClear(GL_COLOR_BUFFER_BIT);
	glAccum(GL_RETURN,1.0);
	//glFlush(); // single buffering, for double use glutSwapBuffers();
	glutSwapBuffers();
}
void mouse(int button,int state,int xscr,int yscr)
{
	if(button==GLUT_LEFT_BUTTON)
		if(state==GLUT_DOWN)
		{
			x=xmin+(xmax-xmin)*(  xscr)/w;
			y=ymin+(ymax-ymin)*(h-yscr)/h;

			glutPostRedisplay(); // callback
		}
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
	if(key=='q') {
		exit(0);
	}
	if(key==' ') {
		if (acc)
			acc=0;
		else
			acc=1;
	}
}
void reshape(int wscr,int hscr)
{
	w=wscr; h=hscr;
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	xmin=ymin=0.0; xmax=ymax=1.0;
	if(w<=h)
		ymax=1.0*(GLfloat)h/(GLfloat)w;
	else
		xmax=1.0*(GLfloat)w/(GLfloat)h;

	gluOrtho2D(xmin,xmax,ymin,ymax);
	glMatrixMode(GL_MODELVIEW);
}
void idle() {
	usleep(5000);
	step();
	glutPostRedisplay();
}

int main(int argc,char* argv[])
{ 
	srand(0);
	int i = 0;
	for (i = 0; i < POINTS; i++) {
		points[i][0] = 0.5;
		points[i][1] = 0.5;
		velocity[i][0] = 0.0;
		velocity[i][1] = 0.0;
	}
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
	glutInitWindowSize(w,h);					 // for double use GLUT_DOUBLE
	glutInitWindowPosition(100,50);
	glutCreateWindow("OpenGL Demo");

	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glutDisplayFunc(display);		// register callback functions
	glutIdleFunc(idle);				// no animation
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyfunc);
	glutReshapeFunc(reshape);

	glutMainLoop();					// here we go

	return 0;
}
