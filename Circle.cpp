//
// Created by jadams on 11/22/15.
//

#include "Circle.h"

Circle::Circle() :
        _vao(0), _vbo(0)
{
}


Circle::~Circle() {

    if (_vao != 0)
        glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0)
        glDeleteBuffers(1, &_vbo);
}

void Circle::init() {

//    //push centre vertex:
//    for(int i=0;i<3;i++) vertices_rgb.push_back(0.0f);
//
//    vertices_rgb.push_back(1.0f);
    programManager::sharedInstance()
            .createProgram("circle",
                           SHADERS_DIR "CircleShader.vert",
                           SHADERS_DIR "CircleShader.frag");
    _program = programManager::sharedInstance().programWithID("circle");

    _fillColorUV  = glGetUniformLocation(_program, "fillColor");
    std::vector<float> vertices;
    //work out and push the rest of the vertices_rgb.

    for (int i=0; i < TRIANGLE_AMOUNT; i++) {

        vertices.push_back((float)(CIRCLE_RADIUS * std::cos(2.0 * M_PI * i / TRIANGLE_AMOUNT)));
        vertices.push_back((float)(CIRCLE_RADIUS *  std::sin(2.0 * M_PI * i / TRIANGLE_AMOUNT)));
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
    }

    // Create and bind the object's Vertex Array Object:
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Create and load vertex data into a Vertex Buffer Object:
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Obtain attribute handles:
    _posAttrib = glGetAttribLocation(_program, "position");
    glEnableVertexAttribArray(_posAttrib);
    glVertexAttribPointer(_posAttrib, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          0,
                          0);

    // Unbind vertex array:
    glBindVertexArray(0);

}

void Circle::draw() {

    // Set the program to be used in subsequent lines:
    glUseProgram(_program);

    GLenum polygonMode = GL_LINE;   // Also try using GL_FILL and GL_POINT
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Set uniform variable with RGB values:
    //pink would be 255, 102, 204
    float red = 1.0f; float green = (102.0f)/255; float blue = (204.f)/255;
    glUniform4f(_fillColorUV, red, green, blue, 1.0f);

    // Draw using the state stored in the Vertex Array object:
    glBindVertexArray(_vao);

    glDrawArrays(GL_LINE_LOOP, 0, TRIANGLE_AMOUNT);

    // Unbind the Vertex Array object
    glBindVertexArray(0);

    // Cleanup, not strictly necessary
    glUseProgram(0);
}