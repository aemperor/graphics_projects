#ifndef __GEOM2_H__
#define __GEOM2_H__

#include <iostream>
#include <vector>
#include "./common.h"

// Handy geometry classes and functions
class Point2 {
  public:
    GLfloat x, y;
    Point2();  // default constructor
    Point2(GLfloat _x, GLfloat _y);  // constructor
};

class Color {
  public:
    GLfloat c[3];  // color array
    Color();  // default constructor
    Color(GLfloat _r, GLfloat _g, GLfloat _b);  // constructor
};

#endif
