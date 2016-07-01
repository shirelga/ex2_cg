
#version 330

#define M_PI 3.1415926535897932384626433832795

uniform float shine;
uniform float texScale;
uniform float turbCoeff;
in vec3 eyeDirection_camspace;
in vec3 normal_cam_space;
in vec3 light_position_camspace1;
in vec3 light_position_camspace2;
in vec3 vert_pos_camspace;

out vec4 outColor;

vec3 lightColor1 = vec3(1.0, 0.9, 0.7); // First light color
vec3 lightColor2 = vec3(0.6, 0.6, 1.0); // Second light color
vec3 ambientColor = vec3(1.0, 1.0, 1.0); // Ambient light color

vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
vec3 kd = vec3(0.3, 0.3, 0.3); // Diffuse coefficient
vec3 ks = vec3(0.3, 0.3, 0.3); // Specular coefficient
vec4 phongModel();
void main()
{
    outColor = phongModel();
}

vec4 phongModel()
{
    float norm_light_angel_1 = max(clamp(dot(normalize(light_position_camspace1), normalize(normal_cam_space)), 0, 1), 0.f);
    float norm_light_angel_2 = max(clamp(dot(normalize(light_position_camspace2), normalize(normal_cam_space)),0 ,1), 0.f);

    vec3 r_1 = -reflect(normalize(light_position_camspace1), normalize(normal_cam_space));
    vec3 r_2 = -reflect(normalize(light_position_camspace2), normalize(normal_cam_space));
    float viewer_reflector_angle_1 = pow(max(clamp(dot(r_1, normalize(eyeDirection_camspace)), 0 ,1), 0.f), shine);
    float viewer_reflector_angle_2 = pow(max(clamp(dot(r_2, normalize(eyeDirection_camspace)), 0, 1), 0.f), shine);

    vec4 i;
    vec3 i_a = ka * (ambientColor);
    vec3 i_d = (kd * lightColor1 * norm_light_angel_1) + (kd * lightColor2 * norm_light_angel_2);
    vec3 i_s = (ks * lightColor1 * viewer_reflector_angle_1) + (ks * lightColor2 * viewer_reflector_angle_2);
    i = vec4(i_a + i_d + i_s, 1.f);
    return i;
}