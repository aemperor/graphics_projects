
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <Cg/double.hpp>
#include <Cg/vector.hpp>
#include <Cg/mul.hpp>
#include <Cg/normalize.hpp>
#include <Cg/length.hpp>
#include <Cg/degrees.hpp>
#include <Cg/radians.hpp>
#include <Cg/stdlib.hpp>

#include "glmatrix.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "countof.h"
#include "trackball.h"
#include "menus.hpp"
#include "global.hpp"
#include "request_vsync.h"

using namespace Cg;

#ifndef GLAPIENTRY
# ifdef _WIN32
#  define GLAPIENTRY __stdcall
# else
#  define GLAPIENTRY
# endif
#endif

const char *program_name = "shader_scene";

bool has_EXT_direct_state_access;

// Declared in global.hpp
ScenePtr scene;
MaterialPtr material;
LightPtr light;
float bump_height = 2.0;

float3 eye_vector = float3(0,0,5);
float3 at_vector = float3(0,0,0);
float3 up_vector = float3(0,1,0);

bool verbose = false;

bool use_vsync = true;

bool moving_eye = false;
int begin_x;
int begin_y;

bool moving_light = false;
int begin_light_x;
int begin_light_y;

bool spin_object = false;
int begin_spin_x;
int begin_spin_y;
bool new_spin_update = true;
bool animate_object_spinning = false;
float curquat[4];
/* Initial slight rotation */
float lastquat[4] = { 1.78721e-006, -0.00139029, 3.47222e-005, 0.999999 };

float window_widthf, window_heightf;

bool wireframe = false;

void initglext()
{
    glewInit();  // Initialize OpenGL Extension Wrangler.
    GLboolean has21 = glewIsSupported("GL_VERSION_2_1");
    if (!has21) {
        printf("%s: requires an OpenGL 2.1 implementation or better, exiting...\n", program_name);
        exit(1);
    }
    has_EXT_direct_state_access = !!glutExtensionSupported("GL_EXT_direct_state_access");
}

void initGraphics()
{
    trackball(curquat, 0.0, 0.0, 0.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);  // avoid GL's dumb default of 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // avoid GL's dumb default of 4
    glClearColor(0.1, 0.1, 0.2, 0.0);

    scene = ScenePtr(new Scene(Camera(40, 1, 0.1, 100),
                               View(eye_vector,
                                    at_vector,
                                    up_vector))); 

    material = MaterialPtr(new Material());
    // A little hacky - pick initial values from menu items
    materialMenu(0);
    bumpyMenu(0);
    decalMenu(0);
    envMapMenu(0);

    Mesh2DPtr mesh2d = Mesh2DPtr(new Mesh2D(float2(0,0), float2(1,1), int2(4,4)));

    TorusPtr torus = TorusPtr(new Torus(Transform(), material));
    scene->addObject(torus);

    light = LightPtr(new Light());
    light->setCenter(float3(0,0,0));
    light->setRadius(3.5);
    light->setAngleInDegrees(60.0);
    // A little hacky - pick initial value from menu item
    lightMenu(1);
    scene->addLight(light);
}

void printMatrices()
{
    GLfloat projection_matrix[16], modelview_matrix[16];

    glGetFloatv(GL_TRANSPOSE_PROJECTION_MATRIX, projection_matrix);
    glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, modelview_matrix);
    printf("Projection matrix:\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            printf(" %f", projection_matrix[4*i+j]);
        }
        printf("\n");
    }
    printf("Modelview matrix:\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            printf(" %f", modelview_matrix[4*i+j]);
        }
        printf("\n");
    }
}

void doGraphics()
{
    if (new_spin_update) {
        GLfloat m[4][4];

        build_rotmatrix(m, curquat);
        scene->object_list[0]->transform.setMatrix(m);
    }
    scene->draw();
#if 0
    printMatrices();
#endif
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    doGraphics();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0,0,w,h);
    float aspect_ratio = float(w)/float(h);
    scene->camera.setAspectRatio(aspect_ratio);
    window_widthf = w;
    window_heightf = h;
}

void toggleWireframe()
{
    wireframe = !wireframe;  // toggle
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void keyboard(unsigned char c, int x, int y)
{
    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        shaderMenu(c-'0');
        break;
    case 27:  /* Esc quits */
        exit(0);
        return;
    case 'q':  /* q quits */
        exit(0);
        return;
    case 13:  /* Enter redisplays */
        break;
    case 'w':
        toggleWireframe();
        break;
    case 'V':
        verbose = !verbose;  // toggle
        break;
    case 'v':
       use_vsync = !use_vsync;
       printf("use vsync = %s\n", use_vsync ? "enabled" : "disabled");
       requestSynchornizedSwapBuffers(use_vsync);
       break;
    case 'f':
        break;
    case 'B':
        bump_height -= 0.2;
        // Fallthrough...
    case 'b':
        bump_height += 0.1;
        printf("bump_height = %f\n", bump_height);

        material->normal_map->load(bump_height);
        material->normal_map->tellGL();

        material->bindTextures();
        break;
    default:
        return;
    }
    glutPostRedisplay();
}

void stopObjectSpinning()
{
    animate_object_spinning = false;
    glutIdleFunc(NULL);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        int modifiers = glutGetModifiers();

        if (modifiers & GLUT_ACTIVE_CTRL) {
            if (state == GLUT_DOWN) {
                stopObjectSpinning();
                spin_object = true;
                begin_spin_x = x;
                begin_spin_y = y;
            } else {
                assert(state == GLUT_UP);
                spin_object = false;
            }
        } else {
            if (state == GLUT_DOWN) {
                moving_eye = true;
                begin_x = x;
                begin_y = y;
            } else {
                assert(state == GLUT_UP);
                moving_eye = false;
            }
        }
    }
    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            moving_light = true;
            begin_light_x = x;
            begin_light_y = y;
        } else {
            assert(state == GLUT_UP);
            moving_light = false;
        }
    }
}

void spin_animate()
{
    add_quats(lastquat, curquat, curquat);
    new_spin_update = true;
    glutPostRedisplay();
}

void motion(int x, int y)
{
    if (moving_eye) {
        scene->view.spinDegrees(-0.2*(x-begin_x));
        scene->view.lift(0.02*(y-begin_y));

        glutPostRedisplay();
        begin_x = x;
        begin_y = y;
    }
    if (moving_light) {
        LightPtr light = scene->light_list[0];

        light->spinDegrees(0.2*(x-begin_light_x));
        light->lift(-0.02*(y-begin_light_y));

        glutPostRedisplay();
        begin_light_x = x;
        begin_light_y = y;
    }
    if (spin_object) {
        trackball(lastquat,
            (2.0 * begin_spin_x - window_widthf) / window_widthf,
            (window_heightf - 2.0 * begin_spin_y) / window_heightf,
            (2.0 * x - window_widthf) / window_widthf,
            (window_heightf - 2.0 * y) / window_heightf
            );
        begin_spin_x = x;
        begin_spin_y = y;
        animate_object_spinning = true;
        glutIdleFunc(spin_animate);
    }
}

int main(int argc, char **argv)
{
#if defined(_WIN32) && !defined(NDEBUG)  // Set to 1 for Windows heap debugging
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);

    for (int i=1; i<argc; i++) {
       if (!strcmp(argv[i], "-novsync")) {
           use_vsync = false;
       }
    }

    glutCreateWindow(program_name);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    initglext();
    initGraphics();
    initMenus();
    requestSynchornizedSwapBuffers(use_vsync);

    glutMainLoop();
    return 0;
}

