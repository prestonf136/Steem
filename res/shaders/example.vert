#version 330 core
layout (location = 0) in vec4 position; 
layout (location = 1) in vec4 recv_col; 

out vec4 out_color;

void main()
{
    gl_Position = vec4(position);
    out_color = recv_col;
}