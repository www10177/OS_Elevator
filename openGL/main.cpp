//
//  main.cpp
//  2D Game Programming Tutorial
//
//  Created by UglyMan.nothinglo on 13/10/7.
//  Copyright (c) 2013年 UglyMan.nothinglo. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
//#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>
#include "Timer.h"
#include "elevator.h"
#include "Vec.h"
#include "passenger.h"
#define NumberOfFloors 8 

/*CONFIGS*/
const int GAME_ONE_SHOT_TIME = 33;
int screenWidth = 800 , screenHeight = 800;
const int floorHeight = screenHeight / NumberOfFloors;
const float floorColor1[3] = {248.0/255,219.0/255,14.0/255};
const float floorColor2[3] = {228.0/255,219.0/255,141.0/255};

/*VARIABLES*/
elevator* ele; 
passenger* passengers;

/*FUNCTIONS*/
void myDisplay();
void updateTheGame(int value);
void myReshape(int w, int h);
void drawFloor();
void init();

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 30);
	glutCreateWindow("OS Elevator Problem");
	init();
	updateTheGame(0);
	glutMainLoop();
	return 0;
}

void myDisplay(void) {/*{{{*/
	glClear(GL_COLOR_BUFFER_BIT);
	drawFloor();
	passengers->display();
	ele->display();
	glutSwapBuffers();
}/*}}}*/

void updateTheGame(int value) { /*{{{*/
	glutPostRedisplay();
	glutTimerFunc(GAME_ONE_SHOT_TIME, updateTheGame, 0);
}/*}}}*/

void drawFloor(){/*{{{*/
	for (int i = 0; i <NumberOfFloors ; ++i) {
		if(i%2)
			glColor3f(floorColor1[0],floorColor1[1],floorColor1[2]);
		else
			glColor3f(floorColor2[0],floorColor2[1],floorColor2[2]);
		std::cout << floorHeight*i<<"  " << screenWidth<< "  " << floorHeight*(i+1) << std::endl; 
		glRectf(0,floorHeight*i,screenWidth,floorHeight*(1+i));
	}
}/*}}}*/

void init() {/*{{{*/
	/*openGL init*/
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	/*Var Init*/
//	ele = new elevator();
	ele = new elevator(Vec3(screenWidth/2,0,1));
	passengers= new passenger();
}/*}}}*/

void myReshape(int w, int h) {/*{{{*/
	screenWidth  = w;
	screenHeight = h;
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat)screenWidth, 0.0, (GLfloat)screenHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}/*}}}*/
