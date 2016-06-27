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
/**********************/
using namespace std;

int angle = 0;

void init(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 400);
	glutCreateWindow("Project 3, Shane Lopez");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.7f, 0.0f, 1.0f, 1.0f);
}
/* output code by Mark Kilgard */
void output(int x, int y, char *string) {
	int len, i;
	
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
		       (double)w / (double)h,
		       1.0,
		       200.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode (GL_MODELVIEW);
}

void keys(unsigned char key, int x, int y) {
	//output(100,100,(char*)key);
	//glRasterPos2f(100, 100);
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, key);
	switch(key) {
		case 27: exit(0);
	}
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	//draw and manipulate
	//Perspective Projection Volume
	gluLookAt(0.0 /*eyeX    (PRP)*/,
		  0.0 /*eyeY*/,
		  0.0 /*eyeZ*/,
		  0.0 /*centerX (VRP)*/,
		  0.0 /*centerY*/,
		-10.0 /*centerZ*/,
		  0.0 /*upX     (VUP)*/,
		  1.0 /*upY*/,
		  0.0 /*upZ*/);
	//Draw Objects
	glBegin(GL_QUADS);
	  // Red Wall
	  glColor3f(50.0, 0.0, 0.0);
	  
	  glVertex3f(-40.0f, -20.0f, -10.0f);
	  glVertex3f(-40.0f, 30.0f, -10.0f);
	  glVertex3f(-40.0f, 30.0f, -80.0f);
	  glVertex3f(-40.0f, -20.0f, -80.0f);
	  //output(100,100,"test"); ////////////////////
	  // Green Wall
	  glColor3f(0.0, 50.0, 0.0);
	  
	  glVertex3f(40.0f, -20.0f, -10.0f);
	  glVertex3f(40.0f, 30.0f, -10.0f);
	  glVertex3f(40.0f, 30.0f, -80.0f);
	  glVertex3f(40.0f, -20.0f, -80.0f);
	  
	  // Blue wall
	  glColor3f(0.0, 0.0, 50.0);
	  
	  glVertex3f(-40.0f, -20.0f, -80.0f);
	  glVertex3f(-40.0f, 30.0f, -80.0f);
	  glVertex3f(40.0f, 30.0f, -80.0f);
	  glVertex3f(40.0f, -20.0f, -80.0f);
	
	  // Ceiling
	  glColor3f(0.0, 20.0, 10.0);
	  
	  glVertex3f(-40.0f, 30.0f, -10.0f);  // B
	  glVertex3f(40.0f, 30.0f, -10.0f);   // F
	  glVertex3f(40.0f, 30.0f, -80.0f);   // G
	  glVertex3f(-40.0f, 30.0f, -80.0f);  // C
	  
	  // Floor
	  glColor3f(20.0, 0.0, 10.0);
	  
	  glVertex3f(-40.0f, -20.0f, -10.0f); // A
	  glVertex3f(40.0f, -20.0f, -10.0f);  // E
	  glVertex3f(40.0f, -20.0f, -80.0f);  // H
	  glVertex3f(-40.0f, -20.0f, -80.0f); // D
	  
	glEnd();
	
	//swap buffers
	glutSwapBuffers();
}

void update(int val) {
	angle+=1;
	if(angle > 360)
		angle = 0;
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

int main(int argc, char** argv) {
	init(argc, argv);
	glutDisplayFunc(draw);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keys);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}
