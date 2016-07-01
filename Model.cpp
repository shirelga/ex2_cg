//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Circle.h"


#define SHADERS_DIR "shaders/"

Model::Model() : _width(600), _height(600)
{
}

Model::~Model()
{
}

void Model::init(const char* file_name)
{
    _open.init(file_name,_width,_height);
    circle.init();
    _programCircle = circle._program;
    _programOpenMesh =  _open._program;
    //for the circle TODO: move to function
    glEnable(GL_DEPTH_TEST);
    glUseProgram(_programCircle);
    glUniform2f(glGetUniformLocation(_programCircle, "window"), _width, _height);
    glUseProgram(_programOpenMesh);
    glUniform2f(glGetUniformLocation(_programOpenMesh, "window"), _width, _height);
}

void Model::draw(GLenum polygonMode)
{
    _open.draw(polygonMode);
	circle.draw();
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _open.window_height=height;
    _open.window_width=width;
    //for the circle TODO: move to function
    glUseProgram(_programCircle);
    glUniform2f(glGetUniformLocation(_programCircle, "window"), _width, _height);
    glUseProgram(_programOpenMesh);
    glUniform2f(glGetUniformLocation(_programOpenMesh, "window"), _width, _height);

    _offsetX = 0;
    _offsetY = 0;
}




