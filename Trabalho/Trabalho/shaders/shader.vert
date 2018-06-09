#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

in vec3 vPosition;
in vec2 vTextureCoords;

layout (location = 0) out vec2 fTextureCoords;

void main()
{
    gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
    fTextureCoords = vTextureCoords;
}