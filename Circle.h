//
// Created by jadams on 11/22/15.
//

#ifndef EX2_CIRCLE_H
#define EX2_CIRCLE_H
//setenv LD_LIBRARY_PATH /cs/course/2013/cg/OpenMesh/lib/:/cs/course/2013/cg/bimage/:/cs/course/2013/cg/linear/

#include "ShaderIO.h"
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <iostream>


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define TRIANGLE_AMOUNT 300.0
#define CIRCLE_RADIUS 0.775f
#define SHADERS_DIR "shaders/"

class Circle {
    GLuint _vao, _vbo;
    std::vector<float> vertices;
    // Attribute handle:
    GLint _posAttrib;
    GLint _fillColorUV;



public:
    Circle();
    virtual ~Circle();
    void draw();
    void init();
    GLuint _program;
};

#endif //EX2_CIRCLE_H