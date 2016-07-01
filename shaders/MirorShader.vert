#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 norm;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 camera_pos_world;


out vec3 eyeDirection_camspace;
out vec3 normal_cam_space;



vec4 light_position1 = vec4(  3.0, 2.0,  1.0, 1.f); // First light position
vec4 light_position2 = vec4( -3.0, 0.0,  1.0, 1.f); // Second light position


void main()
{
    gl_Position = MVP * position;
    normal_cam_space = (V * M * norm).xyz;
    vec3 vert_pos_camspace_not_trans = (V * M * position).xyz;
    eyeDirection_camspace = vec3(0,0,0) - vert_pos_camspace_not_trans;
}