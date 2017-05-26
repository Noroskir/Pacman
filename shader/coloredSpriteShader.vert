#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform vec3 gPos;


void main()
{
    gl_Position = vec4(inPosition.x + gPos.x, inPosition.y + gPos.y , inPosition.z, 1.0);
    texCoord  = vec2(inTexCoord.x, -inTexCoord.y);
}