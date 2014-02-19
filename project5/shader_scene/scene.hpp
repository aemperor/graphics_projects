
#ifndef __scene_hpp__
#define __scene_hpp__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <Cg/double.hpp>
#include <Cg/vector/xyzw.hpp>
#include <Cg/vector.hpp>

#include "glmatrix.hpp"

#include "texture.hpp"

using namespace Cg;

class Transform {
    float4x4 matrix;

    mutable bool dirty;
    mutable float4x4 inverse_matrix;

    void validate() const;

public:
    Transform();
    Transform(const float4x4& matrix);

    void setMatrix(float4x4 m);
    void setMatrix(const GLfloat m[4][4]);
    void multMatrix(float4x4 m);

    float4x4 getMatrix() const;
    float4x4 getInverseMatrix() const;
};
typedef shared_ptr<Transform> TransformPtr;

struct Camera {
    float fov_degrees;
    float aspect_ratio;
    float znear;
    float zfar;

    // Derived state
    float4x4 projection_matrix;

    Camera(float fov, float ar, float zn, float zf);
    Camera& operator =(const Camera& rhs);
    Camera(const Camera& copy);

    void setAspectRatio(float ar);

    void tellGL();

    void validate();
};

struct Geometry {
    virtual void draw() {}
};
typedef shared_ptr<Geometry> GeometryPtr;

class Mesh2D : Geometry {
    float2 xy_min, xy_max;
    int2 steps;
    GLuint vbo; // OpenGL Vertex Buffer Object (VBO)
    bool valid;

    // glDrawElements indices
    int num_ndxs;
    GLuint *ndx;

    void validate();

public:
    Mesh2D(float2 xy_min, float2 xy_max, int2 steps);
    ~Mesh2D();

    void draw();
};
typedef shared_ptr<Mesh2D> Mesh2DPtr;

struct GLSLShader {
    GLenum shader_type;
    GLint bytes;
    GLchar* source;
    GLuint shader_object;
    bool dirty;

    GLSLShader(GLenum type, int len, const char* s);
    GLSLShader(GLenum type);
    ~GLSLShader();

    bool readTextFile(const char *filename);
    void validate();
    GLuint getShader();
    void showInfoLog(const char* msg);
    void reset();
    void release();
};
typedef shared_ptr<GLSLShader> GLSLShaderPtr;

struct FragmentShader : GLSLShader {
    FragmentShader(int len, const char *s);
    FragmentShader();
};

struct VertexShader : GLSLShader {
    VertexShader(int len, const char *s);
    VertexShader();
};

struct GLSLProgram {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program_object;
    bool dirty;

    GLSLProgram(GLuint vertex_shader, GLuint fragment_shader);
    GLSLProgram();
    void showInfoLog(const char* msg);
    bool validate();
    void use();
    void reset();
    void swap(GLSLProgram &other);
    void setVec1f(const char *name, float v);
    void setVec2f(const char *name, float2 v);
    void setVec3f(const char *name, float3 v);
    void setVec4f(const char *name, float4 v);
    void setMat3f(const char *name, const Transform &transform);
    void setSampler(const char *name, int texture_unit);
    GLint getLocation(const char *name);
    ~GLSLProgram();
};

struct SurfaceShader {
    virtual void bind() = 0;
};
typedef shared_ptr<SurfaceShader> SurfaceShaderPtr;

struct DiffuseShader : SurfaceShader {
    GLuint shader_object;

    float3 diffuse_color;
};

struct View {
    float3 initial_at_position;
    float3 initial_eye_position;
    float3 up_vector;

    float3 at_position;
    float3 eye_position;

    // Derived state
    float1 eye_height;
    float1 eye_angle;

    float eye_radius;
    bool dirty;  // is view_matrix up-to-date?
    float4x4 view_matrix;

    View(float3 eye, float3 at, float3 up);

    void spinDegrees(float angle);
    void lift(float height);

    void validate();

    void tellGL();
    void reset();

    float4x4 getViewMatrix();

    View& operator =(const View& rhs);
};
typedef shared_ptr<View> ViewPtr;

class Light {
private:
    float3 color;
    float3 center;
    float1 radius;

    float1 angle;
    float1 height;

    bool dirty;
    float3 position;

    void validate();

public:
    Light();

    void setColor(const float3 &color);
    void setCenter(float3 center);
    void setRadius(float radius);
    void setAngleInDegrees(float degrees);
    void setAngleInRadians(float radians);

    void spinDegrees(float angle);
    void lift(float height);

    float4 getPosition();
    float4 getColor();

    Light& operator =(const Light& rhs);

    void draw(const View& view);
};
typedef shared_ptr<Light> LightPtr;

struct Material {
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;

    NormalMapPtr normal_map;
    Texture2DPtr decal;
    Texture2DPtr height_field;
    CubeMapPtr envmap;

    Material();

    void bindTextures();

    Material& operator =(const Material& rhs);
};
typedef shared_ptr<Material> MaterialPtr;

struct Object {
    string vertex_filename;
    string fragment_filename;
    GLSLProgram program;

    Transform transform;
    MaterialPtr material;
    
    Object(Transform t, MaterialPtr m)
        : transform(t)
        , material(m)
    {}

    virtual void draw(const View& view, LightPtr light) = 0;
    virtual void loadProgram() = 0;
};
typedef shared_ptr<Object> ObjectPtr;

class Torus : public Object {
private:
    Mesh2DPtr mesh2d;

public:
    Torus(Transform t, MaterialPtr m);
    ~Torus();
    void loadProgram();
    void draw(const View& view, LightPtr light);
};
typedef shared_ptr<Torus> TorusPtr;

struct Scene {
    Camera camera;
    View view;
    vector<LightPtr> light_list;
    vector<ObjectPtr> object_list;
    CubeMapPtr envmap;

    Scene(const Camera& c, const View& v);
    void setView(const View& v);
    void setCamera(const Camera& c);
    void draw();
    void addObject(ObjectPtr object);
    void addLight(LightPtr light);
    void setEnvMap(CubeMapPtr envmap);
};
typedef shared_ptr<Scene> ScenePtr;



#endif // __scene_hpp__
