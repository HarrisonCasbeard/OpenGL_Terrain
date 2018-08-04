#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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
	vec3 direction;
	float cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform Material material;

uniform DirectionalLight directionalLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

//-------------------------------------------------------------------------------------------------
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	//Diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	//Combine Results
	vec3 ambient = light.ambient * texture(material.diffuse, UV).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, UV).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, UV).rgb;

	return (ambient + diffuse + specular);
}

//-------------------------------------------------------------------------------------------------
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);

	//Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	//Attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//Results
	vec3 ambient = light.ambient * texture(material.diffuse, UV).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, UV).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, UV).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

//-------------------------------------------------------------------------------------------------
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
	vec3 lightDir = normalize(light.position - FragPos);
	float spotlightAngle = dot(lightDir, normalize(-light.direction));

	if(spotlightAngle > light.cutoff)
	{
		//Diffuse
		float diff = max(dot(normal, lightDir), 0.0f);

		//Specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

		//Attenuation
		float dist = length(light.position - fragPos);
		float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		//Results
		vec3 ambient = light.ambient * texture(material.diffuse, UV).rgb;
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, UV).rgb;
		vec3 specular = light.specular * spec * texture(material.specular, UV).rgb;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		return (ambient + diffuse + specular);
	}
	else
		return light.ambient * texture(material.diffuse, UV).rgb;
}

//-------------------------------------------------------------------------------------------------
void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcDirectionalLight(directionalLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += calcPointLight(pointLights[i], norm, FragPos, viewDir);

	//Spotlight will go here!
	result += calcSpotLight(spotLight, norm, FragPos, viewDir);

	colour = vec4(result, 1.0f);
}