#version 440 core

uniform mat4 View;
uniform sampler2D Texture;

layout (location = 0) in vec3 fPositionEyeSpace;
layout (location = 1) in vec2 fTextureCoords;
layout (location = 2) in vec3 fNormalEyeSpace;

layout (location = 0) out vec4 fColor;

void main()
{
    fColor = texture(Texture, fTextureCoords);
}