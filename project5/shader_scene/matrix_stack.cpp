
// matrix_stack.cpp - OpenGL matrix stack manipulation

#include "matrix_stack.hpp"

using namespace Cg;

extern bool has_EXT_direct_state_access;

void loadGLMatrix(GLenum mode, const float4x4& matrix)
{
    assert(sizeof(matrix) == 16*sizeof(GLfloat));
    if (has_EXT_direct_state_access) {
        glMatrixLoadTransposefEXT(mode, &matrix[0][0]);
    } else {
        glMatrixMode(mode);
        glLoadTransposeMatrixf(&matrix[0][0]);
    }
}

void multGLMatrix(GLenum mode, const float4x4& matrix)
{
    assert(sizeof(matrix) == 16*sizeof(GLfloat));
    if (has_EXT_direct_state_access) {
        glMatrixMultTransposefEXT(GL_PROJECTION, &matrix[0][0]);
    } else {
        glMatrixMode(GL_PROJECTION);
        glMultTransposeMatrixf(&matrix[0][0]);
    }
}

void pushGLMatrix(GLenum mode)
{
    if (has_EXT_direct_state_access) {
        glMatrixPushEXT(mode);
    } else {
        glMatrixMode(mode);
        glPushMatrix();
    }
}

void pushAndMultGLMatrix(GLenum mode, const float4x4& matrix)
{
    assert(sizeof(matrix) == 16*sizeof(GLfloat));
    if (has_EXT_direct_state_access) {
        glMatrixPushEXT(mode);
        glMatrixMultTransposefEXT(mode, &matrix[0][0]);
    } else {
        glMatrixMode(mode);
        glPushMatrix();
        glMultTransposeMatrixf(&matrix[0][0]);
    }
}

void popGLMatrix(GLenum mode)
{
    if (has_EXT_direct_state_access) {
        glMatrixPopEXT(mode);
    } else {
        glMatrixMode(mode);
        glPopMatrix();
    }
}
