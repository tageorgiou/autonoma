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
#define SCALE (1.0/700.0)
double points[POINTS][3];
double velocity[POINTS][3];
double acceleration[POINTS][3];
double oldpoints[POINTS][3];

double x=0.667,y=0.5;
double xmin,ymin,xmax,ymax;
int w=500,h=500;

double eyex = 0.0, eyey = 0.5, eyez = -3.0;
double centerx = 0.5, centery = 0.5, centerz = 0.5;

int acc=1;
int draw_acc=0;
void step()
{
	memcpy(oldpoints,points,POINTS*3*sizeof(double));
	int i;
	for (i = 0; i < POINTS; i++) {
		acceleration[i][0]=0;
		acceleration[i][1]=0;
		acceleration[i][2]=0;
		//random acceleration
		if (acc) {
			double theta = rand()/(double)RAND_MAX*2.0*M_PI;
			acceleration[i][0]+=cos(theta)*SCALE;
			acceleration[i][1]+=sin(theta)*SCALE;
			acceleration[i][2]+=sin(theta)*cos(theta)*SCALE;
		}
		//gravity
		acceleration[i][0]-=(points[i][0]-0.5)*SCALE;
		acceleration[i][1]-=(points[i][1]-0.5)*SCALE;
		acceleration[i][2]-=(points[i][2]-0.5)*SCALE;

		velocity[i][0]+=acceleration[i][0];
		velocity[i][1]+=acceleration[i][1];
		velocity[i][2]+=acceleration[i][2];
		//change position
		points[i][0]+=velocity[i][0];
		points[i][1]+=velocity[i][1];
		points[i][2]+=velocity[i][2];
		
	}
}
int clear = 3;
void drawString(char* s)
{
	int k;
	for(k=0;k<strlen(s);k++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[k]);
}

struct timeval timea;
struct timeval timeb;
char str[10];
void display(void)
{
	glLoadIdentity();
	gluLookAt(eyex,eyey,eyez,centerx,centery,centerz,0.0,1.0,0.0);
	double t;
	int i;
	if (clear) {
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT);
//		clear--;
	}
	//get front buffer
	glReadBuffer(GL_FRONT);
	glRasterPos2i(0,0);
	glCopyPixels(0,0,w,h,GL_COLOR);
	//fade out the front buffer
	glColor4f(0.0,0.0,0.0,0.05);
	glBegin(GL_QUADS);
	glVertex2i(0,0);
	glVertex2i(0,1);
	glVertex2i(1,1);
	glVertex2i(1,0);
	glEnd();
	glReadBuffer(GL_BACK);

	//axes
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);
	glEnd();

	//draw points
	glColor3f(1.0,1.0,1.0);
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glVertex3f(0.5,0.5,0.5);
	glEnd();
	glBegin(GL_LINES);
	for (i = 0; i < POINTS; i++) {
		glColor3f(1.0,1.0,1.0);
		glVertex3f(points[i][0],points[i][1],points[i][2]);
		glVertex3f(oldpoints[i][0],oldpoints[i][1],oldpoints[i][2]);
		if (draw_acc) {
			glColor3f(1.0,0.0,0.0);
			glVertex2f(points[i][0],points[i][1]);
			glVertex2f(points[i][0]+acceleration[i][0]*50,points[i][1]+acceleration[i][1]*50);
		}
	}
	glEnd();
	timea=timeb;
	gettimeofday(&timeb,NULL);
	double us = (timeb.tv_sec-timea.tv_sec)*1000000 + (timeb.tv_usec-timea.tv_usec);
	//FIXME: color needs to be before rasterpos2i, WHY??
	glColor3f(0.0,0.0,0.0);
	glRasterPos2f(0.0,0.0);
	drawString(str);
	
	sprintf(str,"%3.1f",1e6/us);
	glColor3f(1.0,1.0,1.0);
	glRasterPos2i(0,0);
	drawString(str);
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
		acc=!acc;
	}
	if(key=='m') {
			draw_acc=!draw_acc;
	}
	if(key=='w') {
		eyez+=0.1;
	}
	if(key=='s') {
		eyez-=0.1;
	}
	if(key=='a') {
		eyex+=0.1;
	}
	if(key=='d') {
		eyex-=0.1;
	}
	if(key=='r') {
		eyey+=0.1;
	}
	if(key=='f') {
		eyey-=0.1;
	}
//	gluLookAt(eyex,eyey,eyez, 0.0,0.0,1.0, 0.0,1.0,0.0);
	printf("%f %f %f\n",eyex,eyey,eyez);
	glutPostRedisplay();
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
	
	//gluPerspective(180.0,1.0,0.0,200.0);
	glFrustum(-0.10,0.10,-0.10,0.10,0.5,50.0);
//	gluLookAt(eyex,eyey,eyez, 0.0,0.0,1.0, 0.0,1.0,0.0);

	//gluOrtho2D(xmin,xmax,ymin,ymax);
	glMatrixMode(GL_MODELVIEW);
}
void idle() {
	usleep(20000);
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
		points[i][2] = 0.5;
		velocity[i][0] = 0.0;
		velocity[i][1] = 0.0;
		velocity[i][2] = 0.0;
	}
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
	glutInitWindowSize(w,h);					 // for double use GLUT_DOUBLE
	glutInitWindowPosition(100,50);
	glutCreateWindow("Particles");

	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	glutDisplayFunc(display);		// register callback functions
	glutIdleFunc(idle);				// no animation
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyfunc);
	glutReshapeFunc(reshape);

	glutMainLoop();					// here we go

	return 0;
}
