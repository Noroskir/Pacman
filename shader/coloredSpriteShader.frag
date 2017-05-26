#version 330

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D gTexture1;
uniform vec2 gTexShift;
uniform vec3 gColor;

void main()
{
    outColor = texture(gTexture1, vec2(texCoord.x + gTexShift.x, texCoord.y + gTexShift.y)) * vec4(gColor, 1.0f);
}
