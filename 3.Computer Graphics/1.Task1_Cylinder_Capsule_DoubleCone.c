//#include "stdafx.h"

// File:        Task1_l630003001.cpp
// Author:      Matt Daisley, modified by Andy Zha
// Created:     4/25/2012, Modified in 3/8/2019
// Project:     Source code for Make a cylinder, a capsule and a double cone in OpenGL
// Description: Creates an OpenGL window and draws a 3D cube
//              That the user can rotate using the arrow keys
// 
// Controls:    Left Arrow  - Rotate Left
//              Right Arrow - Rotate Right
//              Up Arrow    - Rotate Up
//              Down Arrow  - Rotate Down     

// ----------------------------------------------------------
// Includes
// ----------------------------------------------------------
//#include <stdio.h>
//#include <stdarg.h>
#include <math.h>
//#define GL_GLEXT_PROTOTYPES

#include <GL\glut.h>


// ----------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------
void display();
void specialKeys();

// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
double rotate_y = 0;
double rotate_x = 0;
#define M_PI 3.14159265357

// ----------------------------------------------------------
// Drawing cylinder, cylinder is 1 top plane + 1 bottom plane + 1 side
// ----------------------------------------------------------
void cylinder()
{
	float v[121][3];//points, sector+1
	float Radius = 0.6;
	int sectors = 10;
	int rings = 10; //sbudivide the sphere
	float height = 10;
	float x, y, z;
	for (int i = 0; i < rings + 1; i++) //0 to half of rings 
	{
		for (int j = 0; j < sectors + 1; j++) // half to the rings
		{
			/* "i" will not change in the inner loop, thus the y and x will be the same line */
			y = Radius * sin(i * 2 * M_PI / sectors);
			x = Radius * cos(i * 2 * M_PI / sectors);
			
			/*
			  "j" means height of the cylinder, "j/height" will increase gradually
			  when j = sector, a verticle line will be drawn.
			  lastly, "-0.5" is to move the whole cylinder to center so it looks better.
			*/
			z = j / height - 0.5;
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	int np = 0;
	int index[484];
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index[4 * np + 0] = i * (sectors + 1) + j;
			index[4 * np + 1] = i * (sectors + 1) + (j + 1);
			index[4 * np + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index[4 * np + 3] = (i + 1) * (sectors + 1) + j;
			np++;
		}
	}

	for (int i = 0; i < np; i++) { // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(v[index[4 * i + 0]]);
		glVertex3fv(v[index[4 * i + 1]]);
		glVertex3fv(v[index[4 * i + 2]]);
		glVertex3fv(v[index[4 * i + 3]]);
		glEnd();
	}
}

// ----------------------------------------------------------
// Drawing capsule, a capsule is top half of a sphere + bottom half of a sphere + side of a cylinder
// This is the first version of capsule(only two halves of sphere)
// *** We have the second one (with a cylinder) below ***
// ----------------------------------------------------------
void capsule()
{
	float v[141][3];//points, sector+1
	float Radius = 0.6;
	int sectors = 10;
	int rings = 11; //sbudivide the sphere
	float x, y, z;

	/* here "i" is related to the height, we want to draw the first half of sphere, thus we set i in range
	0-5 firstly */
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			/* 
			  to make the first half of sphere far away from the second half of sphere, we move this part 
			  to a upper place, thus we add 0.3 in y for all point.
			*/
			y = Radius * cos(i*M_PI / rings) + 0.3;
			x = Radius * sin(i*M_PI / rings)*cos(2 * j*M_PI / sectors);
			z = Radius * sin(i*M_PI / rings)*sin(2 * j*M_PI / sectors);
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	/* and then, set i in range 6-11, this is the second half of sphere*/
	for (int i = 6; i < 12; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			/* same reason as above, we move down the second half of sphere */
			y = Radius * cos(i*M_PI / rings) - 0.3;
			x = Radius * sin(i*M_PI / rings)*cos(2 * j*M_PI / sectors);
			z = Radius * sin(i*M_PI / rings)*sin(2 * j*M_PI / sectors);
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	int np = 0;
	int index[600];// we changed number of index because we are worrying about overflow, actually this value can be smaller
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index[4 * np + 0] = i * (sectors + 1) + j;
			index[4 * np + 1] = i * (sectors + 1) + (j + 1);
			index[4 * np + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index[4 * np + 3] = (i + 1) * (sectors + 1) + j;
			np++;
		}
	}

	for (int i = 0; i < np; i++) { // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(v[index[4 * i + 0]]);
		glVertex3fv(v[index[4 * i + 1]]);
		glVertex3fv(v[index[4 * i + 2]]);
		glVertex3fv(v[index[4 * i + 3]]);
		glEnd();
	}
}

// ----------------------------------------------------------
// This is the second version of capsule, just added a cylinder in the middle
// To make this is because the requirement of assignment is:
// "A capsule = top half of a sphere + bottom half of a sphere + side of a cylinder"
// I can't make sure whether the first version above meets this requirement
// thus I add a cylinder in the second version.
// ----------------------------------------------------------
void capsuleWithCylinder()
{
	/* first version of capsule start */

	float v[141][3];//points, sector+1
	float Radius = 0.6;
	int sectors = 10;
	int rings = 11; //sbudivide the sphere
	float x, y, z;
	float x2, y2, z2;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			y = (Radius * cos(i*M_PI / rings)) + 0.3;
			x = Radius * sin(i*M_PI / rings)*cos(2 * j*M_PI / sectors);
			z = Radius * sin(i*M_PI / rings)*sin(2 * j*M_PI / sectors);
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	for (int i = 6; i < 12; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			y = (Radius * cos(i*M_PI / rings)) - 0.3;
			x = Radius * sin(i*M_PI / rings)*cos(2 * j*M_PI / sectors);
			z = Radius * sin(i*M_PI / rings)*sin(2 * j*M_PI / sectors);
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	int np = 0;
	int index[600];
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index[4 * np + 0] = i * (sectors + 1) + j;
			index[4 * np + 1] = i * (sectors + 1) + (j + 1);
			index[4 * np + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index[4 * np + 3] = (i + 1) * (sectors + 1) + j;
			np++;
		}
	}

	for (int i = 0; i < np; i++) { // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(v[index[4 * i + 0]]);
		glVertex3fv(v[index[4 * i + 1]]);
		glVertex3fv(v[index[4 * i + 2]]);
		glVertex3fv(v[index[4 * i + 3]]);
		glEnd();
	}

	/* first version of capsule end */

	/* cylinder part start*/

	float v2[121][3];//points, sector+1
	rings = 10;
	sectors = 10;
	
	/* 
	  to make sure the cylinder is in the middle of capsule, we need to calculate the height of cylinder
	*/
	float bottom = (Radius * cos(6 * M_PI / 11)) - 0.3;
	float top = Radius * cos(5 * M_PI / 11) + 0.3;
	/*
	  "10" means the number of floor of cylinder, it is same as the number of the inner loop below 
	  it can be replace by "sectors"
	*/
	float addUnit = (top - bottom) / 10;
	/*
	  at first, the height equal to "bottom", which is the top of half sphere below
	  after add "addUnit" for 10 times, it goes to "top", which is the bottom part of half sphere on the top.
	*/
	y2 = bottom;
	
	for (int i = 0; i < rings + 1; i++) //0 to half of rings 
	{
		for (int j = 0; j < sectors + 1; j++) // half to the rings
		{
			z2 = Radius * sin(i * 2 * M_PI / sectors);
			x2 = Radius * cos(i * 2 * M_PI / sectors);
			v2[i*(sectors + 1) + j][0] = x2;
			v2[i*(sectors + 1) + j][1] = y2;
			v2[i*(sectors + 1) + j][2] = z2;
			y2 += addUnit;
		}
		y2 = bottom;
	}

	int np2 = 0;
	int index2[484];
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index2[4 * np2 + 0] = i * (sectors + 1) + j;
			index2[4 * np2 + 1] = i * (sectors + 1) + (j + 1);
			index2[4 * np2 + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index2[4 * np2 + 3] = (i + 1) * (sectors + 1) + j;
			np2++;
		}
	}

	for (int i = 0; i < np2; i++) { // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(v2[index2[4 * i + 0]]);
		glVertex3fv(v2[index2[4 * i + 1]]);
		glVertex3fv(v2[index2[4 * i + 2]]);
		glVertex3fv(v2[index2[4 * i + 3]]);
		glEnd();
	}
	/* cylinder part end */
}

// ----------------------------------------------------------
// This function is to draw a double cone
// ----------------------------------------------------------
void doubleCone()
{
	float v[121][3];//points, sector+1
	float h = -0.5;// we set h = -0.5 is because we want the graphic is in the middle of viewport
	int rings = 10;
	int sectors = 10;
	float x, y, z;
	for (int i = 0; i < rings + 1; i++, h += 0.1)
	{
		for (int j = 0; j < sectors + 1; j++)
		{
			/* firstly, we set y = h, then using h as radius to draw a circle */
			y = h;
			x = fabs(h) * cos(2 * j * M_PI / sectors);
			z = fabs(h) * sin(2 * j * M_PI / sectors);
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}
	int np = 0;
	int index[484];
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index[4 * np + 0] = i * (sectors + 1) + j;
			index[4 * np + 1] = i * (sectors + 1) + (j + 1);
			index[4 * np + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index[4 * np + 3] = (i + 1) * (sectors + 1) + j;
			np++;
		}
	}

	for (int i = 0; i < np; i++) { // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(v[index[4 * i + 0]]);
		glVertex3fv(v[index[4 * i + 1]]);
		glVertex3fv(v[index[4 * i + 2]]);
		glVertex3fv(v[index[4 * i + 3]]);
		glEnd();
	}
}

// ----------------------------------------------------------
// This function is to draw a cylinder with a "plane", which is a surface on the top and bottom of a cylinder
// I make it as a octagon so it looks like Marisa's Mini Hakkero, kira~¡î
// ----------------------------------------------------------
void cylinderWithPlane()
{
	float v[121][3];//points, sector+1
	float Radius = 0.4;
	int sectors = 8;
	int rings = 10; //sbudivide the sphere
	float height = 5;
	float x, y, z;
	for (int i = 0; i < rings + 1; i++) //0 to half of rings 
	{
		for (int j = 0; j < sectors + 1; j++) // half to the rings
		{
			y = Radius * sin(i * 2 * M_PI / sectors);
			x = Radius * cos(i * 2 * M_PI / sectors);
			z = j / height * 0.2;
			v[i*(sectors + 1) + j][0] = x;
			v[i*(sectors + 1) + j][1] = y;
			v[i*(sectors + 1) + j][2] = z;
		}
	}

	int np = 0;
	int index[484];
	for (int i = 0; i < rings; i++) { // r: index for ring
		for (int j = 0; j < sectors; j++) { // s: index for sector
			index[4 * np + 0] = i * (sectors + 1) + j;
			index[4 * np + 1] = i * (sectors + 1) + (j + 1);
			index[4 * np + 2] = (i + 1) * (sectors + 1) + (j + 1);
			index[4 * np + 3] = (i + 1) * (sectors + 1) + j;
			np++;
		}
	}

	for (int i = 0; i < np; i++)
	{ // i: index for polygon
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3fv(v[index[4 * i + 0]]);
		glVertex3fv(v[index[4 * i + 1]]);
		glVertex3fv(v[index[4 * i + 2]]);
		glVertex3fv(v[index[4 * i + 3]]);
		glEnd();
	}
	
	/* draw the bottom of the cylinder, each loop will draw a line*/
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < sectors; i++)
	{
		glVertex3f(0, 0, 0);
		glVertex3f(Radius * cos(i * 2 * M_PI / sectors), Radius * sin(i * 2 * M_PI / sectors), 0);
	}
	glEnd();

	/* draw the top of the cylinder, each loop will draw a line */
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < sectors; i++)
	{
		glVertex3f(0, 0, 8 / height * 0.2);
		glVertex3f(Radius * cos(i * 2 * M_PI / sectors), Radius * sin(i * 2 * M_PI / sectors), 8 / height * 0.2);
	}
	glEnd();
}

// ----------------------------------------------------------
// display() Function
// ----------------------------------------------------------
void display() {

	// Clear screen and Z-buffer
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

	cylinderWithPlane();

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
	glutCreateWindow("Color Cube");

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
