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

void Ball::Deaccelerate() {
  this->xvel /= 1.001;
  this->yvel /= 1.001;
  if (abs(this->xvel) <= .0001) {
    this->xvel = 0;
  }
  if (abs(this->yvel) <= .0001) {
    this->yvel = 0;
  }
}

std::vector<Point2> Ball::Move() {
  std::cout << "inside move" << std::endl;
  float xnext;
  float ynext;
  std::vector<Point2> res;
  while (this->yvel != 0 || this->yvel != 0) {
    std::cout << "inside while" << std::endl;
    xnext = this->point.x + this->xvel;
    ynext = this->point.y + this->yvel;
    if (xnext <= 80 || xnext >= 720) {
      this->xvel = (-1)*(this->xvel);
    } else {
      this->point.x = xnext;
    }
    this->point.x = xnext;
    if (ynext <= 80 || ynext >= 520) {
      this->yvel = (-1)*(this->yvel);
    } else {
      this->point.y = ynext;
    }
    this->point.y = ynext;
    res.push_back(this->point);
    (*this).Deaccelerate();
    std::cout << this->xdir << std::endl;
    glutPostRedisplay();
  }
  return res;
}

std::vector<Point2> Ball::Hit(float* mag, float* dir) {
  float start_time = glutGet(GLUT_ELAPSED_TIME);
  float initial_velocity = (*mag)*.5;
  float velocity = initial_velocity;
  float acceleration = -1;
  float angle = (*dir);
  float xinitial = this->point.x;
  float yinitial = this->point.y;
  // std::cout << *mag << " is magnitude " << angle << "direction" << std::endl;
  float vy = initial_velocity*sin(angle);
  float vx = initial_velocity*cos(angle);
  this->xvel = vx;
  this->yvel = vy;
  return (*this).Move();
}
