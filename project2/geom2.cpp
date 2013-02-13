#include "./geom2.h"

Point2::Point2() {  // default constructor
  x = 0;
  y = 0;
}

Point2::Point2(GLfloat _x, GLfloat _y) {  // constructor
  x = _x;
  y = _y;
}

Color::Color() {  // default constructor
  c[0] = 0.0;
  c[1] = 0.0;
  c[2] = 0.0;
}

Color::Color(GLfloat _r, GLfloat _g, GLfloat _b) {  // constructor
  c[0] = _r;
  c[1] = _g;
  c[2] = _b;
}
