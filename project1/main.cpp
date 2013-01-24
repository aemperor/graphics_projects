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
  // glutInitWindowSize(200, 200);
  // glutInitWindowPosition(0, 0);
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
const Color white = Color(1, 1, 1);  // white

// Color recentColor = zero;
Color color = zero;
int direction = 0;  // right = 0; left = 1
int penState = 0;  // down = 0; up = 1
float sine = 0.0;
float cosine = 0.0;

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO: Change to draw according to turtle commands given
  // Draw a red triangle.
  if (penState == 0) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < points.size(); ++i) {
      glColor3fv(color);
      glVertex2i(points[i].x, points[i].y);
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

void Interpret(const vector<Command>& commands) {
  // TODO: Interpret each command one-by-one.
  // TODO: Remove output statements as you implement them.
  Point2 last = Point2(0.0, 0.0);
  for (int i = 0; i < commands.size(); ++i) {
    const Command& c = commands[i];
    // cout << "commands[i]" << c.arg() << endl;
    switch (c.name()) {
    case FORWARD:
      // cout << "before last " << endl;
      if (points.size() > 0) {
        last = Point2(points[points.size() - 1].x,
                      points[points.size() - 1].y);
        // cout << "last " << endl;
        if (direction == 0)
          points.push_back(Point2(last.x + (sine * c.arg()),
                           last.y + (cosine * c.arg())));
        if (direction == 1)
          points.push_back(Point2(last.x + (cosine * c.arg()),
                           last.y + (sine * c.arg())));
        direction = -1;
      }
      // cout << "forward " << c.arg() << endl;
      break;
    case RIGHT:
      direction = 0;
      cosine = cos(c.arg());
      sine = sin(c.arg());
      // cout << "right " << c.arg() << endl;
      break;
    case LEFT:
      direction = 1;
      cosine = cos(c.arg());
      sine = sin(c.arg());
      cout << "left " << c.arg() << endl;
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
