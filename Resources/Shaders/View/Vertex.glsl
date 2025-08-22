#version 330 core

layout (location = 0) in vec2 i_pos;
layout (location = 1) in vec2 i_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 p_tex_coord;

void main()
{
    p_tex_coord = vec2(i_tex_coord.x, i_tex_coord.y);
    gl_Position = u_proj * u_view * u_model * vec4(i_pos.x, i_pos.y, 0.0f, 1.0f);
}
