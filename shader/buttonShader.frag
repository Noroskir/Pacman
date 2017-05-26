#version 330 core

out vec4 outColor;

uniform vec3 gColor;


void main()
{
    outColor = vec4(gColor, 1.0f);
}