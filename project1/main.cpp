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

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  // TODO: Change to draw according to turtle commands given
  // Draw a red triangle.
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex2f(-0.5, -0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(0.5, -0.5);
  glVertex2f(0.0, 0.5);
  glVertex2f(0.0, 0.5);
  glVertex2f(-0.5, -0.5);
  glEnd();

  glFlush();
}

void Interpret(const vector<Command>& commands) {
  // TODO: Interpret each command one-by-one.
  // TODO: Remove output statements as you implement them.
  for (int i = 0; i < commands.size(); ++i) {
    const Command& c = commands[i];
    switch (c.name()) {
    case FORWARD:
      cout << "forward " << c.arg() << endl;
      break;
    case RIGHT:
      cout << "right " << c.arg() << endl;
      break;
    case LEFT:
      cout << "left " << c.arg() << endl;
      break;
    case PEN_UP:
      cout << "pen up" << endl;
      break;
    case PEN_DOWN:
      cout << "pen down" << endl;
      break;
    case COLOR:
      cout << "color " << c.arg() << endl;
      break;
    case ORIGIN:
      cout << "origin" << endl;
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
  glutInitWindowSize(500, 500);
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
