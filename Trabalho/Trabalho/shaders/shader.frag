#version 440 core

uniform sampler2D Texture;

layout (location = 0) in vec2 fTextureCoords;

layout (location = 0) out vec4 fColor;

void main()
{
    fColor = texture(Texture, fTextureCoords);
}