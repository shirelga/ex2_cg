//#version 330
//
//layout(location = 0) in vec4 position;
//
//void main()
//{
//    gl_Position = position;
//}

// Extended by James Adams and Shiran Gazit.

#version 330

layout(location = 0) in vec4 position;
uniform vec2 window;

void main()
{
    vec4 tempPos = position;

    gl_Position = tempPos;
}