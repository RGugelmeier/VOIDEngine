#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

//TODO get multiple lights working.

struct Light
{
	vec3 position;
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
uniform Light light;

//Create a uniform to hold the material's properties.
uniform Material material;

//The fragment colour to render the vertex. This must always be in a fragment shader.
out vec4 fColour;

void main()
{
	//Calculate ambient value of light.
	vec3 ambient = light.ambientVal * material.ambient * texture(material.diffuseMap, TexCoords).rgb * light.lightColour;

	//Calculate diffuse value of light.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse) * texture(material.diffuseMap, TexCoords).rgb * light.lightColour;

	//Calculate specular value of light.
	vec3 viewDir = normalize(cameraPosition - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.lightColour;

	//Calculate the final light value on the fragment.
	vec3 result = ambient + diffuse + specular;

	//Set the fColour variable to the texture, using the result vec3 we just got and the material's transparency value.
	fColour = vec4(result, material.transparency);
}