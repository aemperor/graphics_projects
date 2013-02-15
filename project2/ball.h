#ifndef __BALL_H__
#define __BALL_H__

#include <math.h>
#include <iostream>
#include <vector>
#include "./geom2.h"
#include "./common.h"

// A handy Ball class
class Ball {
  public:
    Color colr;
    Point2 point;
    double radius;
    double xpos;
    double ypos;
    double xdir;
    double ydir;
    Ball();  // default constructor
    Ball(Color col, Point2 init);  // constructor
    std::vector<Point2> Hit(float* mag, float* dir);
    void DrawBall(Point2 current);
    void MoveBall(float mag, float dir);
    Point2 CurrentPos();
    double GetRadius(void);
};

#endif
