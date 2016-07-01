//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#include "Circle.h"
#include "OpenMeshObject.h"

class Model {

	
	// View port frame:
	float _width, _height, _offsetX, _offsetY;

	GLuint _programCircle, _programOpenMesh;
	Circle circle;


public:
	Model();

public:
	virtual ~Model();

public:
	void init(const char* file_name);
	
public:
	void draw(GLenum polygonMode);
	
public:
	void resize(int width, int height);
	OpenMeshObject _open;
};

#endif /* defined(__ex0__Model__) */
