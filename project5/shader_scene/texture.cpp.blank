
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "texture.hpp"
#include "matrix_stack.hpp"

#include <Cg/stdlib.hpp>
#include <Cg/iostream.hpp>

#include "../stb/stb_image.h"

extern const char *program_name;
extern bool verbose;

TextureImage::TextureImage()
    : filename(NULL)
    , width(0)
    , height(0)
    , components(0)
    , image(NULL)
{
}

TextureImage::TextureImage(const char *fn)
    : width(0)
    , height(0)
    , components(0)
    , image(NULL)
{
    filename = new char[strlen(fn)+1];
    assert(filename);
    strcpy(filename, fn);
}

TextureImage::~TextureImage()
{
    stbi_image_free(image);
    delete filename;
}

TextureGLState::TextureGLState()
    : mipmapped(true)
    , texture_object(0)
{
}

TextureGLState::~TextureGLState()
{
    if (texture_object) {
        glDeleteTextures(1, &texture_object);
    }
}

void TextureGLState::tellGL()
{
    if (texture_object == 0) {
        glGenTextures(1, &texture_object);
    }
}

GLuint TextureGLState::getTextureObject()
{
    return texture_object;
}

bool TextureGLState::isMipmapped()
{
    return mipmapped;
}

Texture::Texture(const char *fn)
    : TextureImage(fn)
{
}

Texture2D::Texture2D(const char *fn)
    : Texture(fn)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::bind()
{
    glBindTexture(target, getTextureObject());
}

void Texture2D::bind(GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(target, getTextureObject());
}

bool TextureImage::load()
{
    int requested_components = 4;
    image = stbi_load(filename, &width, &height, &components, requested_components);
    if (image) {
        // success
        return true;
    } else {
        printf("%s: failed to load image %s\n", program_name, filename);
        return false;
    }
}

void Texture2D::tellGL()
{
    TextureGLState::tellGL();

    bind();
    glTexImage2D(target, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (isMipmapped()) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

Texture::~Texture()
{
}

NormalMap::NormalMap(const char *fn)
    : Texture2D(fn)
    , normal_image(NULL)
{}

struct PackedNormal {
    GLubyte n[3];

    PackedNormal(float3 normal);
};

PackedNormal::PackedNormal(float3 normal)
{
    assert(normal.x >= -1);
    assert(normal.y >= -1);
    assert(normal.z >= -1);

    assert(normal.x <= 1);
    assert(normal.y <= 1);
    assert(normal.z <= 1);
    n[0] = 128 + 127*normal.x;
    n[1] = 128 + 127*normal.y;
    n[2] = 128 + 127*normal.z;
}

float3 NormalMap::computeNormal(int i, int j, float scale)
{
    float3 normal = float3(0,0,1);  // XXX fix me
    return normal;
}

bool NormalMap::load(float scale)
{
    int actual_components = 0;
    int requested_components = 1;
    components = requested_components;
    image = stbi_load(filename, &width, &height, &actual_components, requested_components);
    if (image) {
        assert(width > 0);
        assert(height > 0);
        assert(actual_components > 0);
        if (actual_components != requested_components) {
            if (verbose) {
                printf("warning: %d component normal map treated as gray scale height map\n",
                    actual_components);
            }
        }
        normal_image = new GLubyte[width*height*3];
        assert(normal_image);
        GLubyte* p = normal_image;
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                float3 normal = computeNormal(x,y, scale);
                PackedNormal packed_normal(normal);
                p[0] = packed_normal.n[0];
                p[1] = packed_normal.n[1];
                p[2] = packed_normal.n[2];
                p += 3;
            }
        }
        assert(p == normal_image+(width*height*3));
        // success
        return true;
    } else {
        printf("%s: failed to load image %s\n", program_name, filename);
        return false;
    }
}

void NormalMap::tellGL()
{
    TextureGLState::tellGL();

    bind();
    glTexImage2D(target, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, normal_image);
    if (isMipmapped()) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

NormalMap::~NormalMap()
{
    delete normal_image;
}

static const char *face_name[6] = {
    "xpos",
    "xneg",
    "ypos",
    "yneg",
    "zpos",
    "zneg"
};

#ifdef _WIN32  // grr, Microsoft refuses to use the C99 name
#define snprintf _snprintf
#endif

CubeMap::CubeMap(const char *filename_pattern)
{
    for (int i=0; i<6; i++) {
        char buffer[200];

        snprintf(buffer, sizeof(buffer), filename_pattern, face_name[i]);
        face[i].filename = new char[strlen(buffer)+1];
        strcpy(face[i].filename, buffer);
    }
}

CubeMap::~CubeMap()
{
}

bool CubeMap::load()
{
    bool success = true;
    for (int i=0; i<6; i++) {
        success &= face[i].load();
    }
    return success;
}

void CubeMap::tellGL()
{
    TextureGLState::tellGL();

    bind();
    
    GLint base_level = 0;
    for (int i=0; i<6; i++) {
        TextureImage &img = face[i];

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, base_level,
            GL_RGBA8, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.image);
    }
    if (isMipmapped()) {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void CubeMap::bind()
{
    glBindTexture(target, getTextureObject());
}

void CubeMap::bind(GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(target, getTextureObject());
}

static const GLfloat vertex[4*6][3] = {
    /* Positive X face. */
    { 1, -1, -1 },  { 1, 1, -1 },  { 1, 1, 1 },  { 1, -1, 1 },
    /* Negative X face. */
    { -1, -1, -1 },  { -1, 1, -1 },  { -1, 1, 1 },  { -1, -1, 1 },
    /* Positive Y face. */
    { -1, 1, -1 },  { 1, 1, -1 },  { 1, 1, 1 },  { -1, 1, 1 },
    /* Negative Y face. */
    { -1, -1, -1 },  { 1, -1, -1 },  { 1, -1, 1 },  { -1, -1, 1 },
    /* Positive Z face. */
    { -1, -1, 1 },  { 1, -1, 1 },  { 1, 1, 1 },  { -1, 1, 1 },
    /* Negatieve Z face. */
    { -1, -1, -1 },  { 1, -1, -1 },  { 1, 1, -1 },  { -1, 1, -1 },
};

void CubeMap::draw(float scale)
{
    GLenum texture_unit = GL_TEXTURE3;
    glUseProgram(0);

    bind(texture_unit);
    glActiveTexture(texture_unit);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    pushGLMatrix(GL_MODELVIEW); {
        glScalef(scale, scale, scale);
        glBegin(GL_QUADS); {
            /* For each vertex of each face of the cube... */
            for (int i=0; i<4*6; i++) {
                glMultiTexCoord3fv(texture_unit, vertex[i]);
                glVertex3f(vertex[i][0], vertex[i][1], vertex[i][2]);
            }
        } glEnd();
    } popGLMatrix(GL_MODELVIEW);
    glDisable(GL_TEXTURE_CUBE_MAP);
    glActiveTexture(GL_TEXTURE0);
}
