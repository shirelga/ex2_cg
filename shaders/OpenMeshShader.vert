
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
uniform mat4 MVP;
uniform vec2 window;

out vec4 in_Color;
void main()
{
	in_Color = color;
    gl_Position = MVP * position;
}