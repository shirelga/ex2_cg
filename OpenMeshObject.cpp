//
// Created by shirelga on 11/22/15.
//

#include <GL/glew.h>
#include "OpenMeshObject.h"
#include <string>
OpenMeshObject::OpenMeshObject():_vao(0), _vbo(0),  texture_num(0),shading_mode(PHONG), specExp(SPEC_EXP),textureScale(TEX_SCALE), turbulenceCoeff(TURB_COEFF),
                                 tex_programs{"openmeshMarble", "openmeshWood", "openmeshMiror", "openmeshBrick", "openmeshPhong"}
{
}


// Load a mesh from the given input file
void OpenMeshObject::loadMesh(Mesh& mesh, const char* filename)
{
    if (!OpenMesh::IO::read_mesh(mesh, filename))
    {
        // if we didn't make it, exit...
        fprintf(stderr, "Error loading mesh, Aborting.\n");
        return;
    }
}

/** This function computes the geometrical center and the axis aligned bounding box of the
object. The bounding box is represented by the lower left and upper right corners. */
void OpenMeshObject::computeBoundingBox(Mesh &mesh)
{
    // Vertex iterator is an iterator which goes over all the vertices_rgb of the mesh:
    Mesh::VertexIter vertexIter;
    // This is used to store the geometrical position of the vertices_rgb of the mesh:
    Mesh::Point p, center;
    const float fm = std::numeric_limits<float>::max();
    Mesh::Point lowerLeft(fm, fm, fm);
    Mesh::Point upperRight(-fm,-fm,-fm);
    vertexIter = mesh.vertices_begin();
    lowerLeft = upperRight = mesh.point(vertexIter);
    // This is how to go over all the vertices_rgb in the mesh:

    for (vertexIter = mesh.vertices_begin();vertexIter != mesh.vertices_end();++vertexIter){

        // This is how to get the extrema associated with the set of vertices_rgb:
        p = mesh.point(vertexIter);
        center += p;
        for (int i = 0;i < 3;i++) {
            lowerLeft[i] = std::min(lowerLeft[i], p[i]);
            upperRight[i] = std::max(upperRight[i],p[i]);
        }
    }

    //calculates the differences and the mid points.
    for(int i=0; i<3;i++) {
        lower[i] = lowerLeft[i];
        higher[i] = upperRight[i];
        diff[i] = upperRight[i] - lowerLeft[i];
        mid_point[i] = (lowerLeft[i] + upperRight[i]) / 2;
    }
    //calculate the max difference (i.e the 'largest' axis)
    max_diff=std::max(diff[0],diff[1]);
    max_diff=std::max(max_diff,diff[2]);
}

/**axis: 0 x, 1 y, 2 z ->normalises the value correctly based on proportional magnitudes of the axes. */
float OpenMeshObject::normalise(float value, int axis) {
    return (((value - lower[axis]) / (diff[axis])) * 2 - 1) * (diff[axis] / max_diff);
}

float OpenMeshObject::normalise_colour(float value, int axis) {
    return ((value - lower[axis])/(diff[axis]));
}

/* this function computes the center of each mesh */
void OpenMeshObject::init(const char* file_name, float width, float height)
{
    programManager::sharedInstance()
            .createProgram("openmesh",
                           SHADERS_DIR "OpenMeshShader.vert",
                           SHADERS_DIR "OpenMeshShader.frag");
    programManager::sharedInstance()
            .createProgram("openmeshPhong",
                           SHADERS_DIR "OpenMeshShaderPhong.vert",
                           SHADERS_DIR "OpenMeshShaderPhong.frag");
    programManager::sharedInstance()
            .createProgram("openmeshGouraud",
                           SHADERS_DIR "OpenMeshShaderGouraud.vert",
                           SHADERS_DIR "OpenMeshShaderGouraud.frag");
    programManager::sharedInstance()
            .createProgram("openmeshMarble",
                           SHADERS_DIR "OpenMeshShaderPhong.vert",
                           SHADERS_DIR "MarbleShader.frag");
    programManager::sharedInstance()
            .createProgram("openmeshWood",
                           SHADERS_DIR "OpenMeshShaderPhong.vert",
                           SHADERS_DIR "WoodShader.frag");
    programManager::sharedInstance()
            .createProgram("openmeshMiror",
                           SHADERS_DIR "MirorShader.vert",
                           SHADERS_DIR "MirorShader.frag");
    programManager::sharedInstance()
            .createProgram("openmeshBrick",
                           SHADERS_DIR "BrickShader.vert",
                           SHADERS_DIR "BrickShader.frag");
//    specExp = 200.f; // Default Shininess (can be changed dynamically by the user)
//    viewer = glm::vec3(0,0,OBJECT_DEPTH);
    shading_mode = PHONG;
    specExp = SPEC_EXP;
    textureScale = TEX_SCALE;
    turbulenceCoeff = TURB_COEFF;
    window_width = width;
    window_height = height;
    loadMesh(mesh, file_name);
    computeBoundingBox(mesh);
    //init matrices to identity
    _modelMat = glm::mat4(1.f);
    _view = glm::mat4(1.f);
    _perspective = glm::mat4(1.f);
    _normal_mode = FACE_NORM;

    //add normal attribute to the mesh and calculate it.
    mesh.request_face_normals();
    mesh.request_vertex_normals();
    mesh.update_normals();

    /* face iterator */
    Mesh::FaceIter fIter;
    Mesh::FaceHandle fHandle;
    Mesh::VertexHandle vHandle;
    /* face vertex iterator: this iterator goes through all the vertices_rgb which belong to the
   face */
    /* This special type of iterator is called circulator in OpenMesh terms */
    /* There are many types of circulators such as VertexVertex or FaceEdge VertexFace etc.
   */
    Mesh::FaceVertexIter fvIter;
    /* go over all the faces */
    for (fIter = mesh.faces_begin();fIter != mesh.faces_end(); ++fIter)
    {
        fHandle = fIter.handle();
        Mesh::Point center(0,0,0);
        /* for each face - go over all the vertices_rgb that belong to it and compute their
       average position (center) of face */
        /* notice the termination condition of this iterator */
        for (fvIter = mesh.fv_iter(fHandle);fvIter;++fvIter)
        {
            vHandle = fvIter.handle();
            //sort out vertices_rgb
            for(int i=0;i<3;i++) {
                vertices_rgb.push_back(normalise(mesh.point(fvIter)[i], i));
                vertices_normal.push_back(normalise(mesh.point(fvIter)[i], i));
                vertices_avg_normal.push_back(normalise(mesh.point(fvIter)[i], i));
            }
            vertices_rgb.push_back(1.f);
            vertices_normal.push_back(1.f);
            vertices_avg_normal.push_back(1.f);


            //sort out colours
            for(int i=0;i<3;i++) {
                vertices_rgb.push_back(normalise_colour(mesh.point(fvIter)[i], i));
            }
            vertices_rgb.push_back(1.f);
            calculate_vertex_normal(vHandle, fHandle);
        }
    }
    mesh.release_face_normals();
    mesh.release_vertex_normals();

    glGenVertexArrays(1, &_vao);

    // Create and load vertex data into a Vertex Buffer Object:
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_rgb.size() * sizeof(float), &vertices_rgb[0], GL_STATIC_DRAW);


    //init perspective and ratio to circle
    _perspective_mode = PERSPECTIVE;
    perspective();

    glGenBuffers(1, &_vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, vertices_normal.size() * sizeof(float), &vertices_normal[0], GL_STATIC_DRAW);

    glGenBuffers(1, &_vbo_avg_norm);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo_avg_norm);
    glBufferData(GL_ARRAY_BUFFER, vertices_avg_normal.size() * sizeof(float), &vertices_avg_normal[0], GL_STATIC_DRAW);
    set_shading_mode(1);
    glUniform1f(glGetUniformLocation(_program, "shine"), specExp);
    glUniform1f(glGetUniformLocation(_program, "texScale"), textureScale);
    glUniform1f(glGetUniformLocation(_program, "turbCoeff"), turbulenceCoeff);

    // Load and bind textures:
    {
        // Load a bitmap file:
        {
            const char*sphere_texture_filename = "textures/spheremap2.bmp";
            BImage image(sphere_texture_filename);

            glGenTextures(1, &_sphere_tex); // Generate a texture in OpenGL
            glBindTexture(GL_TEXTURE_2D, _sphere_tex); // Bind texture before setting its properties

            // Setting texture properties
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Transfer data to texture:
            glTexImage2D(GL_TEXTURE_2D,
                         0, // level
                         GL_RGBA, // Internal representation
                         image.width(), image.height(), // Texture size
                         0, // must be 0
                         GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                         image.getImageData()); // Pixel data
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // Load bump map:
        {
            const char*brick_texture_filename = "textures/brickwork-texture.bmp";
            BImage image(brick_texture_filename);

            glGenTextures(1, &_brick_tex);
            glBindTexture(GL_TEXTURE_2D, _brick_tex);
            // Setting texture properties
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Transfer data to texture:
            glTexImage2D(GL_TEXTURE_2D,
                         0, // level
                         GL_RGBA, // Internal representation
                         image.width(), image.height(), // Texture size
                         0, // must be 0
                         GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                         image.getImageData()); // Pixel data
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // Load bump map:
        {
            const char* specmap_filename = "textures/brickwork-bump-map.bmp";
            BImage image( specmap_filename );

            glGenTextures(1, &_brick_bump);
            glBindTexture(GL_TEXTURE_2D, _brick_bump);
            // Setting texture properties
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Transfer data to texture:
            glTexImage2D(GL_TEXTURE_2D,
                         0, // level
                         GL_RGBA, // Internal representation
                         image.width(), image.height(), // Texture size
                         0, // must be 0
                         GL_RGB, GL_UNSIGNED_BYTE, // Pixel data format
                         image.getImageData()); // Pixel data
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

}



void OpenMeshObject::draw(GLenum polygonMode) {

    // Set the program to be used in subsequent lines:
    glUseProgram(_program);

    //GLenum polygonMode = GL_LINE;   // Also try using GL_FILL and GL_POINT
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Set texture
    {
        int loc0 = glGetUniformLocation(_program, "sphere_color");
        glUniform1i(loc0, 0);
        int loc1 = glGetUniformLocation(_program, "brick_color");
        glUniform1i(loc1, 1);
        int loc2 = glGetUniformLocation(_program, "brick_bump");
        glUniform1i(loc2, 2);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _sphere_tex);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _brick_tex);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, _brick_bump);
    }

    //calculate the number of vertices_rgb to draw.
    size_t numberOfVertices = mesh.n_faces()*3;
    //update the transformations before the draw.
    glUniform1f(glGetUniformLocation(_program, "shine"), specExp);
    glUniform1f(glGetUniformLocation(_program, "texScale"), textureScale);
    glUniform1f(glGetUniformLocation(_program, "turbCoeff"), turbulenceCoeff);
    glUniformMatrix4fv(glGetUniformLocation(_program, "MVP"), 1, GL_FALSE,
                                           glm::value_ptr(_perspective * _view *  _modelMat));
    glUniformMatrix4fv(glGetUniformLocation(_program, "V"), 1, GL_FALSE,
                       glm::value_ptr(_view));
    glUniformMatrix4fv(glGetUniformLocation(_program, "M"), 1, GL_FALSE,
                       glm::value_ptr(_modelMat));
    glUniform3fv(glGetUniformLocation(_program, "camera_pos_world"),  1, glm::value_ptr(viewer));
    // Draw using the state stored in the Vertex Array object:
    glBindVertexArray(_vao);
    //draw
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

    // Unbind the Vertex Array object
    glBindVertexArray(0);

    // Cleanup, not strictly necessary
    glUseProgram(0);
}

/**
 * adjust the ratio of the object into the circle for the prespectives
 */
void OpenMeshObject::adjust_perspective()
{
    glUseProgram(_program);
    glm::mat4 view;
    if(_perspective_mode == PERSPECTIVE)
    {
        //calculate the wanted height and normlize it between 1-5
        float wanted_depth = (glm::tan(glm::radians(FOV/2.f)) * OBJECT_DEPTH)*OBJECT_DEPTH - 4;
        view = glm::lookAt(
                glm::vec3(0, 0, OBJECT_DEPTH + wanted_depth), //position camera center
                glm::vec3(0, 0, -1.f), //position object center
                glm::vec3(0, 1.f, 0)  //position y axis up
        );
        viewer = glm::vec3(0, 0, OBJECT_DEPTH + wanted_depth);
    }
    else
    {
        view = glm::lookAt(
                glm::vec3(0, 0, OBJECT_DEPTH), //position camera center
                glm::vec3(0, 0, -1.f), //position object center
                glm::vec3(0, 1.f, 0)  //position y axis up
        );
        viewer = glm::vec3(0, 0, OBJECT_DEPTH);
    }
    _view = view;
}

/**
 * move between othographic projection to prespective projection
 */
void OpenMeshObject::perspective()
{
    glm::mat4 perspective_mat;
    if(_perspective_mode == PERSPECTIVE)
    {
        perspective_mat = glm::perspective(glm::radians(FOV), 1.f, OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
    }
    else
    {
        //calculate the bottom of he object
        float bottom = (glm::tan(glm::radians(FOV/2.f)) * OBJECT_DEPTH)*5.f - 4.7f;
        perspective_mat = glm::ortho(-bottom, bottom, -bottom, bottom, OBJECT_DEPTH - OBJECT_B_RAD, OBJECT_DEPTH + OBJECT_B_RAD);
    }
    adjust_perspective();
    _perspective = perspective_mat;
}


void OpenMeshObject::translate( ) {
    //only enter if there's been some movement.
    if (current_x!=last_x || current_y!=last_y) {
        //calculate the difference vector.
        glm::vec3 difference= glm::vec3(1.0f*(current_x-last_x)/window_width,1.0f*(last_y-current_y)/window_height,0.0f);
        //obtain the transformation matrix
        glm::mat4 translation= glm::translate(glm::mat4(1.0f),difference);
        //pre multiply to perform the translation and update the model matrix.
        _modelMat = translation * _modelMat;
        //update the x/y mouse positions.
        last_x=current_x;
        last_y=current_y;
    }

}

void OpenMeshObject::update_current_pos(int x, int y) {
    current_x=x;
    current_y=y;
}

void OpenMeshObject::update_last_pos(int x,int y) {
    last_x=x;
    last_y=y;
}

void OpenMeshObject::update_right_down(int right_down_new) {
    right_down = right_down_new;
}

int OpenMeshObject::get_right_down() {
    return right_down;
}

void OpenMeshObject::update_left_down(int left_down_new) {
    left_down = left_down_new;
}

int OpenMeshObject::get_left_down() {
    return left_down;
}

void OpenMeshObject::update_middle_down(int middle_down_new) {
    middle_down = middle_down_new;
}

int OpenMeshObject::get_middle_down() {
    return  middle_down;
}


void OpenMeshObject::rotate() {
    //only enter if there's been some movement.
    if (current_x!=last_x || current_y!=last_y) {

        //get the normalised vectors from the centre to the points on the sphere
        glm::vec3 va = get_arcball_vector(last_x, last_y);
        glm::vec3 vb = get_arcball_vector(current_x,  current_y);

        //calculate the angle and scale it so it rotates at a nice speed. Also negate, so it goes
        //according to the school solution.
        float angle = glm::angle(va, vb);
        //get the cross product
        glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
        //obtain the transformation matrix
        glm::mat4 temp_rotate= glm::rotate(glm::mat4(1.0f), glm::radians(angle) * 2, axis_in_camera_coord);
        //premultiply to update the model matrix
        _modelMat = temp_rotate * _modelMat;
        //update the mouse positions
        last_x = current_x;
        last_y = current_y;
    }
}

void OpenMeshObject::scale() {
    //only enter if there's been some movement.
    if (current_x!=last_x || current_y!=last_y) {
        float diff_y=(last_y-current_y)/window_height;
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1-diff_y,1-diff_y,1-diff_y));
        _modelMat = scale * _modelMat;
        last_x=current_x;
        last_y=current_y;
    }

}

/** function to get arcball normal vectors */
glm::vec3 OpenMeshObject::get_arcball_vector(float x, float y) {
    //calculate the ratio to the screen when resulation is changed
    float max_axis = std::max(x, y);
    x = x + max_axis/2.f - window_width/2.f;
    y = y + max_axis/2.f - window_height/2.f;
    glm::vec3 P = glm::vec3((x/max_axis)*2.f - 1.0f,
                            (y/max_axis)*2.f - 1.0f,
                            0.f);
    P.y = -P.y;
    float OP_squared = P.x * P.x + P.y * P.y;
    if (OP_squared <= std::pow(0.9f, 2.f))
        P.z = sqrt((float)std::pow(0.9f, 2.f) - OP_squared);  // Pythagore
    else P = glm::normalize(P);
    return P;
}

void OpenMeshObject::set_perspective_mode(int mode)
{
    _perspective_mode = mode;
    perspective();
}

int OpenMeshObject::get_perspective_mode()
{
    return _perspective_mode;
}

//resets the transformation matrix to the identity matrix, returning the mesh to the centre without any transformations.
void OpenMeshObject::reset() {
    _modelMat=glm::mat4(1.f);
    specExp = SPEC_EXP;
    textureScale = TEX_SCALE;
    turbulenceCoeff = TURB_COEFF;
    if(shading_mode == PHONG) {
        texture_num = 0;
        set_shading_mode(PHONG);
    }
}

void OpenMeshObject::modify_vertex_normals() {
    glBindVertexArray(_vao);
    if(_normal_mode)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_avg_norm);

    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_norm);
    }
    // Obtain attribute handles:
    _posAttrib = glGetAttribLocation(_program, "position");
    glEnableVertexAttribArray(_posAttrib);
    glVertexAttribPointer(_posAttrib, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          sizeof(glm::vec4)*2,
                          0);
    _fillColorUV = glGetAttribLocation(_program, "norm");
    glEnableVertexAttribArray(_fillColorUV);
    glVertexAttribPointer(_fillColorUV, // attribute handle
                          4,          // number of scalars per vertex
                          GL_FLOAT,   // scalar type
                          GL_FALSE,
                          sizeof(glm::vec4)*2,
                          (GLvoid*)(sizeof(glm::vec4)));
    glBindVertexArray(0);
}

void OpenMeshObject::calculate_vertex_normal(Mesh::VertexHandle &vHandle, Mesh::FaceHandle &fHandle)
{
    OpenMesh::VectorT<float, 3> meshNorm;
    meshNorm = mesh.normal(fHandle);
    for(int i = 0; i < 3; i++)
    {
        vertices_normal.push_back(meshNorm[i]);
    }
    vertices_normal.push_back(0.f);
    meshNorm = mesh.calc_vertex_normal(vHandle);
    for(int i = 0; i < 3; i++)
    {
        vertices_avg_normal.push_back(meshNorm[i]);
    }
    vertices_avg_normal.push_back(0.f);
}

void OpenMeshObject::set_normal_mode(int mode)
{
    _normal_mode = mode;
    modify_vertex_normals();
}
int OpenMeshObject::get_normal_mode()
{
    return _normal_mode;
}

void OpenMeshObject::set_shading_mode(int mode)
{
    if(mode == PHONG)
    {
        shading_mode = PHONG;
        _program = programManager::sharedInstance().programWithID(tex_programs[texture_num]);
        glUniform1f(glGetUniformLocation(_program, "shine"), specExp);
        glUniform1f(glGetUniformLocation(_program, "texScale"), textureScale);
        glUniform1f(glGetUniformLocation(_program, "turbCoeff"), turbulenceCoeff);
        set_normal_mode(get_normal_mode());
    }
    else if(mode == GOURAUD)
    {
        shading_mode = GOURAUD;
        _program = programManager::sharedInstance().programWithID("openmeshGouraud");
//        glUniform1f(glGetUniformLocation(_program, "shine"), specExp);
        set_normal_mode(get_normal_mode());
        return;
    }
    else
    {
        shading_mode = RGB;
        _program = programManager::sharedInstance().programWithID("openmesh");
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        _posAttrib = glGetAttribLocation(_program, "position");
        glEnableVertexAttribArray(_posAttrib);
        glVertexAttribPointer(_posAttrib, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              sizeof(glm::vec4)*2,
                              0);

        _fillColorUV = glGetAttribLocation(_program, "color");
        glEnableVertexAttribArray(_fillColorUV);
        glVertexAttribPointer(_fillColorUV, // attribute handle
                              4,          // number of scalars per vertex
                              GL_FLOAT,   // scalar type
                              GL_FALSE,
                              sizeof(glm::vec4)*2,
                              (GLvoid*)(sizeof(glm::vec4)));
        glBindVertexArray(0);
    }

}

void OpenMeshObject::set_shine(int mode)
{
    if(mode)
    {
        if(specExp < 2001.f)
        {
            specExp += 2.f;
        }
    }
    else
    {
        if(specExp > 0.f)
        {
            specExp -= 2.f;
        }
    }
}

void OpenMeshObject::set_texture_scale(int mode)
{
    if(mode)
    {
        textureScale += 0.2f;
    }
    else
    {
        if(textureScale > 0)
        {
            textureScale -= 0.2f;
        }
    }
}

void OpenMeshObject::set_trub_coeff(int mode)
{
    if(mode)
    {
        turbulenceCoeff += 0.2f;
    }
    else
    {
        if(textureScale > 0)
        {
            turbulenceCoeff -= 0.2f;
        }
    }
}

void OpenMeshObject::set_texture()
{
    if(shading_mode == PHONG) {
        if (texture_num < (tex_programs.size() - 1)) {
            texture_num++;
        }
        else {
            texture_num = 0;
        }
        set_shading_mode(PHONG);
    }
}



