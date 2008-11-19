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

#define POINTS 2000
#define RADIUS (tehradius)
#define ROFFSET (POINTS/2)
#define WALKERS 1000
#define SCALE (1.0/70.0)
#define BARS 60
#define MAXD 1.0
int points[WALKERS][3];
int maxheight = 0;
double sigma,mean;
int tehradius = 20;
int stickypoints = 1;
//char grid[POINTS+5][POINTS+5];
char** grid;
		//int cx[WALKERS];
//int cy[WALKERS];
double x=0.667,y=0.5;
double xmin,ymin,xmax,ymax;
int w=500,h=500;

double eyex = 0.0, eyey = 0.5, eyez = 6.0;
double centerx = 0.0, centery = 0.0, centerz = 0.0;

int acc=1;
int draw_acc=0;

//void calcradius()
//{
//	tehradius = log(sqrt((double)stickypoints)*5)*10;
//	printf("%d\n",tehradius);
//}

void printGrid() {
	printf("-------\n");
	int i,j;
	for (i = 0; i < POINTS+5; i++) {
		for (j = 0; j < POINTS+5; j++) {
			printf("%d ",grid[j][i]);
		}
		printf("\n");
	}
}
int distance_off(int x, int y) {
	return sqrt((x-ROFFSET)*(x-ROFFSET)+(y-ROFFSET)*(y-ROFFSET));
}
void step()
{
	int n;
	for (n = 0; n < WALKERS; n++) {
			int sum = 0;
			int distance = sqrt((points[n][0]-ROFFSET)*(points[n][0]-ROFFSET)+(points[n][1]-ROFFSET)*(points[n][1]-ROFFSET));
			if (distance < RADIUS) {
				if (points[n][0] + 1 < POINTS)
					sum += grid[points[n][0]+1][points[n][1]];
				if (points[n][0] > 0)
					sum += grid[points[n][0]-1][points[n][1]];
				if (points[n][1] + 1 < POINTS)
					sum += grid[points[n][0]][points[n][1]+1];
				if (points[n][1] > 0)
					sum += grid[points[n][0]][points[n][1]-1];
			}
			if (sum || distance >= RADIUS) {
					if (sum) {
						grid[points[n][0]][points[n][1]] = 1;
						int dist = distance_off(points[n][0],points[n][1]);
						if (dist + 50 > tehradius)
							tehradius = dist + 50;
						stickypoints++;
					}
					double theta = (double)rand()/RAND_MAX*M_PI*2;
					points[n][0] = (int)(cos(theta)*(RADIUS-1)+ROFFSET);
					points[n][1] = (int)(sin(theta)*(RADIUS-1)+ROFFSET);
//					int rad = (double)rand()/RAND_MAX*RADIUS;
//					points[n][0] = (int)(cos(theta)*rad+ROFFSET);
//					points[n][1] = (int)(sin(theta)*rad+ROFFSET);
//					if (sum) {
//						glutPostRedisplay();
//					}
			} else {
					switch (rand()%4) {
						case 0:
							points[n][0]++;
							break;
						case 1:
							points[n][0]--;
							break;
						case 2:
							points[n][1]++;
							break;
						case 3:
							points[n][1]--;
							break;
					}
			}
	}
	glutPostRedisplay();
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
/*	glReadBuffer(GL_FRONT);
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
*/
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
	
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINE_STRIP);
	for (t=0.0; t < 2*M_PI; t+=0.1) {
		glVertex3f(cos(t),sin(t),0);
	}
	glEnd();

	//draw points
/*	glColor3f(1.0,1.0,1.0);
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
			glVertex3f(points[i][0],points[i][1],points[i][2]);
			glVertex3f(points[i][0]+acceleration[i][0]*50,points[i][1]+acceleration[i][1]*50,points[i][2]+acceleration[i][2]*50);
		}
	}
	glEnd();
*/	
	glPointSize(1.0);
	int x,y;
	glBegin(GL_POINTS);
	for (x=0; x < POINTS; x++) {
		for (y=0; y < POINTS; y++) {
			if (grid[x][y]) {
				glColor3f(grid[x][y],0,0);
				glVertex3f((double)x/POINTS*2-1.0,(double)y/POINTS*2-1.0,0.0);
			}
		}
	}
	glColor3f(0.0,1.0,0.0);
	int n;
	for (n = 0; n < WALKERS; n++) {
		glVertex3f((double)points[n][0]/POINTS*2-1.0,(double)points[n][1]/POINTS*2-1.0,0.0);
	}
	glEnd();
/*
	double scale = 1.0/(sigma*sqrt(2*M_PI));
	glColor4f(0.5,1.0,0.5,0.5);
	for (i = 0; i < BARS; i++) {
		glBegin(GL_QUADS);
		glVertex3f(1.0/BARS*i,0,0);
		glVertex3f(1.0/BARS*i,(double)bars[i]/maxheight,0);
		glVertex3f(1.0/BARS*(i+1),(double)bars[i]/maxheight,0);
		glVertex3f(1.0/BARS*(i+1),0,0);
		glEnd();
	}
	glColor4f(1.0,1.0,1.0,0.9);
	glBegin(GL_LINE_STRIP);
	double pos;
	for (pos = 0.0; pos <= 1.0; pos+=0.005)
	{
		double y = 1.0/(sigma*sqrt(2*M_PI))
						*exp(-1.0*pow(pos*MAXD-mean,2.0)/(2*sigma*sigma))
						;
		glVertex3f(pos,y/scale,0.0);
	}
	glEnd();
	*/
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
//	usleep(20000);
	int i;
	for (i = 0; i < 10; i++)
		step();
	//calcradius();
//	glutPostRedisplay();
}

int main(int argc,char* argv[])
{ 
	int i = 0;
	grid = (char**)malloc(sizeof(char*)*POINTS);
	for (i = 0; i < POINTS; i++) {
		grid[i] = (char*)malloc(sizeof(char)*POINTS);
	}
	for (i = 0; i < WALKERS; i++) {
		double theta = (double)rand()/RAND_MAX*M_PI*2;
		int rad = (double)rand()/RAND_MAX*RADIUS;
		points[i][0] = (int)(cos(theta)*rad+ROFFSET);
		points[i][1] = (int)(sin(theta)*rad+ROFFSET);
	}
	srand(9);
	grid[ROFFSET][ROFFSET] = 1;
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
