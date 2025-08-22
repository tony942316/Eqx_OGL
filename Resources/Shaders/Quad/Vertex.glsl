#version 330 core

layout (location = 0) in vec2 i_pos;
layout (location = 1) in vec4 i_color;

out vec4 p_color;

void main()
{
    p_color = vec4(i_color.x, i_color.y, i_color.z, i_color.w);
    gl_Position = vec4(i_pos.x, i_pos.y, 0.0f, 1.0f);
}
