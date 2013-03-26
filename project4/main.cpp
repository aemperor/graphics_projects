#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "./common.h"
#include "./bb.h"
#include "./mesh.h"
#include "./io.h"
#include "./texture.h"

#define ARCBALL_RADIUS 1

using namespace std;

Mesh mesh;

GLuint* texture_ids;

// window parameters
int window_width = 800, window_height = 600;
float window_aspect = window_width / static_cast<float>(window_height);

float angle = 0.0;
Vec3f rotation_axis, start_vector, end_vector;
GLfloat previous_rotation[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 0.0f};

GLfloat current_rotation[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 0.0f};

bool scene_lighting;
bool isClicked = false;

struct Point2 {
  GLfloat x, y;
  Point2() {
    x = 0;
    y = 0;
  }
  Point2(GLfloat _x, GLfloat _y) {
    x = _x;
    y = _y;
  }
};

void PrintMatrix();
void PrintMatrix(GLfloat* m);

Point2 mouse_pnt, mouse_curr;

void DrawVertices() {
  glLineWidth(1);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < mesh.vertices.size(); ++i) {
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(mesh.vertices[i][0],
               mesh.vertices[i][1],
               mesh.vertices[i][2]);
  }
  glEnd();
}

void DrawPolygons() {
  for (int i = 0; i < mesh.polyVerts.size(); ++i) {
    glBegin(GL_POLYGON);
    for (int j = 0; j < mesh.polyVerts[i].size(); ++j) {
      glVertex3f(mesh.vertices[mesh.polyVerts[i][j]][0],
                 mesh.vertices[mesh.polyVerts[i][j]][1],
                 mesh.vertices[mesh.polyVerts[i][j]][2]);
    }
    glEnd();
  }
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, window_aspect, 1, 1500);

  // TODO call gluLookAt such that mesh fits nicely in viewport.
  // mesh.bb() may be useful.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  cout << "matrix" << endl;
  PrintMatrix(previous_rotation);
  // glLoadMatrixf(previous_rotation);
  cout << "angle" << angle << endl;
  // glRotatef(angle, rotation_axis.x[0],
  // rotation_axis.x[1], rotation_axis.x[2]);

  gluLookAt(2, 2, 5,
            0, 0, 0,
            0, 1, 0);


  // TODO set up lighting, material properties and render mesh.
  // Be sure to call glEnable(GL_RESCALE_NORMAL) so your normals
  // remain normalized throughout transformations.

  // You can leave the axis in if you like.
  glDisable(GL_LIGHTING);
  glLineWidth(4);
  glRotatef(angle, rotation_axis.x[0], rotation_axis.x[1], rotation_axis.x[2]);
  DrawAxis();
  // DrawVertices();
  DrawPolygons();
  glEnable(GL_LIGHTING);

  glFlush();
  glutSwapBuffers();
}

void PrintMatrix(GLfloat* m) {
  cout.precision(2);
  int w = 6;
  for (int i = 0; i < 4; ++i) {
    cout << setprecision(2) << setw(w) << m[i] << " "
         << setprecision(2) << setw(w) << m[i+4] << " "
         << setprecision(2) << setw(w) << m[i+8] << " "
         << setprecision(2) << setw(w) << m[i+12] << " "
         << endl;
  }
  cout << endl;
}

void PrintMatrix(GLint matrix) {
  GLfloat m[16];
  glGetFloatv(matrix, m);
  PrintMatrix(m);
}

void PrintMatrix() {
  PrintMatrix(GL_MODELVIEW_MATRIX);
}

void LoadMatrix(GLfloat* m) {
  // transpose to column-major
  for (int i = 0; i < 4; ++i) {
    for (int j = i; j < 4; ++j) {
      swap(m[i*4+j], m[j*4+i]);
    }
  }
  glLoadMatrixf(m);
}

void MultMatrix(GLfloat* m) {
  // transpose to column-major
  for (int i = 0; i < 4; ++i) {
    for (int j = i; j < 4; ++j) {
      swap(m[i*4+j], m[j*4+i]);
    }
  }
  glMultMatrixf(m);
}

void Init() {
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // resize the window
  window_aspect = window_width/static_cast<float>(window_height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(40.0, window_aspect, 1, 1500);
}

void DrawAxis() {
  const Vec3f c = {0, 0, 0};
  const float L = 1;
  const Vec3f X = {L, 0, 0}, Y = {0, L, 0}, Z = {0, 0, L};

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

Vec3f make_arcball_vector(int x, int y) {
    Vec3f vector = Vec<float, 3u>::makeVec((2.0 * x / window_width) - 1.0,
                                           (2.0 * y / window_height) - 1.0,
                                            0.0);
    float length = sqrt(vector.x[0] * vector.x[0] +
                        vector.x[1] * vector.x[1]);
    if (length <= ARCBALL_RADIUS*ARCBALL_RADIUS) {
      vector.x[2] = sqrt(1 - length);
    } else {  // nearest point
        vector = vector.unit();
    }

    return vector;
}

void MouseButton(int button, int state, int x, int y) {
  // TODO implement arc ball and zoom
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      isClicked = true;
      mouse_pnt = Point2(x, window_height - y);
      memcpy(previous_rotation, current_rotation, sizeof(current_rotation));
    } else {
      isClicked = false;
    }
  }
  glutPostRedisplay();
}

void MouseMotion(int x, int y) {
  // TODO implement arc ball and zoom
  if (isClicked) {
    glPushMatrix();
    MultMatrix(previous_rotation);
    glGetFloatv(GL_MODELVIEW_MATRIX, current_rotation);
    glPopMatrix();
    mouse_curr.x = x;
    mouse_curr.y = window_height - y;

    start_vector = make_arcball_vector(mouse_pnt.x, mouse_pnt.y);
    end_vector   = make_arcball_vector(mouse_curr.x, mouse_curr.y);

    angle = acos(min(1.0f, start_vector * end_vector)) * 180 / M_PI;
    rotation_axis = start_vector ^ end_vector;

    start_vector = end_vector;

    /*
    cout << "start: " << start_vector << "\n";
    cout << "end: "   << end_vector << "\n";
    cout << "dot: "   << start_vector * end_vector << "\n";
    cout << "angle: " << angle << " degrees\n" << endl;
    */
  }

  glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
    case 27:  // esc
      exit(0);
      break;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << endl;
    cout << "Usage: ./viewer (filename.obj | -s) [-l]" << endl;
    cout << endl;
    cout << "To load data/test.obj: ./viewer data/test.obj" << endl;
    cout << "To load a custom scene: ./viewer -s" << endl;
    cout << endl;
    return 0;
  }

  // Initialize GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Object viewer");
  glutMouseFunc(MouseButton);
  glutMotionFunc(MouseMotion);
  glutKeyboardFunc(Keyboard);
  glutDisplayFunc(Display);

  Init();

  if (string(argv[1]) == "-s") {
    cout << "Create scene" << endl;
  } else {
    string filename(argv[1]);
    cout << filename << endl;

    // Detect whether to fix the light source(s) to the camera or the world
    scene_lighting = false;
    if (argc > 2 && string(argv[2]) == "-l") {
      scene_lighting = true;
    }

    // Parse the obj file, compute the normals, read the textures

    ParseObj(filename, mesh);
    mesh.compute_normals();

    texture_ids = new GLuint[mesh.num_materials()];
    glGenTextures(mesh.num_materials(), texture_ids);

    for (int i = 0; i < mesh.num_materials(); ++i) {
      Material& material = mesh.material(i);
      material.LoadTexture(texture_ids[i]);
    }
  }

  glutMainLoop();

  return 0;
}
