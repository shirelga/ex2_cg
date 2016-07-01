#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 norm;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 camera_pos_world;

out vec3 eyeDirection_camspace;
out vec3 normal_cam_space;
out vec3 light_position_camspace1;
out vec3 light_position_camspace2;
out vec3 vert_pos_camspace_not_trans;


vec4 light_position1 = vec4(  3.0, 2.0,  1.0, 1.f); // First light position
vec4 light_position2 = vec4( -3.0, 0.0,  1.0, 1.f); // Second light position


void main()
{
    gl_Position = MVP * position;

    vec3 vert_pos_camspace = (V * M * position).xyz;
    vert_pos_camspace_not_trans = (V * position).xyz;
    eyeDirection_camspace = camera_pos_world - vert_pos_camspace;

    light_position_camspace1 = (V * vec4(3.0, 2.0, 1.0, 1.f)).xyz + eyeDirection_camspace; // First light position
    light_position_camspace2 = (V * vec4( -3.0, 0.0, 1.0, 1.f)).xyz + eyeDirection_camspace; // Second light position

    normal_cam_space = (V * M * norm).xyz;
}