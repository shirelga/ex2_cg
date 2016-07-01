//
// Created by shirelga on 11/22/15.
//

#ifndef EX2_OPENMESHIO_H
#define EX2_OPENMESHIO_H

#ifdef DEBUG
#define DEB 1
#else
#define DEB 0
#endif

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <vector>
#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "ShaderIO.h"
#include "bimage.h"

#define SHADERS_DIR "shaders/"
#define OBJECT_DEPTH 5.f
#define OBJECT_B_RAD 4.9f
#define FOV 30.0f
#define CIRCLE_RADIUS 0.775f
#define ORTHGONAL 0
#define PERSPECTIVE 1
#define FACE_NORM 1
#define AVG_FACE_NORM 0
#define SPEC_EXP 200.f
#define TEX_SCALE 1.f
#define TURB_COEFF 0.f
#define PHONG 1
#define GOURAUD 2
#define RGB 3
typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;

class OpenMeshObject {
private:

    Mesh mesh;

    GLuint _vao, _vbo, _vbo_norm, _vbo_avg_norm;
    GLint _posAttrib;
    GLint _fillColorUV;
    // Texture handles:
    GLuint _sphere_tex, _brick_tex, _brick_bump;
    float diff[3];
    float max_diff;
    float mid_point[3], lower[3], higher[3];

    std::vector<float> vertices_rgb;
    std::vector<float> vertices_normal;
    std::vector<float> vertices_avg_normal;

    //the world mat of the object
    glm::mat4 _modelMat;
    //perspective matrix, perspective by default of orthographic
    glm::mat4 _perspective;
    //perspective matrix, used for rotation, scale and translation
    glm::mat4 _view;
    glm::vec3 viewer;

    int _perspective_mode;
    int _normal_mode;
    int current_x,current_y;
    int last_x,last_y;
    int right_down;
    int left_down;
    int middle_down;
    unsigned int texture_num;
    int shading_mode;

    float specExp, textureScale, turbulenceCoeff;
    std::vector<std::string> tex_programs;




public:
    float window_width, window_height;

    OpenMeshObject();

    // Load a mesh from the given input file
    void loadMesh(Mesh &mesh, const char *filename);

    /** This function computes the geometrical center and the axis aligned bounding box of the
    object. The bounding box is represented by the lower left and upper right corners. */
    void computeBoundingBox(Mesh &mesh);

    /* this function computes the center of each mesh */
    void init(const char* file_name, float width, float height);

    //draw the open mesh object
    void draw(GLenum polygonMode);


    void adjust_perspective();

    void perspective();


    float normalise(float value, int axis);

    float normalise_colour(float value, int axis);
    void translate();
    void update_current_pos(int x, int y);
    void update_last_pos(int x,int y);
    void update_right_down(int right_down_new);
    int get_right_down();
    void update_left_down(int left_down_new);
    int get_left_down();
    void rotate();
    glm::vec3 get_arcball_vector(float x, float y);
    void scale();
    void update_middle_down(int middle_down_new);
    int get_middle_down();
    void set_perspective_mode(int mode);
    int get_perspective_mode();
    void modify_vertex_normals();
    void calculate_vertex_normal(Mesh::VertexHandle &vHandle, Mesh::FaceHandle &fHandle);
    void set_normal_mode(int mode);
    int get_normal_mode();
    void set_shading_mode(int mode);
    void set_shine(int mode);
    void set_texture_scale(int mode);
    void set_trub_coeff(int mode);
    void set_texture();
    void reset();

    GLuint _program;
};

#endif //EX2_OPENMESHIO_H
