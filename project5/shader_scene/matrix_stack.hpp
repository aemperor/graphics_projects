
#ifndef __matrix_stack_hpp__
#define __matrix_stack_hpp__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if !defined(__Cg_vector_hpp__)
#include <Cg/double.hpp>
#endif
#include <Cg/vector.hpp>
#include <Cg/matrix.hpp>

void loadGLMatrix(GLenum mode, const Cg::float4x4& matrix);
void multGLMatrix(GLenum mode, const Cg::float4x4& matrix);
void pushGLMatrix(GLenum mode);
void pushAndMultGLMatrix(GLenum mode, const Cg::float4x4& matrix);
void popGLMatrix(GLenum mode);

#endif // __matrix_stack_hpp__
