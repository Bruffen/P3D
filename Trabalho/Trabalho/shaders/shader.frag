#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout (location = 0) out vec4 fColor;

void main()
{
    fColor = vec4(1.0, 1.0, 1.0, 1.0)
}