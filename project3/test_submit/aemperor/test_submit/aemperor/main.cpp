/*
  CS 354 - Fall 2012
  main.cpp

  This is simple skeleton GLUT code.
*/

// C++ library includes
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <vector>

#include "./common.h"

#include "./types.h"
#include "./bvh_defs.h"
#include "./joint.h"
#include "./loader.h"
#include "./geom.h"

using namespace std;

// window parameters
int window_width = 800, window_height = 600;
float window_aspect = window_width/static_cast<float>(window_height);

// callback prototypes
void InitGL();
void Display();
void Resize(int width, int height);
void Keyboard(unsigned char key, int x, int y);
void Idle();

SceneGraph sg;
bool play = false;
int frameNumber = 0;
int curTime = 0;
int prevTime = 0;
int interval;
int rate = 20;

#define PI 3.14159265f

Vec3f eye, center, up;
int waypoint = 1;

BoundingBox bbox = {{-100, -100, -100}, {100, 100, 100}};

char filename[1000];

bool showAxis = true;
float axisLen = 1.0f;

bool showBounds = false;

void SetLighting();

void InitGL() {
  // Perform any necessary GL initialization in this function

  // enable depth testing, and set clear color to white
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  SetLighting();

  // resize the window
  Resize(window_width, window_height);
}

void ComputeLookAt() {
  float maxDist = (bbox.max-bbox.min).max();
  center = (bbox.max+bbox.min)/2.0f;
  up = Vec3f::makeVec(0.0f, 1.0f, 0.0f);
  eye = center+Vec3f::makeVec(0.0f, 0.75f*maxDist, -1.5f*maxDist);
  if (waypoint == 1) {
    eye = center+Vec3f::makeVec(0.5f*maxDist, 0.75f*maxDist, 1.5f*maxDist);
  } else if (waypoint == 2) {
    eye = center+Vec3f::makeVec(0, 0.1f*maxDist, 1.5f*maxDist);
  } else if (waypoint == 3) {
    eye = center+Vec3f::makeVec(1.5f*maxDist, 0.1f*maxDist, 0);
  }
  axisLen = maxDist*0.05f;
}

void SetLighting() {
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_COLOR_MATERIAL);
}

void SetCamera() {
  gluLookAt(eye[0], eye[1], eye[2],
            center[0], center[1], center[2],
            up[0], up[1], up[2]);
}

void SetProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(40.0, window_aspect, 1, 1500);
}

void SetDrawMode() {
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);
}

void DrawAxis() {
  Vec3f c = (bbox.min+bbox.max)*0.5f;
  float L = (bbox.max-bbox.min).max() * 0.2;

  Vec3f X = {L, 0, 0}, Y = {0, L, 0}, Z = {0, 0, L};

  glLineWidth(2.0);

  glBegin(GL_LINES);

  glColor3f(1, 0, 0);
  glVertex3fv(c.x);
  glVertex3fv((c+X).x);

  glColor3f(0, 1, 0);
  glVertex3fv(c.x);
  glVertex3fv((c+Y).x);

  glColor3f(0, 0, 1);
  glVertex3fv(c.x);
  glVertex3fv((c+Z).x);

  glEnd();
}

void DrawFloor(float W, float H, float w, float h) {
  float a = H/h, b = W/w;
  int M = static_cast<int>(floor(a+0.5f));
  int N = static_cast<int>(floor(b+0.5f));
  int i = 0, j = 0;
  Vec3f u = {w, 0, 0}, v = {0, 0, h}, r = {-(N/2)*w, 0, -(M/2)*h};
  Vec3f p0, p1, p2, p3;
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glBegin(GL_QUADS);
  glColor3f(0.7, 0.7, 0.7);
  for (j = 0; j < N; j++) {
    p0 = r+u*static_cast<float>(j%2);
    for (i = j%2; i < M; i += 2) {
      p1 = p0+u;
      p2 = p1+v;
      p3 = p2-u;

      glVertex3fv(p3.x);
      glVertex3fv(p2.x);
      glVertex3fv(p1.x);
      glVertex3fv(p0.x);

      p0 += u*2.0f;
    }
    r += v;
  }
  glEnd();
}

void DrawRect(const Vec3f & u, const Vec3f & v, const Vec3f & o) {
  glBegin(GL_LINE_STRIP);
  glColor3f(0, 0, 1);
  glVertex3fv(o.x);
  glVertex3fv((o+u).x);
  glVertex3fv((o+u+v).x);
  glVertex3fv((o+v).x);
  glVertex3fv(o.x);
  glEnd();
}

void DrawBounds() {
  Vec3f u, v, m1[] = {bbox.min, bbox.max}, m2[] = {bbox.max, bbox.min};

  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 3; i++) {
      for (int j = i+1; j < 3; j++) {
        u = m1[k];
        v = m1[k];
        u.x[i] = m2[k].x[i];
        v.x[j] = m2[k].x[j];
        u = u-m1[k];
        v = v-m1[k];
        DrawRect(u, v, m1[k]);
      }
    }
  }
}

void RotateLeft() {
  float temp1 = eye[0];
  eye[0] = eye[0]*cos(1*PI/180) - eye[2]*sin(1*PI/180);
  eye[1] = eye[1];
  eye[2] = temp1*sin(1*PI/180) + eye[2]*cos(1*PI/180);
}

void RotateRight() {
  float temp1 = eye[0];
  eye[0] = eye[0]*cos(-1*PI/180) - eye[2]*sin(-1*PI/180);
  eye[1] = eye[1];
  eye[2] = temp1*sin(-1*PI/180) + eye[2]*cos(-1*PI/180);
}

void ZoomIn() {
  eye[0] = eye[0]*0.95;
  eye[1] = eye[1]*0.95;
  eye[2] = eye[2]*0.95;
}

void ZoomOut() {
  eye[0] = eye[0]*1.05;
  eye[1] = eye[1]*1.05;
  eye[2] = eye[2]*1.05;
}

void DrawTree(int i) {
  glPushMatrix();
  glTranslatef(sg.listOfJoints[i].xoff,
               sg.listOfJoints[i].yoff,
               sg.listOfJoints[i].zoff);
  if (sg.listOfJoints[i].channel > 0) {
    glRotatef(sg.listOfJoints[i].posRot[0], 0, 0, 1);
    glRotatef(sg.listOfJoints[i].posRot[1], 0, 1, 0);
    glRotatef(sg.listOfJoints[i].posRot[2], 1, 0, 0);
    sg.listOfJoints[i].posRot.clear();
  }
  glColor3f(0.5f, 0.0f, 1.0f);
  glutSolidSphere(0.5, 10, 10);
  for (int j = 0; j < sg.listOfJoints[i].childIds.size(); ++j) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(sg.listOfJoints[sg.listOfJoints[i].childIds[j]].xoff,
               sg.listOfJoints[sg.listOfJoints[i].childIds[j]].yoff,
               sg.listOfJoints[sg.listOfJoints[i].childIds[j]].zoff);
    glEnd();
    DrawTree(sg.listOfJoints[i].childIds[j]);
  }
  glPopMatrix();
}

void DrawJoint(float x, float y, float z) {
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(sg.listOfJoints[0].posRot[3], 0, 0, 1);
  glRotatef(sg.listOfJoints[0].posRot[4], 0, 1, 0);
  glRotatef(sg.listOfJoints[0].posRot[5], 1, 0, 0);
  sg.listOfJoints[0].posRot.clear();
  glutSolidSphere(0.5, 10, 10);
  for (int i = 0; i < sg.listOfJoints[0].childIds.size(); i++) {
    DrawTree(sg.listOfJoints[0].childIds[i]);
  }
  glPopMatrix();
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SetProjection();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  SetLighting();
  SetCamera();
  SetDrawMode();
  DrawFloor(800, 800, 80, 80);
  glColor3f(0.5f, 0.0f, 1.0f);
  // TODO: draw scene graph and animate
  if (frameNumber >= sg.frames.size())  // reset to loop
    frameNumber = 0;
  for (int i = 0; i < sg.listOfJoints.size(); i++) {
    int DataIndex = sg.listOfJoints[i].frameIdx;
    for (int j = 0; j < sg.listOfJoints[i].channel; j++) {
      sg.listOfJoints[i].posRot.push_back(
             sg.frames[frameNumber].frameData[DataIndex]);
      DataIndex++;
    }
  }
  // controls speed up/slow down
  while (interval < rate) {
    curTime = glutGet(GLUT_ELAPSED_TIME);
    interval = curTime - prevTime;
  }

  // Draw root to draw the rest
  DrawJoint(sg.listOfJoints[0].posRot[0],
            sg.listOfJoints[0].posRot[1],
            sg.listOfJoints[0].posRot[2]);
  if (play) {
    interval = 0;
    prevTime = curTime;
    curTime = 0;
    frameNumber++;
  }
  if (showAxis) DrawAxis();
  if (showBounds) DrawBounds();
  glFlush();          // finish the drawing commands
  glutSwapBuffers();  // and update the screen
  glutPostRedisplay();
}

// This reshape function is called whenever the user
// resizes the display window.
void Resize(int width, int height) {
  window_width = width;
  window_height = height;
  float fheight = static_cast<float>(height);
  window_aspect = width / fheight;

  // resize the window
  glViewport(0, 0, window_width, window_height);

  SetProjection();

  // setup basic orthographic projection
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // let glut know to redraw the screen
  glutPostRedisplay();
}

void SpeedRate(float speed) {
  sleep(speed * sg.frameTime);
}

// This function is called whenever the user hits letters or numbers
// on the keyboard.  The 'key' variable has the character the user hit,
// and x and y tell where the mouse was when it was hit.
void Keyboard(unsigned char key, int x, int y) {
  y = window_height - y;

  float sgn = 1.0f;
  Vec3f v;

  switch (key) {
    case 'e':
      eye[2] = eye[2] - 1;
      center[2] = center[2] - 1;
      break;
    case 'd':
      eye[2] = eye[2] + 1;
      center[2] = center[2] + 1;
      break;
    case 's':
      eye[0] = eye[0] - 1;
      center[0] = center[0] - 1;
      break;
    case 'f':
      eye[0] = eye[0] + 1;
      center[0] = center[0] + 1;
      break;
    case '1':
      waypoint = 1;
      ComputeLookAt();
      break;
    case '2':
      waypoint = 2;
      ComputeLookAt();
      break;
    case '3':
      waypoint = 3;
      ComputeLookAt();
      break;
    case 'z':
      // cout << "Zoom in" << endl;
        ZoomIn();
      break;
    case 'Z':
      // cout << "Zoom out" << endl;
      ZoomOut();
      break;
    case 'j':
      // cout << "Orbit left" << endl;
      RotateLeft();
      break;
    case 'k':
      // cout << "Orbit right" << endl;
      RotateRight();
      break;
    case ' ':
      if (play) {
        play = false;
      } else {
        play = true;
      }
      // cout << "Start/stop animation" << endl;
      break;
    case 'c':
      rate = (rate < 100) ? rate*=5 : rate;
      cout << "slowing down -- rate = " << rate << endl;
      break;
    case 'x':
      rate = (rate > 1) ? rate/=5 : rate;
      if (rate == 0)
        rate = 1;
      cout << "speeding up -- rate = " << rate << endl;
      break;
    case 'g':
      rate = 20;
      cout << "returning to original speed -- rate = " << rate << endl;
      break;
    case 'a':
      showAxis=!showAxis;
      break;
    case 'b':
      showBounds=!showBounds;
      break;
    case 'q':
    case 27:  // esc
      exit(0);
      break;
  }
  glutPostRedisplay();
}

void Idle() {
}

void processCommandLine(int argc, char *argv[]) {
  if (argc>1) {
    snprintf(&(filename[0]), strlen(argv[1])+1, "%s", argv[1]);
    BVHLoader::loadBVH(filename, &sg);
    ComputeLookAt();
  } else {
    printf("Filename argument required.\n");
    exit(0);
  }
}

void showMenu() {
  cout << "------------ menu ------------" << endl;
  cout << "q - quit" << endl;
  cout << "a - show/hide axis" << endl;
  cout << "b - show/hide bounds" << endl;
  cout << "[1-3] - move to waypoint" << endl;
  cout << "z - zoom in" << endl;
  cout << "Z - zoom out" << endl;
  cout << "j - rotate left" << endl;
  cout << "k - rotate right" << endl;
  cout << "e - move backward" << endl;
  cout << "d - move forward" << endl;
  cout << "f - move right" << endl;
  cout << "s - move left" << endl;
  cout << "x - speed up" << endl;
  cout << "c - slow down" << endl;
  cout << "g - original speed" << endl;
  cout << "[SPACE] - start/stop" << endl;
}

int main(int argc, char *argv[]) {
  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 100);
  // Do not change the name of the window -- it is needed for the screenshot
  glutCreateWindow("Mocap");
  glutDisplayFunc(Display);
  glutReshapeFunc(Resize);
  glutKeyboardFunc(Keyboard);
  glutIdleFunc(Idle);

  processCommandLine(argc, argv);

  showMenu();

  InitGL();

  glutMainLoop();

  return 0;
}
