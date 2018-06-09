#version 440 core

uniform mat4 ModelView;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

in vec3 vPosition;
in vec2 vTextureCoords;
in vec3 vNormal;

layout (location = 0) out vec3 fPositionEyeSpace;
layout (location = 1) out vec2 fTextureCoords;
layout (location = 2) out vec3 fNormalEyeSpace;

void main()
{
    gl_Position = Projection * ModelView * vec4(vPosition, 1.0);
    fTextureCoords = vTextureCoords;
    fNormalEyeSpace = normalize(NormalMatrix * vNormal);
}