#include <time.h>
#include <vector>
#include "./ball.h"
// #include <iostream>

// Handy ball class implementation
/*
  Ball has: 
  - Point
  - Color
  - Circle
*/
Ball::Ball() {  // default constructor
  colr = Color();
  point = Point2();
  radius = 30.0;
}

Ball::Ball(Color col, Point2 init) {  // constructor
  colr = col;
  point = init;
  radius = 30.0;
}

void Ball::DrawBall(Point2 current) {
  Point2 pt = current;
  float x, y, x2, y2;
  x = pt.x;
  y = pt.y;
  GLfloat r = this->colr.c[0];
  GLfloat g = this->colr.c[1];
  GLfloat b = this->colr.c[2];
  glColor3f(r, g, b);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for (float angle = 1.0f; angle < 361.0f; angle += 0.2) {
    x2 = x+sin(angle)*this->radius;
    y2 = y+cos(angle)*this->radius;
    glVertex2f(x2, y2);
  }
  glEnd();
}

Point2 Ball::CurrentPos() {
  return this->point;
}

std::vector<Point2> Ball::Hit(float* mag, float* dir) {
  float start_time = glutGet(GLUT_ELAPSED_TIME);
  float initial_velocity = (*mag)*.01;
  float velocity = initial_velocity;
  float acceleration = -1;
  float angle = (*dir)*(180/3.141592);
  float xinitial = this->point.x;
  float yinitial = this->point.y;
  std::vector<Point2> res;
  std::cout << *mag << " is magnitude " << angle << "direction" << std::endl;
  while (velocity > 0) {
    float elapsed_time = (glutGet(GLUT_ELAPSED_TIME) - start_time);
    velocity = acceleration*elapsed_time + initial_velocity;
    float vy = initial_velocity*sin(angle);
    float vx = initial_velocity*cos(angle);
    float xnext = this->point.x
                      + (this->xdir)*(0.5)*acceleration*pow(elapsed_time, 2)
                      + vx*elapsed_time;
    float ynext = this->point.y
                      + (this->ydir)*(0.5)*acceleration*pow(elapsed_time, 2)
                      + vy*elapsed_time;
    if (xnext <= 80 || xnext >= 720) {
      this->xdir = (-1)*this->xdir;
      xnext = this->point.x
                      + (this->xdir)*(0.5)*acceleration*pow(elapsed_time, 2)
                      + vx*elapsed_time;
    }
    this->point.x = xnext;
    if (ynext <= 80 || ynext >= 520) {
      this->xdir = (-1)*this->xdir;
      ynext = this->point.y
                      + (this->ydir)*(0.5)*acceleration*pow(elapsed_time, 2)
                      + vy*elapsed_time;
    }
    this->point.y = ynext;
    res.push_back(this->point);
    std::cout << initial_velocity << std::endl;
    std::cout << velocity << std::endl;
    glutPostRedisplay();
  }
  return res;
}
/*
  bal - ball to draw
  k - translation on y axis
  r - radius
  h - translation on x axis
  All it does now is draw a circle -- not a BALL
  Needs to be modified for BALL class
*/
// void Ball::DrawBall(Ball bal, GLfloat k, GLfloat r, GLfloat h) {
//   // Point2 pt = &(bal.point);
//   GLfloat _x, _y;
//   glBegin(GL_LINES);
//   for (int i = 0; i < 180; i++) {
//     _x = r * cos(i) - h;
//     _y = r * sin(i) + k;
//     glVertex2f(_x + k, _y - h);
//     _x = r * cos(i + 1.0) - h;
//     _y = r * sin(i + 1.0) + k;
//     glVertex2f(_x + k, _y - h);
//   }
//   glEnd();
// }
