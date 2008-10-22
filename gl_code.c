#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#define POINTS 500
#define SCALE (1.0/200.0)
double points[POINTS][2];

double x=0.667,y=0.5;
double xmin,ymin,xmax,ymax;
int w=400,h=400;

void step()
{
	int i;
	double theta;
	for (i = 0; i < POINTS; i++) {
		theta = rand()/(double)RAND_MAX*2.0*M_PI;
		points[i][0]+=cos(theta)*SCALE;
		points[i][1]+=sin(theta)*SCALE;
	}
}

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

	glPointSize(1.0);

	glBegin(GL_POINTS);
	int i;
	for (i = 0; i < POINTS; i++) {
		glVertex2f(points[i][0],points[i][1]);
	}
	glEnd();

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
void idle() {
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
	}
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // single buffering
	glutInitWindowSize(w,h);					 // for double use GLUT_DOUBLE
	glutInitWindowPosition(100,50);
	glutCreateWindow("OpenGL Demo");

	glClearColor(1.0,1.0,1.0,0.0);
	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(display);		// register callback functions
	glutIdleFunc(idle);				// no animation
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyfunc);
	glutReshapeFunc(reshape);

	glutMainLoop();					// here we go

	return 0;
}
