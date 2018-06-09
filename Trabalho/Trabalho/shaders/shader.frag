#version 440 core

uniform mat4 View;
uniform sampler2D Texture;

layout (location = 0) in vec3 fPositionEyeSpace;
layout (location = 1) in vec2 fTextureCoords;
layout (location = 2) in vec3 fNormalEyeSpace;

layout (location = 0) out vec4 fColor;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

vec4 calcAmbientLight(AmbientLight light);

void main()
{
	//Componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// C�lculo do efeito da ilumina��o no fragmento.
	vec4 light;
	// fonte de luz ambiente
	light = calcAmbientLight(ambientLight);

    fColor = texture(Texture, fTextureCoords);
}

vec4 calcAmbientLight(AmbientLight light) {
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}