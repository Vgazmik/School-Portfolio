/* Authors: Chihua Ma, Shane Lopez 
 *
 * Functions by Shane Lopez:
 *
 *	void line(pt,pt);
 *	void circlePoints(pt);
 *	void circleSegment(int);
 *	void name(pt);
 *      void fill(int,pt,pt,pt,pt);
 *      void house(pt);
 *
 * Skeleton code provided by: Chichua Ma
 *
 * Date: Sep 18, 2013
 *
 * Operating System: Ubuntu 12.6 64-bit
 * Compiler: g++
 *
 * Press key 'd' to advance through slides
 * Press 'esc' to exit
 */
#pragma GCC diagnostic ignored "-Wwrite-strings" //ignores annoying compiler warnings
#include "pt.h" // By Shane Lopez

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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Intialization of the sign of drawing graphics */
bool drawLines = false;//not used
bool drawName = true;//determines whether to draw name
bool drawHouse = false;//'                      ' house
bool drawFillHouse = false;//'                  ' filled house

/******************************************************************/

/* output code by Mark Kilgard */
void output(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}
/**********************************************************
Adapted from skeleton code provided by Chihua Ma, detects keyboard use 
and changes boolean draw values as specified by the instructions.
**********************************************************/
void Keyboard(unsigned char key, int x, int y)
{
	switch(key){
		case 'd'://the 'd' key advances through the different 'slides'
			if(drawName == true) {
				drawName = false;
				drawHouse = true;
				drawFillHouse = false;
				break;
			}
			if(drawHouse == true) {
				drawName = false;
				drawHouse = false;
				drawFillHouse = true;
				break;
			}
			if(drawFillHouse == true) {
				drawName = true;
				drawHouse = false;
				drawFillHouse = false;
				break;
			}
			break;
		case 27://escape key exits
			exit(0);
			break;
	}
}
/*******************************************************************
*line(pt,pt) takes two pt objects, calculates a decision variable d 
*to determine position relative to the line between the two pt's, and
*fills pixels accordingly to give the appearance of a straight line.
*Code modified from the course text to handle slopes -1<=m<=1 as well 
*as vertical lines and steep slopes |m|>1
*Author: Shane Lopez
*******************************************************************/
void line(pt a, pt b) {
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	int d = 2 * dy - dx;
	int incrE = 2*dy;
	int incrNE = 2 * (dy - dx);
	int incrSE = 2 * (dy + dx);
	int x = a.x;
	int y = a.y;
	bool ENE = true;//east or northeast 
	bool UD = false;//up or down 
	bool SE = false;//south east
	bool steep = false;//slope |m|>1
		
	if((x == b.x) && (y == b.y))//two endpoints equal
		return;
	if(x > b.x) {//line from right to left, this implementation from left
	             //to right
		line(b,a);
		return;
	}
	
	if(x == b.x) {//same x values of points, vertical
		ENE = false;
		UD = true;
	}
	if(!UD && ((dy/dx) > 1 || (dy/dx) < -1))//steep line
		steep = true;
	if((x < b.x) && (y > b.y)) {//south east direction, negative slope
		ENE = false;
		SE = true;
	}

	if(UD) {

		glBegin(GL_POINTS);
		
		glVertex2f((float)x,(float)y);
		if(y < b.y) {
			y++;
			while(y < b.y) {
				glVertex2f((float)x,(float)y);
				y++;	
			}
		}
		else{
			y--;
			while(y > b.y) {
				glVertex2f((float)x,(float)y);
				y--;	
			}
		}
		
		glEnd();
		return;
	}
	else if(ENE && !steep) {
		glBegin(GL_POINTS);
		
		glVertex2f((float)x,(float)y);
		while(x < b.x) {
			if(d <= 0) {
				d+=incrE;
				x++;
			}	
			else {
				d+=incrNE;
				x++;
				y++;
			}

			glVertex2f((float)x,(float)y);
		}
		
		glEnd();
		return;
	}
	else if (SE && !steep) {
		d = 2 * dy + dx;
		glBegin(GL_POINTS);
		
		glVertex2f((float)x,(float)y);		
		while(x < b.x) {
			if(d <= 0) {
				d+=incrSE;
				x++;
				y--;
			}
			else {
				d+=incrE;
				x++;
			}
			
			glVertex2f((float)x,(float)y);
		}

		glEnd();
		return;
	}
	else if(steep) {//slopes |m|>1 require the use of floating point 
			//coordinates as opposed to integers
		float x_ = (float) x;
		float y_ = (float) y;
		float dx_ = (float)(b.x - a.x);
		float dy_ = (float)(b.y - a.y);
		float d_ = (float)(2 * (dy - dx));
		float incrE_ = 2*dy_;
		float incrNE_ = 2 * (dy_ - dx_);
		float incrSE_ = 2 * (dy_ + dx_);
		if(ENE) {
			glBegin(GL_POINTS);
			
			glVertex2f((float)x,(float)y);
			while(y_ < (float)b.y) {
				if(d_ <= 0) {
					d_+=incrE;
					y_+=0.2;
				}	
				else {
					d_+=incrNE;
					x_+=0.1;
					y_+=0.2;
				}

				glVertex2f(x_,y_);
			}
		
			glEnd();
			return;
			
		}
		else if(SE) {
			d_ = 2 * dy_ + dx_;
			glBegin(GL_POINTS);
		
			glVertex2f((float)x,(float)y);		
			while(y_ > (float)b.y) {
				if(d_ <= 0) {
					d_+=incrSE;
					x_+=0.1;
					y_-=0.2;
				}
				else {
					d_+=incrE;
					y_-=0.2;
				}
			
				glVertex2f(x_,y_);
			}

			glEnd();
			return;
			
		} 
	}
	else return;

}

/*******************************************************************
*circlePoints(pt) takes a pt object and fills pixels relative to it's 
*position to fill in points on an arc.
*Author: Shane Lopez
*******************************************************************/

void circlePoints(pt start) {
	float x = (float)start.x;
	float y = (float)start.y;
	
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glVertex2f(x,-y);
	glVertex2f(-x,y);
	glVertex2f(-x,-y);
	glVertex2f(y,x);
	glVertex2f(y,-x);
	glVertex2f(-y,x);
	glVertex2f(-y,-x);
	glEnd();
}

/*******************************************************************
*circleSegment(pt) takes an int to represent the radius of a desired circle
*segment, and according to calculations relating to position on a circle 
*utilizes circlePoints(pt) to selectively fill an arc of the desired radius.
*Author: Shane Lopez
*******************************************************************/

void circleSegment(int radius) {
	int x = 0;
	int y = radius;
	pt origin = pt(x,radius);
	double d = 5.0 / 4.0 - radius;
	circlePoints(origin);
	pt a;
	
	while(y > x) {
		if(d < 0)
			d+= 2.0 * x + 3.0;
		else {
			d+= 2.0 * (x - y) + 5.0;
			y--;
		}
		x++;
		a.setXY(x,y);
		circlePoints(a);
	}
}

/*******************************************************************
*name(pt) takes a pt object and uses it's position as that of the top left 
*pt of the first letter of the Author's first name. From this point
*all other point positions are derived by means of variables dE and dS,
*representing the length of an edge and that of a space between letters 
*respectively.
*Author: Shane Lopez
*******************************************************************/

void name(pt start) {
	int x = start.x;
	int y = start.y;
	int dE = 24.0;
	int dS = (dE / 2);
	
	/*
		____ _  _  _  _  _ ____
		|___ |__| /_\ |\ | |___
		____||  |/   \| \| |___

	*/
	glBegin(GL_POINTS);
	//Draw 'S'
	pt a = pt(x,y);//top left
	pt b = pt((x+dE),y);//top right
	pt c = pt(x,(y-dE));//middle left
	pt d = pt((x+dE),(y-dE));//middle right
	pt e = pt(x,(y-(2*dE)));//bottom left
	pt f = pt((x+dE),(y-(2*dE)));//bottom right
	
	line(a,b);
	line(a,c);
	line(c,d);
	line(d,f);
	line(e,f);
	//increment x for ease
	x = x+dE+dS;//initial x plus edge length plus a space
	//Draw 'H'
	pt g = pt(x,y);//TL
	pt h = pt((x+dE),y);//TR
	pt i = pt(x,(y-dE));//ML
	pt j = pt((x+dE),(y-dE));//MR
	pt k = pt(x,(y-(2*dE)));//BL
	pt l = pt((x+dE),(y-(2*dE)));//BR
	
	line(g,i);
	line(i,j);
	line(i,k);
	line(h,j);
	line(j,l);
	//glVertex2f((float)k.x,((float)(y-(2*dE)))-0.1);
	//glVertex2f((float)l.x,((float)(y-(2*dE)))-0.1);
	//increment x
	x = x+dE+dS;
	//Draw 'A'
	pt m = pt(x,(y-(2*dE)));
	pt n = pt(x,(y-dE));
	pt o = pt((x+(dE/2)),y);
	pt p = pt((x+dE),(y-dE));
	pt q = pt((x+dE),(y-(2*dE)));

	line(m,n);
	line(n,o);
	line(n,p);
	line(o,p);
	line(p,q);
	//glVertex2f((float)m.x,(float)m.y);
	//glVertex2f((float)q.x,(float)q.y);
	//increment x
	x = x+dE+dS;
	//Draw 'N'
	pt r = pt(x,y);
	pt s = pt(x,(y-(2*dE)));
	pt t = pt((x+dE),y);
	pt u = pt((x+dE),(y-(2*dE)));
	
	line(r,s);
	line(r,u);
	line(t,u);
	//glVertex2f((float)s.x,(((float)s.y)-0.1));
	//increment x
	x = x+dE+dS;
	//draw 'E'
	pt v = pt(x,y);
	pt w = pt((x+dE),y);
	pt x_ = pt(x,(y-dE));
	pt y_ = pt((x+dE),(y-dE));
	pt z = pt(x,(y-(2*dE)));
	pt end = pt((x+dE),(y-(2*dE)));
	
	line(v,w);
	line(v,x_);
	line(x_,y_);
	line(x_,z);
	line(z,end);
	
	glEnd();
}
/******************************************************************
fill(int,pt,pt,pt,pt) takes an integer parameter to determine the type
of fill, at present calibrated only for retangular and trapezoidal fills.   
******************************************************************/
void fill(int opt,pt tl,pt tr,pt bl, pt br) {
	if(opt == 0) { //rectangular fill, moving from top down to bottom
                       //with horizontal lines.
		int ymx = tl.y;//top left vertex y val
		int ymn = bl.y;//bottom left vertex y val
		int i = 1;
		float dec = 0;
		//pt tmp = pt(tl.x,tl.y);
		pt t2 = pt(tl.x,(ymx));
		pt t3 = pt(tr.x,(ymx));

		while(t2.y != ymn) {
			dec = i;
			t2.setXY(tl.x,(ymx-dec));
			t3.setXY(tr.x,(ymx-dec));
			line(t2,t3);
			i++;
		}
	}
	else if(opt == 1) {//trapezoidal fill
		/*************************************/
		//this takes the mechanics of the line function to calculate
		//point positions on the outer edges of the trapezoid,linking
		//them with the very line function these calculations are
                //derived from, line(pt,pt).
		int dx1 = tl.x - bl.x;
		int dy1 = tl.y - bl.y;
		int dx2 = br.x - tr.x;//
		int dy2 = br.y - tr.y;//
		int d1 = 2 * dy1 - dx1;
		int d2 = 2 * dy2 + dx2;
		int incrE1 = 2*dy1;
		int incrE2 = 2*dy2;
		int incrNE = 2 * (dy1 - dx1);
		int incrSE = 2 * (dy2 + dx2);	
		int x1 = bl.x;
		int x2 = tr.x;//
		int y1 = bl.y;
		int y2 = tr.y;//
		int i = 0;
		
		pt l1[dy1];//array of pts to be calculated
		pt l2[dy1]; //array of pts to be calculated
		pt a = pt(bl.x,bl.y);
		pt b = pt(tr.x,tr.y);
		
		line(tl,tr);
		line(bl,br);
		while(a.x != tl.x) {
		
			if(d1 <= 0) {
				d1+=incrE1;
				x1++;
			}	
			else {
				d1+=incrNE;
				x1++;
				y1++;
			}
	
			if(d2 <= 0) {
				d2+=incrSE;
				x2++;
				y2--;
			}
			else {
				d2+=incrE2;
				x2++;
			}
			a.setXY(x1,y1);
			b.setXY(x2,y2);
	
			l1[i] = a;
			l2[i] = b;
			i++;
		}
		for(int j = 0; j < dy1; j++) {//as the left edge points are 
					      //calculated from the bottom up
					      //and the right top down, the
					      //latter will be reversed and 
					      //used to draw lines.
			for(int k = dy1-1; k > -1; k--) {
				line((l1[j]),(l2[k]));
			}
		}
			
	}
}
/******************************************************************
house(pt,int) takes a starting point parameter and an integer parameter
to decide whether or not to fill in the shapes generated, which consist
of two rectangles and three trapezoids.
******************************************************************/
void house(pt start, int fil) {
int dE = 50;
int x = start.x;
int y = start.y; 

glBegin(GL_POINTS);

pt a = pt(x,y); //top left trap
pt b = pt((x+(2*dE)),y); //top right trap
pt c = pt((x-dE),(y-dE)); //bottom left trap
pt d = pt((x+(3*dE)),(y-dE)); //bottom right trap
pt e = pt(x,(y-dE)); //top left square
pt f = pt((x+(2*dE)),(y-dE)); //top right square
pt g = pt(x,(y-(2*dE))); // bottom left square
pt h = pt((x+(2*dE)),(y-(2*dE))); //bottom right square

line(b,a);
line(a,c);
line(b,d);
line(c,d);
line(e,f);
line(e,g);
line(f,h);
line(g,h);

if(fil == 1) {
	fill(0,e,f,g,h);
	glColor3f(1.0,0.0,0.0);
	fill(1,a,b,c,d);
}
/////////////////////////////Tree-Time :)
//Trunk
pt blt = pt((h.x+dE+(dE/2)),h.y);//bottom left of tree
pt brt = pt((blt.x+(dE/2)),blt.y);//bottom right of tree
pt tlt = pt(blt.x,(blt.y + (2*dE)));//top left of tree
pt trt = pt(brt.x,tlt.y);//top right of tree
////////////////////////////////
//Leaves
//trap
pt ll = pt((tlt.x-(dE/2)),tlt.y);//lower left trap
pt rl = pt((trt.x+(dE/2)),trt.y);//lower right trap
pt ul = pt(tlt.x,(tlt.y+(dE/2)));//upper left trap
pt ur = pt(trt.x,ul.y);//upper right trap
//trap2
pt bl = pt(ll.x,ul.y);//bottom left tri
pt br = pt(rl.x,ur.y);//bottom right tri
pt a1 = pt(ul.x,(ul.y+(dE/2)));//apex of tri
pt a2 = pt(ur.x,(a1.y));
//draw trunk
glColor3f(0.5,0.35,0.05);
line(blt,brt);
line(blt,tlt);
line(brt,trt);
line(tlt,trt);
if(fil == 1)
	fill(0,tlt,trt,blt,brt);
//draw leaves
glColor3f(0.0,0.1,0.0);
line(ll,rl);
line(ll,ul);
line(ul,ur);
line(ur,rl);
if(fil == 1)
	fill(1,ul,ur,ll,rl);
//
line(bl,br);
line(bl,a1);
line(a1,a2);
line(a2,br);
if(fil == 1)
	fill(1,a1,a2,bl,br);
//line(ll,a1);
//line(a1,rl);

glEnd();

return;

}

/******************************************************************/

/******************************************************************/
void display(void) {
	/* clear the screen to the clear colour */
	glClear(GL_COLOR_BUFFER_BIT);

	// draw lines
    if (drawLines == true) { //Basically a blank canvas for testing
                             //functionality, not used for the project.
		glColor3f(1.0,0.0,0.0);
		
		pt a = pt(100,100);
		pt b = pt(200,200);
		pt c = pt(400,100);
		pt d = pt(200,50);

		line(a,b);
		line(a,c);
		line(b,c);

		circleSegment(300);		

		glColor3f(0.0, 0.0, 1.0);
		output(200,150,"Lines");
	}
	//draw name
	if (drawName == true) {

		glColor3f(0.0, 1.0, 0.0);
		pt s = pt(175,225);
		name(s);
		//circleSegment(300);

		glColor3f(1.0, 0.0, 1.0);
		output(200,150,"Name");
	}
	//draw house scene
	if (drawHouse == true) {
				
		glColor3f(0.0, 1.0, 1.0);
		pt nu = pt(175,175);

		house(nu,0);
		
		glColor3f(1.0, 1.0, 0.0);
		output(200,150,"House");
	}	
	//draw and fill house scene
	if(drawFillHouse == true) {
		pt nu = pt(175,175);
		house(nu,1);

		glColor3f(1.0,1.0,1.0);
		output(200,150,"Filled House");
	}
    /* swap buffers */
    glutSwapBuffers();
}

void reshape (int w, int h) {
	/* set the viewport */
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	/* Matrix for projection transformation */
	glMatrixMode (GL_PROJECTION); 

	/* replaces the current matrix with the identity matrix */
	glLoadIdentity ();

	/* Define a 2d orthographic projection matrix */
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

/*******************************************************************/

int main(int argc, char** argv) {

	/* deal with any GLUT command Line options */
    glutInit(&argc, argv);

	/* create an output window */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500, 500);

	/* set the name of the window and try to create it */
    glutCreateWindow("Shane Lopez, CS 488 - Project 1");

	/* specify clear values for the color buffers */
	glClearColor (0.0, 0.0, 0.0, 1.0);

    /* Receive keyboard inputs */
    glutKeyboardFunc (Keyboard);

    /* assign the display function */
    glutDisplayFunc(display);

	/* assign the idle function */
    glutIdleFunc(display);

    /* sets the reshape callback for the current window */
	glutReshapeFunc(reshape);

    /* enters the GLUT event processing loop */
    glutMainLoop();

    return (EXIT_SUCCESS);
}


