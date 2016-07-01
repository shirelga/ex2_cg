#version 330

//uniform vec4 fillColor;
//in vec4 in_Color;
in vec4 outIntensity;

out vec4 outColor;
void main()
{
    outColor = outIntensity;
}

