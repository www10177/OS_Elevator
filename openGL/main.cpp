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
#include "Vec3.h"

const int GAME_ONE_SHOT_TIME = 33;
int screenWidth = 800 , screenHeight = 600;
elevator* ele; 

void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT);
	ele->display();
    glutSwapBuffers();
}
void updateTheGame(int value) {
    glutPostRedisplay();
    glutTimerFunc(GAME_ONE_SHOT_TIME, updateTheGame, 0);
}

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

void init() {
	/*openGL init*/
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);

	/*Var Init*/
ele = new elevator();
	
}

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

