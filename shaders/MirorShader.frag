#version 330

#define M_PI 3.1415926535897932384626433832795


in vec3 eyeDirection_camspace;
in vec3 normal_cam_space;

uniform sampler2D sphere_color;
uniform vec3 camera_pos_world;

out vec4 outColor;

vec4 miror();
void main()
{
    outColor = miror();
}

vec4 miror()
{
    vec2 fragTexCoord1;

    // Set texture coordinates using spherical mapping:
    {
        vec3 r_1 = -reflect(normalize(eyeDirection_camspace), normalize(normal_cam_space));
        float theta = atan(r_1.x/r_1.z);
        float phi   = atan(r_1.y/length(vec2(r_1.x,r_1.z)));
        float r     = length(r_1.xyz);
        fragTexCoord1.x = (1.f - (theta + M_PI) / (2*M_PI));
        fragTexCoord1.y = (1.f - (phi + M_PI/2)  / M_PI);
    }


    vec4 reflection1 = texture(sphere_color, fragTexCoord1);

    return vec4(reflection1.xyz, 1.f);
}