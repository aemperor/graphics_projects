#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

void Init();
void Display();
void Keyboard(unsigned char key, int x, int y);
void DisableLighting();
void EnableLighting();
void DrawAxis();
void DrawAxes();
void PrintMatrix(GLint matrix);
void PrintMatrix();
void LoadMatrix(GLfloat* m);
void MultMatrix(GLfloat* m);

void Display1();
void Display2();
void Display3();
void Display4();
void Display5();
void Display6();
void Display7();
void Display8();
void Display9();
void Display10();
void Display11();
void Display12();
void Display13();

#endif
