#version 330 core

in vec2 texCoord;

out vec4 outColor;

uniform vec3 gColor;
uniform sampler2D gText;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(gText, texCoord).r);
    outColor = vec4( gColor, 1) * sampled;
}