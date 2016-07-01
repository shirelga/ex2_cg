
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 norm;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 camera_pos_world;
uniform float shine;

out vec4 outIntensity;

vec3 eyeDirection_camspace;
vec3 normal_cam_space;
vec3 light_position_camspace1;
vec3 light_position_camspace2;

//vec4 normal = norm;
//vec4 viewer = normalize(vec4(view, 1.f) - (transformation * position));
vec3 lightColor1 = vec3(1.0, 0.9, 0.7); // First light color
vec3 lightColor2 = vec3(0.6, 0.6, 1.0); // Second light color
vec3 ambientColor = vec3(1.0, 1.0, 1.0); // Ambient light color

vec4 light_position1 = vec4(  3.0, 2.0,  1.0, 1.f); // First light position
vec4 light_position2 = vec4( -3.0, 0.0,  1.0, 1.f); // Second light position
vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
vec3 kd = vec3(0.3, 0.3, 0.3); // Diffuse coefficient
vec3 ks = vec3(0.3, 0.3, 0.3); // Specular coefficient

vec4 GouraudModel();
void main()
{
    gl_Position = MVP * position;

    vec3 vert_pos_camspace = (V * M * position).xyz;
    eyeDirection_camspace = camera_pos_world - vert_pos_camspace;

    light_position_camspace1 = normalize((V * light_position1).xyz + eyeDirection_camspace); // First light position
    light_position_camspace2 = normalize((V * light_position2).xyz + eyeDirection_camspace); // Second light position

    normal_cam_space = normalize(V * M * norm).xyz;

    outIntensity = GouraudModel();
}

vec4 GouraudModel()
{
    float norm_light_angel_1 = max(clamp(dot(light_position_camspace1, normal_cam_space), 0, 1), 0.f);
    float norm_light_angel_2 = max(clamp(dot(light_position_camspace2, normal_cam_space),0 ,1), 0.f);

    vec3 r_1 = -reflect(light_position_camspace1, normal_cam_space);
    vec3 r_2 = -reflect(light_position_camspace2, normal_cam_space);
    float viewer_reflector_angle_1 = pow(max(clamp(dot(r_1, normalize(eyeDirection_camspace)), 0 ,1), 0.f), shine);
    float viewer_reflector_angle_2 = pow(max(clamp(dot(r_2, normalize(eyeDirection_camspace)), 0, 1), 0.f), shine);

    vec4 i;
    vec3 i_a = ka * (ambientColor);
    vec3 i_d = (kd * lightColor1 * norm_light_angel_1) + (kd * lightColor2 * norm_light_angel_2);
    vec3 i_s = (ks * lightColor1 * viewer_reflector_angle_1) + (ks * lightColor2 * viewer_reflector_angle_2);
    i = vec4(i_a + i_d + i_s, 1.f);
    return i;
}