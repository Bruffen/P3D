#version 440 core

struct AmbientLight 
{
    vec3 ambient;
};

struct DirectionalLight
{
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}:

struct PointLight 
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float spotCutOff, spotExponent;
    vec3 spotDirection;
};

struct Material {
    vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

vec4 calcAmbientLight(AmbientLight light)
{
    vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
    return ambient;
}

vec4 calcDirectionalLight()
{
    vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	vec3 V = normalize(-vPositionEyeSpace);

	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) 
{
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	vec3 V = normalize(-vPositionEyeSpace);

	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);

	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	// C�lculo da dist�ncia entre o ponto de luz e o v�rtice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light)
{
	//Ambient
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	//Diffuse
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	//Specular
	vec3 V = normalize(-vPositionEyeSpace);
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
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	// C�lculo da dist�ncia entre o ponto de luz e o v�rtice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (ambient + diffuse + specular));
}