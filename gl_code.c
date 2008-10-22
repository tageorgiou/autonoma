#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#define POINTS

double x=0.667,y=0.5;
double xmin,ymin,xmax,ymax;
int w=400,h=300;

void drawString(char* s)
{
	int k;
	for(k=0;k<strlen(s);k++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[k]);
}
void display(void)
{
	double t;

	glClear(GL_COLOR_BUFFER_BIT); // clear the screen
	glColor3f(0.0,0.0,0.0);

	glBegin(GL_POINTS);
	glVertex2f(0.5,0.2);
	glEnd();

	glRasterPos2f(x,y);
	drawString("x,y"); // center of circle, shows where text prints

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
	if(key=='q')
	{
		exit(0);
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
int main(int argc,char* argv[])
{  
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
	glutInitWindowSize(w,h);					 // for double use GLUT_DOUBLE
	glutInitWindowPosition(100,50);
	glutCreateWindow("OpenGL Demo");

	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(display);		// register callback functions
	glutIdleFunc(NULL);				// no animation
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyfunc);
	glutReshapeFunc(reshape);

	glutMainLoop();					// here we go

	return 0;
}
