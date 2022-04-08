#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

//TODO get different types opf lights working.

struct DirLight
{
	vec3 direction;
	float ambientVal;
	float diffuseVal;
	float specularVal;
	vec3 lightColour;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 lightColour;
	float ambientVal;
	float diffuseVal;
	float specularVal;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 lightColour;
	float ambientVal;
	float diffuseVal;
	float specularVal;
};

struct Material
{
	sampler2D diffuseMap;		//newmtl 

	float shininess;	//Ns
	float transparency;	//d

	vec3 ambient;    	//Ka
	vec3 diffuse;		//Kd
	vec3 specular;		//Ks
};

//Variables from the vertex shader.
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

//Create a position uniform to hold the camera's position. This is used when calculating specular light.
uniform vec3 cameraPosition;

//Create a uniform to hold the light's properties.
uniform DirLight dirLights[10];
uniform PointLight pointLights[10];
uniform SpotLight spotLights[10];

//Create a uniform to hold the material's properties.
uniform Material material;

//The fragment colour to render the vertex. This must always be in a fragment shader.
out vec4 fColour;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(cameraPosition - FragPosition);

	//Only 10 lights allowed in a scene. To change this, change the 10 ion the loop to something else.
	vec3 result;
	//for(int i = 0; i <= 10; i++)
	//{
	//}
	//for(int i = 0; i <= 0; i++)
	//{
	//
	//	result = CalcDirLight(dirLights[i], norm, viewDir);
	//}
	//for(int i = 0; i <= 0; i++)
	//{
	//
	//	result += CalcSpotLight(spotLights[0], norm, FragPosition, viewDir);
	//}

	for(int i = 0; i <= 0; i++)
	{
		//result += CalcPointLight(pointLights[0], norm, FragPosition, viewDir);
		result = CalcDirLight(dirLights[i], norm, viewDir);
	}
	//Add all point lights
	for(int i = 0; i <= 3; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPosition, viewDir);
	}
	//Add all spot lights
	for(int i = 0; i <= 0; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, FragPosition, viewDir);
	}

	//Calculate the final light value on the fragment.
	//vec3 result = ambient + diffuse + specular;

	//Set the fColour variable to the texture, using the result vec3 we just got and the material's transparency value.
	fColour = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Get the proper light direction. When passing in light direction, we give the direction from the mesh to the light. With direction lights we want the opposite.
	vec3 lightDir = normalize(-light.direction);

	//Calculate the diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Calculate specular. Take material shininess into consideration.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Get result
	vec3 ambient = light.ambientVal * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 diffuse = light.diffuseVal * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 specular = (spec * material.specular) * light.lightColour;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Get the proper light direction.
	vec3 lightDir = normalize(light.position - fragPos);

	//Calculate the diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Calculate the specular. Take material shininess into consideration.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Calculate the attenuation with. This determines what will be lit up and by how much depending on distance from the light source.
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Get result
	vec3 ambient = light.ambientVal * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 diffuse = light.diffuseVal * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 specular = (spec * material.specular) * light.lightColour;
	//Apply the attenuation to the light result
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	//Get the proper light direction.
	vec3 lightDir = normalize(light.position - fragPos);

	//Calculate the diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	//Calculate the specular. Take material shininess into consideration.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//Calculate the attenuation with. This determines what will be lit up and by how much depending on distance from the light source.
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//Calculate the light's intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	//Get result
	vec3 ambient = light.ambientVal * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 diffuse = light.diffuseVal * diff * vec3(texture(material.diffuseMap, TexCoords)) * light.lightColour;
	vec3 specular = (spec * material.specular) * light.lightColour;
	//Apply the attenuation and intensity to the light result
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}