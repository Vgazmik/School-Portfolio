#include <GL/glut.h>
#include <stdlib.h>

using namespace std;

int angle = 0;

void init(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 400);
	//glutInitWindowSize(800, 800);
	glutCreateWindow("Introduction to OpenGL");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.7f, 0.0f, 1.0f, 1.0f);
	//glClearColor(0.7f, 0.7f, 0.7f, -1.0f);
}

void handleResize(int w, int h) {
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
		       (double)w / (double)h,
		       1.0,
		       200.0);
	
}

void keys(unsigned char key, int x, int y) {
	
	switch(key) {
		case 27: exit(0);
	}

}

void draw() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glTranslatef(0.0f, 0.0f, -5.0f);
	glTranslatef(0.0f, -0.5f, -4.5f);//
	//glRotatef(25.0, 1.0f, 0.0f, 0.0f);//rotate along X   
	   glPushMatrix();
	glScalef(0.5f, 0.5f, 0.5f);
	   //glRotatef(angle*2, 0.0f, 1.0f, 0.0f);//Y
	   glTranslatef(0.0f, 0.25f, 0.0f);
	   glRotatef(angle*1, 0.0f, 1.0f, 0.0f);//Y
	
	/**Draw**/
	glBegin(GL_TRIANGLES);
	
	glColor3f(0.85, 0.25, 0.25);//
	glVertex3f(1.5f, 1.0f, -5.0f);
	glColor3f(0.85, 0.0, 0.25);//
	glVertex3f(1.0f, 0.0f, -5.0f);
	glColor3f(1.0, 1.0, 1.0);//
	glVertex3f(0.5f, 1.0f, -5.0f);
	
	glEnd();
	
	/*glBegin(GL_POLYGON);
	   
	   glColor3f(0.0, 0.25, 0.7);
	   glVertex3f(1.0f, 1.0f, -5.0f);
	   glVertex3f(1.0f, 1.0f, -7.0f);
	   glVertex3f(-1.0f, 1.0f, -5.0f);
	   glVertex3f(-1.0f, 1.0f, -7.0f);
	   //glVertex3f(-1.0f, 1.0f, -7.0f);
	   
	glEnd();*/

	glPopMatrix();
	//glPushMatrix();
	
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
