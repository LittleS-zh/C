//#include "stdafx.h"

// File:        snowflake.cpp
// Author:      Matt Daisley, modified by LittleS
// Created:     3/25/2012, modified at 3/16/2019
// Project:     Source code for Make a snowflake in OpenGL
// Description: Creates an OpenGL window and draws a 3D snowflake
//              That the user can rotate using the arrow keys
// 
// Controls:    Left Arrow  - Rotate Left
//              Right Arrow - Rotate Right
//              Up Arrow    - Rotate Up
//              Down Arrow  - Rotate Down     

// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
#include <stdio.h>
//#include <stdarg.h>
#include <math.h>
//#define GL_GLEXT_PROTOTYPES

#include <GL\glut.h>


// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void drawKoch(double* pt1, double* pt2, int n);
void drawLine(double* pt1, double* pt2);
// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y = 0;
double rotate_x = 0;
#define M_PI 3.14159265357

void snowflake()
{
	double p1[2], p2[2], p3[2];

	/* free to change here */
	p1[0] = 0;
	p1[1] = 0;
	p2[0] = 0.8;
	p2[1] = 0;

	/*
	  drawing p3 by using rotation
	  this part may be replaced by "glRotate" function
	  firstly, p2 is relative to p1, we want the rotation center is (0,0), thus we move it relative to 0.
	  (here we set p1 is original point, thus the moving part is no use here, but it will be useful when p1 is not original point)
	  then, p2 rotate 60 degree counterclockwise (here, "(p2[0] - p1[0]) / 2" means multiply cos 60, (p2[1] - p1[1]) * 1.732 / 2 means multiply sin 60)
	  for more information, please refer to "how to rotate a graphic using matrix"
	  lastly, move the new point back
	*/
	p3[0] = (p2[0] - p1[0]) / 2 - (p2[1] - p1[1]) * 1.732 / 2 + p1[0];
	p3[1] = (p2[1] - p1[1]) / 2 + (p2[0] - p1[0]) * 1.732 / 2 + p1[1]; //using 1.732 is not so precise

	/* 
	  this is to draw a inner flake 
	  which is told by my friend, director Wu in UIC firstly
	  if you are interesting with it you can run it.
	*/
	/*drawKoch(p1, p2, 10);
	drawKoch(p2, p3, 10);
	drawKoch(p3, p1, 10);*/

	/* draw the main part of the snowflake, let it go. */
	drawKoch(p1, p3 ,5);
	drawKoch(p3, p2 ,5);
	drawKoch(p2, p1 ,5);
}

void drawKoch(double * pt1, double * pt2, int n)
{
	double p1[2], p2[2], p3[2], p4[2], p5[2];

	p1[0] = pt1[0];
	p1[1] = pt1[1];
	p2[0] = pt2[0];
	p2[1] = pt2[1];

	/* stop condition: no need to divide anymore */
	if (n == 1)
		drawLine(p1, p2);

	/* need to divide */
	if (n > 1)
	{
		/* 
		  originally, p1, p2 form a line
		  we seperate this line into equally three part, then generate p3 and p4
		  lastly, we draw p5 using the method that we used in sonwflake method, p3, p4 and p5 can form a little triangle
		  (when show, the bottom of the triangle missed because we link p1 to p5 using GL_LINES)
		*/
		p3[0] = p1[0] + (p2[0] - p1[0]) / 3;
		p3[1] = p1[1] + (p2[1] - p1[1]) / 3;
		p4[0] = p1[0] + 2 * (p2[0] - p1[0]) / 3;
		p4[1] = p1[1] + 2 * (p2[1] - p1[1]) / 3;

		p5[0] = (p4[0] - p3[0]) / 2 - (p4[1] - p3[1]) * 1.732 / 2 + p3[0];
		p5[1] = (p4[1] - p3[1]) / 2 + (p4[0] - p3[0]) * 1.732 / 2 + p3[1];

		/* recursive */
		drawKoch(p1, p3, n - 1);
		drawKoch(p3, p5, n - 1);
		drawKoch(p5, p4, n - 1);
		drawKoch(p4, p2, n - 1);
	}
}

// ----------------------------------------------------------
// Draw a line between two points
// ----------------------------------------------------------
void drawLine(double* pt1, double* pt2)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(pt1[0], pt1[1]);
	glVertex2f(pt2[0], pt2[1]);
	glEnd();
}

void display() {

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Other Transformations
	// glTranslatef( 0.1, 0.0, 0.0 );      // Not included
	// glRotatef( 180, 0.0, 1.0, 0.0 );    // Not included

	// Rotate when user changes rotate_x and rotate_y
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	// Other Transformations
	// glScalef( 2.0, 2.0, 0.0 );          // Not included
	snowflake();

	glFlush();
	glutSwapBuffers();

}
// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys(int key, int x, int y) {

	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;

	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;

	else if (key == GLUT_KEY_UP)
		rotate_x += 5;

	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;

	//  Request display update
	glutPostRedisplay();

}

// ----------------------------------------------------------
// main() function
// ----------------------------------------------------------
int main(int argc, char* argv[]) {

	//  Initialize GLUT and process user parameters
	glutInit(&argc, argv);

	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutCreateWindow("Fractal");

	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Callback functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);

	//  Pass control to GLUT for events
	glutMainLoop();

	//  Return to OS
	return 0;
}
