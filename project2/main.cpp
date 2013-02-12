#include <math.h>
#include <iostream>
#include <vector>
#include "./common.h"
#include "./ball.h"

using namespace std;

GLint window_width = 800;
GLint window_height = 600;

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(0, 0);
  gluOrtho2D(-1*window_width, window_width,
              window_height, -1*window_height);  // switched so y grows upward
  glMatrixMode(GL_MODELVIEW);
}


void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.5f, 0.35f, 0.05f);
  glBegin(GL_POLYGON);
  glVertex2f(-1*window_width, window_height);
  glVertex2f(window_width, window_height);
  glVertex2f(window_width, -1*window_height);
  glVertex2f(-1*window_width, -1*window_height);
  glEnd();

  glColor3f(0.0, 0.5, 0.0);
  glBegin(GL_POLYGON);
  glVertex2f(-1*window_width + 100, window_height - 100);
  glVertex2f(window_width - 100, window_height - 100);
  glVertex2f(window_width - 100, -1*window_height + 100);
  glVertex2f(-1*window_width + 100, -1*window_height + 100);
  glEnd();

  float x1, y1, x2, y2;
  float angle;
  double radius = 30.0;
  x1 = -1*window_width + 100;
  y1 = window_height - 100;
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x1, y1);
  for (angle = 1.0f; angle < 361.0f; angle += 0.2) {
    x2 = x1+sin(angle)*radius;
    y2 = y1+cos(angle)*radius;
    glVertex2f(x2, y2);
  }
  glEnd();


  glFlush();
  glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      cout << "mouse" << endl;
    }
  }

  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(EXIT_SUCCESS);
      break;
  }
}

int main(int argc, char** argv) {
  cout << "Billiards!" << endl;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("Billiards");
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
  Init();
  glutMainLoop();
}
