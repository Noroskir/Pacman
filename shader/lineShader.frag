#version 330

out vec4 outColor;

uniform vec3 gColor;

void main()
{
    outColor = vec4(gColor, 1);
}