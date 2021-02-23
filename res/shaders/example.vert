#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 in_col;

out vec4 color;

void main()
{
   gl_Position = aPos;
   color = in_col;
};