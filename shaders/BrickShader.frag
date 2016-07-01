#version 330

#define M_PI 3.1415926535897932384626433832795
uniform sampler2D brick_color;
uniform sampler2D brick_bump;

in float shineCoef;
in float textureScale;
in float turbulenceCoeff;
in vec3 eyeDirection_camspace;
in vec3 normal_cam_space;
in vec3 light_position_camspace1;
in vec3 light_position_camspace2;
in vec3 vert_pos_camspace;
in vec3 vert_pos_camspace_not_trans;
in vec3 fragTexCoord;

out vec4 outColor;


vec3 lightColor1 = vec3(1.0, 0.9, 0.7); // First light color
vec3 lightColor2 = vec3(0.6, 0.6, 1.0); // Second light color
vec3 ambientColor = vec3(1.0, 1.0, 1.0); // Ambient light color

vec3 ka = vec3(0.1, 0.1, 0.1); // Ambient coefficient
vec3 kd; // Diffuse coefficient
vec3 ks = vec3(0.3, 0.3, 0.3); // Specular coefficient
vec4 phongModel(vec3 normal);
void main()
{
    vec2 fragTexCoord_corr;
    //check with side of the cube we're on, and apply the texture accordingly
    if (fragTexCoord.z < 0.0001 || fragTexCoord.z > 0.9999) fragTexCoord_corr=vec2(fragTexCoord.x,fragTexCoord.y);
    else if(fragTexCoord.x< 0.0001 || fragTexCoord.x> 0.9999) fragTexCoord_corr=vec2(fragTexCoord.y,fragTexCoord.z);
    else if(fragTexCoord.y< 0.0001 || fragTexCoord.y> 0.9999) fragTexCoord_corr=vec2(fragTexCoord.x,fragTexCoord.z);
    //get the texture colour at the specific pixel.
    kd = vec3(texture(brick_color,fragTexCoord_corr));
    //apply the bump map as specified in the project spec.
    vec4 b_xy=texture(brick_bump,fragTexCoord_corr);
    vec4 db_dx=texture(brick_bump,vec2(fragTexCoord_corr.x+1/512.0,fragTexCoord_corr.y))-b_xy;
    vec4 db_dy=texture(brick_bump,vec2(fragTexCoord_corr.x,fragTexCoord_corr.y+1/512.0))-b_xy;
    db_dx=db_dx*textureScale;
    db_dy=db_dy*textureScale;
    //adjust the normals as specified in the project spec.
    vec3 normal=normal_cam_space;
    normal.x=normal.x-db_dx.x;
    normal.y=normal.y-db_dy.y;
    normal=normalize(normal);
    //calculate the final output colour to be displayed.
    outColor = phongModel(normal);
//    outColor = vec4(diffuse.xyz,1);

}

vec4 phongModel(vec3 normal)
{
    float norm_light_angel_1 = clamp(dot(light_position_camspace1, normal), 0, 1);
    float norm_light_angel_2 = clamp(dot(light_position_camspace2, normal),0 ,1);

    vec3 r_1 = -reflect(light_position_camspace1, normal);
    vec3 r_2 = -reflect(light_position_camspace2, normal);
    float viewer_reflector_angle_1 = pow(max(clamp(dot(r_1, normalize(eyeDirection_camspace)), 0 ,1), 0.f), shineCoef);
    float viewer_reflector_angle_2 = pow(max(clamp(dot(r_2, normalize(eyeDirection_camspace)), 0, 1), 0.f), shineCoef);

    vec4 i;
    vec3 i_a = ka * (ambientColor);
    vec3 i_d = (kd * lightColor1 * norm_light_angel_1) + (kd * lightColor2 * norm_light_angel_2);
    vec3 i_s = (ks * lightColor1 * viewer_reflector_angle_1) + (ks * lightColor2 * viewer_reflector_angle_2);
    i = vec4(i_a + i_d + i_s, 1.f);
    return i;
}