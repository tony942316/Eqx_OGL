#version 330 core

out vec4 FragColor;

in vec2 p_tex_coord;

uniform sampler2D u_tex0;

void main()
{
    FragColor = texture(u_tex0, p_tex_coord);
}
