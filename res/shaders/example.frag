#version 330 core

out vec4 FragColor;
in vec4 color;

uniform float u_Color;

void main()
{
   FragColor = color;
};