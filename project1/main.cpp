#include <math.h>
#include <iostream>
#include <vector>

#include "./common.h"
#include "./command.h"

using namespace std;

void Init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // TODO: Change this call so that the drawing methods work on a canvas
  // that is 200x200 units with the origin (0, 0) at the center of the
  // canvas.
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

struct Color {
  Color(GLfloat _r, GLfloat _g, GLfloat _b) {
    c[0] = _r;
    c[1] = _g;
    c[2] = _b;
  }
  operator GLfloat*() { return c; }
  GLfloat c[3];
};

struct Point2 {
  Point2(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
  GLfloat x, y;
};

// debugging
// struct Point2 {
//   Point2(GLint _x, GLint _y) {
//     x[0] = _x;
//     x[1] = _y;
//   }
//   operator GLint*() { return x; }
//   GLint x[2];
// };

vector<Point2> points;

// Colors
const Color zero = Color(0, 0, 0);  // black
const Color one = Color(1, 0, 0);  // red
const Color two = Color(0, 1, 0);  // green
const Color three = Color(0, 0, 1);  // blue

Color color = zero;
float direction[2] = {1.0, 1.0};
double degree = 90;  // default degree is 90 degrees (since forward up is 90)
int penState = 0;  // down = 0; up = 1

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO: Change to draw according to turtle commands given
  // Draw a red triangle.
  if (penState == 0) {
    for (int i = 0; i < points.size(); ++i) {
      glColor3fv(color);
      glBegin(GL_LINE_STRIP);
      glVertex2i(points[i].x, points[i].y);
      cout << "(x, y) = (" << points[i].x << ", " << points[i].y << ")" << endl;
    }
    glEnd();
  }
  /*glBegin(GL_LINES);
  glColor3fv(color);
  glVertex2f(-0.5, -0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(0.0, 0.5);
  glVertex2f(0.0, 0.5);
  glVertex2f(-0.5, -0.5);
  glEnd();*/

  glFlush();
}

/*
  Function to round number to specific precision
*/
float round(float f) {
    return floor(f*5 + 0.5)/5;
}

/*
  Function changes the direction of the line based on the angle given
  stores the new direction in the direction vector
  dir = -1 //right 
  dir = 1 //left

  degrees for different directions:
  forward : 90 (up)
  right : 0
  left : 180
  down : 270

  This function will first multiply the desired angle with dir, 
  and adds to the current degree
  ex.) the current degree is 90 , next command is right 90 and forward 30, current coordinate is (0, 30)
      degree = 90 * (-1) + 90 = 0
	  dir for x - coordinate = cos (0) = 1
	  dir for y - coordinate = sin (0) = 0
	  Then forward 30 
      last. x + (1) * 30 => 30
      last. y + (0) * 30 => 30
      The new coordinate becomes (30, 30)
*/
void rotate(int angle, int dir) {
  float temp1, temp2;
  degree = (dir * angle) + degree;

  if (degree < 0)
    degree = degree + 360;
  if (degree > 359)
    degree = degree - 360;
  cout << "current degree " << degree << endl;
  temp1 = cos((degree) * M_PI / 180);
  temp2 = sin((degree) * M_PI / 180);
  direction[0] = round(temp1);
  direction[1] = round(temp2);
  cout << "rotate_direction[0]" << direction[0] << endl;
  cout << "rotate_direction[1]" << direction[1] << endl;
}

void Interpret(const vector<Command>& commands) {
  // TODO: Interpret each command one-by-one.
  // TODO: Remove output statements as you implement them.
  Point2 last = Point2(0.0, 0.0);
  int dir;
  for (int i = 0; i < commands.size(); ++i) {
    const Command& c = commands[i];
    switch (c.name()) {
    case FORWARD:
      if (points.size() <= 0) {  // special case for first point
        points.push_back(Point2(0, c.arg()));
      } else {
        last = Point2(points[points.size() - 1].x,
                      points[points.size() - 1].y);
        points.push_back(Point2(last.x + (c.arg() * direction[0]),
                           last.y + (c.arg() * direction[1])));
      }
      // cout << "forward " << c.arg() << endl;
      break;
    case RIGHT:
      dir = -1;
      rotate(c.arg(), dir);
      // cout << "right " << c.arg() << endl;
      break;
    case LEFT:
      dir = 1;
      rotate(c.arg(), dir);
      // cout << "left " << c.arg() << endl;
      break;
    case PEN_UP:
      penState = 1;
      // cout << "pen up" << endl;
      break;
    case PEN_DOWN:
      penState = 0;
      // cout << "pen down" << endl;
      break;
    case COLOR:
      if (c.arg() == 1)
         color = one;
      if (c.arg() == 2)
        color = two;
      if (c.arg() == 3)
        color = three;
      // cout << "color " << c.arg() << endl;
      break;
    case ORIGIN:
      points.push_back(Point2(0, 0));
      // cout << "origin" << endl;
      break;
    }
  }
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(EXIT_SUCCESS);
      break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(200, 200);
  glutInitWindowPosition(0, 0);
  // DO NOT change the window title.  It is necessary for the screen capture.
  glutCreateWindow("Turtle graphics");
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  Init();

  if (argc < 2) {
    cout << "Usage: turtle filename" << endl;
    exit(1);
  }
  vector<Command> commands = Parse(argv[1]);
  Interpret(commands);

  glutMainLoop();
}
