#pragma GCC diagnostic ignored "-Wwrite-strings"

#ifdef WIN32
 #define WINDOWS
#elif WIN64
 #define WINDOWS
#endif
#ifdef WINDOWS
  #include <windows.h>  // Without include this header in Windows, you will get error 
			// C2381: 'exit' : redefinition; __declspec(noreturn) differs
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
/**********************/
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include<iostream>//
#define ZC 0
#define ZF 110
#define XM 400
/**********************/
using namespace std;

enum face{BL = 0, TL = 1, TR = 2, BR = 3};
enum dir{L = 0, R = 1, U = 2, D = 3};

float eyeZ = 0.0f;
float eyeX = 0.0f;
float cX = 0.0f;
float cY = 0.0f;

//Light 
GLfloat  mat[4];

GLfloat rrAmb[4] = {0.35, 0.06, 0.02};
GLfloat rrDif[4] = {0.82, 0.2, 0.1};
GLfloat rrSpc[4] = {0.1, 0.1, 0.1};

GLfloat grAmb[4] = {0.35, 0.06, 0.02};
GLfloat grDif[4] = {0.521, 0.72, 0.2};
GLfloat grSpc[4] = {0.1, 0.1, 0.1};

GLfloat bAmb[4] = {0.35, 0.06, 0.02};
GLfloat bDif[4] = {0.1, 0.2, 0.8};
GLfloat bSpc[4] = {0.1, 0.1, 0.1};


GLfloat sAmb[4] = {0.231, 0.231, 0.231};
GLfloat sDif[4] = {0.278, 0.278, 0.278};
GLfloat sSpc[4] = {0.774, 0.774, 0.774};
GLfloat shine[] = {89.6};// silver sphere
	
GLfloat shiny[] = {5.0};// walls
GLfloat amb[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat diff[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
GLfloat lP[] = {0.0, 40.0, -5.0, 1.0};

bool kS[4]; //for camera pan
bool mS[4]; //for "movement"

struct pt{
	float x,y,z;
};

struct quad {
	pt pts[4];
};

quad ct;
pt ftl,ftr;
bool snap = false;

void init(int argc, char** argv) {

        glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_POSITION, lP);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 400);
	glutCreateWindow("Project 3, Shane Lopez");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.7f, 0.0f, 1.0f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void handleResize(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,
		       (double)w / (double)h,
		       1.0,
		       300.0);
	glMatrixMode (GL_MODELVIEW);
}

void keys(unsigned char key, int x, int y) {
	
	switch(key) {
		case 27: exit(0);
		case 's': kS[D] = true; break;
		case 'w': kS[U] = true; break;
		case 'a': kS[L] = true; break;
		case 'd': kS[R] = true; break;
		case 'k': mS[D] = true; break;
		case 'i': mS[U] = true; break;
		case 'j': mS[L] = true; break;
		case 'l': mS[R] = true; break;
		case ' ': snap = true; break;
	}
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	//draw and manipulate
	if(kS[U]) 
		cY++;
	if(kS[D]) 	
		cY--;
	if(kS[L]) 
		cX-=5;
	if(kS[R]) 
		cX+=5;
	if(mS[U]) 
		eyeZ--;
	if(mS[D])
		eyeZ++;
	if(mS[L]) {
		eyeX--;
		cX--;
	}
	if(mS[R]) {
		eyeX++;
		cX++;
	}
	
	if(eyeZ <= (ZF*-1)+5)
		eyeZ = (ZF*-1)+5;
	else if(eyeZ > ZC+5)
		eyeZ = ZC+5;
	if(cX > XM)
		cX = XM;
	else if(cX < XM*-1)
		cX = XM*-1;
	
	if(snap) {
		eyeX = eyeZ = cX = cY = 0.0;
		snap = false;
	}
	//Perspective Projection Volume
	gluLookAt(  eyeX, 0.0,   eyeZ, /*(EYE)*/
		    cX  , cY, -110.0, /*(VRP)*/
		    0.0 , 1.0,    0.0  /*(VUP)*/);
	
	//Draw Objects
	glPushMatrix();
		glTranslated(-20.0,0.0,-40.f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glNormal3f(-1.0f, 0.0f, 1.0f);
		
		
		glColor3f(0.539, 0.539, 0.539);
		mat[0] = sAmb[0];
		mat[1] = sAmb[1];
		mat[2] = sAmb[2];
		mat[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
		mat[0] = sDif[0];
		mat[1] = sDif[1];
		mat[2] = sDif[2];
		mat[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
		mat[0] = sSpc[0];
		mat[1] = sSpc[1];
		mat[2] = sSpc[2];
		mat[3] = 1.0;
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
		glMaterialfv(GL_FRONT, GL_SHININESS, shine);
		
		
		glutSolidSphere(20.0, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(15.0,0.0,-85.0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glNormal3f(-1.0f, 0.0f, 1.0f);
		
		//glColor3f(0.439, 0.439, 0.439);
		glutSolidSphere(20.0, 20, 20);
	glPopMatrix();
	
	mat[0] = rrAmb[0];
	mat[1] = rrAmb[1];
	mat[2] = rrAmb[2];
	mat[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = rrDif[0];
	mat[1] = rrDif[1];
	mat[2] = rrDif[2];
	mat[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = rrSpc[0];
	mat[1] = rrSpc[1];
	mat[2] = rrSpc[2];
	mat[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
			
	glBegin(GL_QUADS);
	  // Red Wall
	  glColor3f(1.0, 0.0, 0.0);
	  
	  glNormal3f(1.0f, 0.0f, 0.0f);
	  glVertex3f(-40.0f, -20.0f, -10.0f);
	  glNormal3f(1.0f, 0.0f, 0.0f);
	  glVertex3f(-40.0f, 30.0f, -10.0f);
	  glNormal3f(1.0f, 0.0f, 0.0f);
	  glVertex3f(-40.0f, 30.0f, -110.0f/*-80.0f*/);
	  glNormal3f(1.0f, 0.0f, 0.0f);
	  glVertex3f(-40.0f, -20.0f, -110.0f/*-80.0f*/);
	  
	  // Green Wall
	  mat[0] = grAmb[0];
	  mat[1] = grAmb[1];
	  mat[2] = grAmb[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	  mat[0] = grDif[0];
	  mat[1] = grDif[1];
	  mat[2] = grDif[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	  mat[0] = grSpc[0];
	  mat[1] = grSpc[1];
	  mat[2] = grSpc[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
	  
	  
	  glColor3f(0.0, 1.0, 0.0);
	  
	  glNormal3f(-1.0f, 0.0f, 0.0f);
	  glVertex3f(40.0f, -20.0f, -10.0f);
	  glNormal3f(-1.0f, 0.0f, 0.0f);
	  glVertex3f(40.0f, 30.0f, -10.0f);
	  glNormal3f(-1.0f, 0.0f, 0.0f);
	  glVertex3f(40.0f, 30.0f, -110.0f/*-80.0f*/);
	  glNormal3f(-1.0f, 0.0f, 0.0f);
	  glVertex3f(40.0f, -20.0f, -110.0f/*-80.0f*/);
	  
	  // Blue wall
	  mat[0] = bAmb[0];
	  mat[1] = bAmb[1];
	  mat[2] = bAmb[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	  mat[0] = bDif[0];
	  mat[1] = bDif[1];
	  mat[2] = bDif[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	  mat[0] = bSpc[0];
	  mat[1] = bSpc[1];
	  mat[2] = bSpc[2];
	  mat[3] = 1.0;
	  glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	  glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
	
	  glColor3f(0.0, 0.0, 1.0);
	 
	  glNormal3f(0.0f, 0.0f, 1.0f); 
	  glVertex3f(-40.0f, -20.0f, -110.0f/*-80.0f*/);
	  glNormal3f(0.0f, 0.0f, 1.0f);
	  glVertex3f(-40.0f, 30.0f, -110.0f/*-80.0f*/);
	  glNormal3f(0.0f, 0.0f, 1.0f);
	  glVertex3f(40.0f, 30.0f, -110.0f/*-80.0f*/);
	  glNormal3f(0.0f, 0.0f, 1.0f);
	  glVertex3f(40.0f, -20.0f, -110.0f/*-80.0f*/);
	
	  // Ceiling
	  glColor3f(0.0, 2.0, 1.0);
	 
	  glNormal3f(0.0f, -1.0f, 0.0f);   
	  glVertex3f(-40.0f, 30.0f, -10.0f);  // B
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glVertex3f(40.0f, 30.0f, -10.0f);   // F
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glVertex3f(40.0f, 30.0f, -110.0f/*-80.0f*/);   // G
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glVertex3f(-40.0f, 30.0f, -110.0f/*-80.0f*/);  // C
	  
	  // Floor
	  ct.pts[BL].x = -40.0; ct.pts[BL].y = -20.0; ct.pts[BL].z = -10.0;
          ct.pts[TL].x = -40.0; ct.pts[TL].y = -20.0; ct.pts[TL].z = -110.0;
	  ct.pts[TR].x = -20.0; ct.pts[TR].y = -20.0; ct.pts[TR].z = -110.0;
	  ct.pts[BR].x = -20.0; ct.pts[BR].y = -20.0; ct.pts[BR].z = -10.0; 
	  
	  for(int row = 0; row < 4; row++) {
	  	for(int col = 0; col < 4; col++) {
	  		if(col == 0) {
				ftl = ct.pts[TL];
				ftr = ct.pts[TR];
			}
			
			if(row%2 == 0) {
				if(col%2 == 0)
					glColor3f(1.0, 1.0, 1.0);
				else
					glColor3f(0.0, 0.0, 0.0);
			}
			else {
				if(col%2 == 0)
					glColor3f(0.0, 0.0, 0.0);
				else
					glColor3f(1.0, 1.0, 1.0);
			}
			
			if(col == 3) {
				ct.pts[TR].x = 40.0;
				ct.pts[BR].x = 40.0;
			}
			
			glVertex3f(ct.pts[BL].x, ct.pts[BL].y, ct.pts[BL].z);
			glVertex3f(ct.pts[TL].x, ct.pts[TL].y, ct.pts[TL].z);
			glVertex3f(ct.pts[TR].x, ct.pts[TR].y, ct.pts[TR].z);
			glVertex3f(ct.pts[BR].x, ct.pts[BR].y, ct.pts[BR].z);
			
			ct.pts[BL].x = ct.pts[BR].x;
			
			ct.pts[TL].x = ct.pts[BL].x;
			
			ct.pts[TR].x = ct.pts[TL].x + 20;
			ct.pts[BR].x = ct.pts[TR].x;
	  	}
		
		ct.pts[BL] = ftl;
		
		ct.pts[TL].x = ct.pts[BL].x;
		ct.pts[TL].y = ct.pts[BL].y;
		ct.pts[TL].z = ct.pts[BL].z + 20;
		
		ct.pts[TR].x = ct.pts[TL].x + 20;
		ct.pts[TR].y = ct.pts[TL].y;
		ct.pts[TR].z = ct.pts[TL].z;
		
		ct.pts[BR] = ftr;
	  }
	  
	  
	glEnd();
	
	//swap buffers
	glutSwapBuffers();
	kS[L] = false;
	kS[R] = false;
	kS[U] = false;
	kS[D] = false;
	mS[L] = false;
	mS[R] = false;
	mS[U] = false;
	mS[D] = false;
}

void update(int val) {
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

int main(int argc, char** argv) {
	init(argc, argv);
	glutDisplayFunc(draw);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keys);
	glutTimerFunc(5, update, 0);
	glDepthFunc(GL_LESS);
	glutMainLoop();
	return 0;
}
