#version 440 core

uniform mat4 View;
uniform sampler2D Texture;
uniform int ambientLightOn;
uniform int directionalLightOn;
uniform int pointLightOn;
uniform int spotLightOn;

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

// Estrutura de uma fonte de luz c�nica
 
struct SpotLight {
  vec3 position;    // Posi��o do foco de luz, espa�o do mundo
  vec3 ambient;    // Componente de luz ambiente
  vec3 diffuse;    // Componente de luz difusa
  vec3 specular;    // Componente de luz especular
 
  float constant;    // Coeficiente de atenua��o constante
  float linear;    // Coeficiente de atenua��o linear
  float quadratic;  // Coeficiente de atenua��o quadr�tica
 
  float spotCutoff, spotExponent;
  vec3 spotDirection; //direction
};
 
uniform SpotLight spotLight; // Fonte de luz conica
 
struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);

void main()
{
  //Componente emissiva do material.
  vec4 emissive = vec4(material.emissive, 1.0);
 
  vec4 light;
  // Iluminacao
  /*vec4 light[4];
  // Fonte de luz ambiente
  light[0] = calcAmbientLight(ambientLight);
  // Fonte de Luz Direcional
  light[1] = calcDirectionalLight(directionalLight);
  //Fonte de Luz Pontual
  light[2] = calcPointLight(pointLight);
  //Fonte de Luz Pontual
  light[3] = calcSpotLight(spotLight);*/
  light = calcSpotLight(spotLight);
  
  fColor = (emissive + light) * texture(Texture, fTextureCoords);
  //fColor = (emissive + light[0] + light[1] + light[2] + light[3]) * texture(Texture, fTextureCoords);

}

vec4 calcAmbientLight(AmbientLight light) 
{
  if (!ambientLightOn)
    return vec4(0.0);
  vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
  return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) 
{
  if (!directionalLightOn)
    return vec4(0.0);
  // Ambiente
  vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
 
  // Difusa
  vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
  vec3 L = normalize(-lightDirectionEyeSpace); // Dire��o inversa � da dire��o luz.
  vec3 N = normalize(fNormalEyeSpace);
  float NdotL = max(dot(N, L), 0.0);
  vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
 
  // Especular
  vec3 V = normalize(-fPositionEyeSpace);
  vec3 R = reflect(-L, N);
  float RdotV = max(dot(R, V), 0.0);
  vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

  return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light)
{
  if (!pointLightOn)
    return vec4(0.0);
  // Ambiente
  vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

  // Difusa
  vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
  vec3 L = normalize(lightPositionEyeSpace - fPositionEyeSpace);
  vec3 N = normalize(fNormalEyeSpace);
  float NdotL = max(dot(N, L), 0.0);
  vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

  // Especular
  vec3 V = normalize(-fPositionEyeSpace);
  vec3 R = reflect(-L, N);
  float RdotV = max(dot(R, V), 0.0);
  vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

  // Atenuacao
  float dist = length(mat3(View) * light.position - fPositionEyeSpace);
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  return (attenuation * (ambient + diffuse + specular)); 
}

/*vec4 calcSpotLight(SpotLight light) 
{ 
  //Ambient
  vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
 
  //Diffuse
  vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
  vec3 L = normalize(lightPositionEyeSpace - fPositionEyeSpace);
  vec3 N = normalize(fNormalEyeSpace);
  float NdotL = max(dot(N, L), 0.0);
  vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
 
  //Specular
  vec3 V = normalize(-fPositionEyeSpace);
  vec3 R = reflect(-L, N);
  float RdotV = max(dot(R, V), 0.0);
  vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
 
  //SpotLight
  float theta = dot(L, normalize(-light.spotDirection));
  float epsilon =  (light.spotCutoff - light.spotExponent);
  float intensity  = clamp((theta - light.spotCutoff) / epsilon, 0.0, 1.0);
  diffuse *=intensity;
  specular *= intensity;
 
  // attenuation
  float dist = length(mat3(View) * light.position - fPositionEyeSpace);  // C�lculo da dist�ncia entre o ponto de luz e o v�rtice
  float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
 
  return (attenuation * (ambient + diffuse + specular));
}*/

vec4 calcSpotLight(SpotLight light){
  if (!spotLightOn)
    return vec4(0.0);
  //Ambient
  vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

  float intensity = 0.0;
  vec4 spec = vec4(0.0);
 
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    vec3 ld = normalize(lightPositionEyeSpace - fPositionEyeSpace);
    vec3 sd = normalize(vec3(-light.spotDirection));  

    vec3 n = normalize(fNormalEyeSpace);
    intensity = max(dot(n,ld), 0.0);
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * intensity;
    
    // inside the cone?
    if (dot(sd,ld) > light.spotCutoff) {
        if (intensity > 0.0) {
            vec3 eye = normalize(fPositionEyeSpace);
            vec3 h = normalize(ld + eye);
            float intSpec = max(dot(h,n), 0.0);
            vec4 spec = vec4(light.specular * material.specular, 1.0) * pow(intSpec, material.shininess);
        }
    }
 
    return max(intensity * diffuse + spec, ambient);
}