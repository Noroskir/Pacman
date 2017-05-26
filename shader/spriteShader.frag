#version 330

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D gTexture1;
uniform vec2 gTexShift;

void main()
{
    outColor = texture(gTexture1, vec2(texCoord.x + gTexShift.x, texCoord.y + gTexShift.y));
}
