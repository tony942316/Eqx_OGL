#version 330 core

out vec4 FragColor;

in vec4 p_color;

void main()
{
    FragColor = vec4(p_color.x, p_color.y, p_color.z, p_color.w);
}
