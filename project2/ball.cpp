#include <time.h>
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

void Ball::DrawBall() {
  Point2 pt = this->point;
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

void Ball::MoveBall(float mag, float dir) {
  float acc = -1 * 0.5;  // acceleration
  float v = 50.0;
  std::cout << "in MOVEBALL" << std::endl;
  // Ball b = this;
  glClear(GL_COLOR_BUFFER_BIT);
  float start_time = glutGet(GLUT_ELAPSED_TIME);
  std::cout << "start_time: " << start_time << std::endl;
  float current_time, delta_t;

  while (delta_t < 20) {
      current_time = glutGet(GLUT_ELAPSED_TIME);
      std::cout << "delta_t: " << delta_t << std::endl;
      delta_t = (current_time - start_time) / 1000.0f;
      this->point.x = (0.5) * acc * pow(delta_t, 2)
                      + v * delta_t + this->point.x;
      this->point.y = (0.5) * acc * pow(delta_t, 2)
                      + v * delta_t + this->point.y;
      (*this).DrawBall();
      v = acc * delta_t + v;
      glutPostRedisplay();
  }
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

void Ball::Move() {
  std::cout << "inside move" << std::endl;
  float xnext;
  float ynext;
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
    (*this).Deaccelerate();
    std::cout << this->xdir << std::endl;
    glutPostRedisplay();
  }
}

void Ball::Hit(float* mag, float* dir) {
  float start_time = glutGet(GLUT_ELAPSED_TIME);
  float initial_velocity = (*mag)*.1;
  float velocity = initial_velocity;
  float acceleration = -.1;
  float angle = (*dir);
  float xinitial = this->point.x;
  float yinitial = this->point.y;
  std::cout << *mag << " is magnitude " << angle << "direction" << std::endl;
  float vy = initial_velocity*sin(angle);
  float vx = initial_velocity*cos(angle);
  this->xvel = vx;
  this->yvel = vy;
  (*this).Move();
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
