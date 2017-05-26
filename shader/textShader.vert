#version 330 core
layout (location = 0) in vec4 inVertex; // <vec2 pos, vec2 tex>

out vec2 texCoord;

void main()
{
    gl_Position = vec4(inVertex.x, inVertex.y, 0.0, 1.0);
    texCoord = inVertex.zw;
}