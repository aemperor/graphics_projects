#include <math.h>
#include <iostream>
#include <vector>
#include "./common.h"
#include "./ball.h"

using namespace std;

GLint window_width = 800;
GLint window_height = 600;

Point2 mouse_pnt(0, 0);
Point2 mouse_pnt2(0, 0);
vector<Point2> mouse_line;
int mouse_move;

Color col = Color(1.0, 1.0, 1.0);
Point2 pt = Point2(window_width/2, window_height/2);
Ball b = Ball(col, pt);
Color col2 = Color(0.8f, 0.0, 0.0);
Point2 pt2 = Point2(window_width/2 + 100, window_height/2 + 100);
Ball r = Ball(col2, pt2);

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(0, 0);
  gluOrtho2D(0, window_width,
              0, window_height);
  glMatrixMode(GL_MODELVIEW);
}

void DrawPocket(float x1, float y1, double radius) {
  float x2, y2;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x1, y1);
  for (float angle = 1.0f; angle < 361.0f; angle += 0.2) {
    x2 = x1+sin(angle)*radius;
    y2 = y1+cos(angle)*radius;
    glVertex2f(x2, y2);
  }
  glEnd();
}


void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.5f, 0.35f, 0.05f);
  // drawing table
  glBegin(GL_POLYGON);
  glVertex2f(0, 0);
  glVertex2f(window_width, 0);
  glVertex2f(window_width, window_height);
  glVertex2f(0, window_height);
  glEnd();

  int edge_width = 50;
  glColor3f(0.0, 0.5, 0.0);
  glBegin(GL_POLYGON);
  glVertex2f(0 + edge_width, 0 + edge_width);
  glVertex2f(window_width - edge_width, 0 +  edge_width);
  glVertex2f(window_width - edge_width, window_height - edge_width);
  glVertex2f(0 + edge_width, window_height - edge_width);
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


  int pocket_size = 30.0;
  glColor3f(0.0, 0.0, 0.0);
  DrawPocket(0 + edge_width, 0 + edge_width, pocket_size);
  DrawPocket(window_width - edge_width, 0 + edge_width, pocket_size);
  DrawPocket(window_width - edge_width,
             window_height - edge_width, pocket_size);
  DrawPocket(0 + edge_width, window_height - edge_width, pocket_size);
  DrawPocket(window_width/2 , 0 + edge_width, pocket_size);
  DrawPocket(window_width/2 ,
             window_height - edge_width, pocket_size);
  // draw white ball
  b.DrawBall();
  cout << b.CurrentPos().x << b.CurrentPos().y << endl;
  cout << mouse_pnt2.x << mouse_pnt2.y << endl;
  // draw red ball
  r.DrawBall();
  // draw mouse line
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex2f(b.CurrentPos().x, b.CurrentPos().y);
  glVertex2f(mouse_pnt2.x, mouse_pnt2.y);
  glEnd();

  // glutPostRedisplay();
  glFlush();
  glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      mouse_pnt2 = Point2(x, y);
      glutPostRedisplay();
    } else {
      y = window_height - y;
      mouse_pnt2 = Point2(x, y);
      float magnitude = sqrt(pow((b.CurrentPos().x - mouse_pnt2.x), 2.0)
      + pow((b.CurrentPos().y - mouse_pnt2.y), 2.0));
      // float dir = atan((b.CurrentPos().x - mouse_pnt2.x)
      // /(b.CurrentPos().y - mouse_pnt2.y));
      // b.Hit(&magnitude, &dir);
      float dir = atan((b.CurrentPos().y - mouse_pnt2.y)
      /(b.CurrentPos().x - mouse_pnt2.x));
      b.Hit(&magnitude, &dir);
      // cout << "calling MoveBall" << endl;
      // cout << "direction: " << dir << endl;
      // b.MoveBall(magnitude, dir);
    }
  }
  glutPostRedisplay();
}
void MouseMotion(int x, int y) {
  y = window_height - y;
  mouse_pnt2 = Point2(x, y);
  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(EXIT_SUCCESS);
      break;
  }
}


// void Idle() {
// }


int main(int argc, char** argv) {
  cout << "Billiards!" << endl;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Billiards");
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);
  // glutIdleFunc(Idle);
  Init();
  glutMainLoop();
}
