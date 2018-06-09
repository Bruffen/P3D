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

// Estrutura de uma fonte de luz direcional
struct DirectionalLight  {
  vec3 direction;    // Direcao da luz
  
  vec3 ambient;    // Componente de luz ambiente
  vec3 diffuse;    // Componente de luz difusa
  vec3 specular;    // Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional global

// Estrutura de uma fonte de luz pontual
struct PointLight  {
  vec3 position;    // Posicao do ponto de luz, espaco do mundo
  
  vec3 ambient;    // Componente de luz ambiente
  vec3 diffuse;    // Componente de luz difusa
  vec3 specular;    // Componente de luz especular
  
  float constant;    // Coeficiente de atenuacao constante
  float linear;    // Coeficiente de atenuacao linear
  float quadratic;  // Coeficiente de atenuacao quadratica
};

uniform PointLight pointLight; // Fonte de luz pontual global

// Estrutura de uma fonte de luz cónica
struct SpotLight {
  vec3 position;    // Posição do foco de luz, espaço do mundo
