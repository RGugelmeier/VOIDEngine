#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

struct Light
{
	vec3 position;
	vec3 lightColour;
	float ambientVal;
	float diffuseVal;
	float specularVal;
};

//Variables from the vertex shader.
in vec3 Normal;
in vec2 TexCoords;
in vec3 Colour;
in vec3 FragPosition;

//Create the uniform for the texture as a "sampler2D" which is a 2D uniform type.
uniform sampler2D inputTexture;

//Create a position uniform to hold the camera's position. This is used when calculating specular light.
uniform vec3 cameraPosition;

//Create a uniform to hold the light's properties.
uniform Light light;

//The fragment colour to render the vertex. This must always be in a fragment shader.
out vec4 fColour;

void main()
{
	//Calculate ambient value of light.
	vec3 ambient = light.ambientVal * texture(inputTexture, TexCoords).rgb * light.lightColour;

	//Calculate diffuse value of light.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * light.diffuseVal) * texture(inputTexture, TexCoords).rgb * light.lightColour;

	//Calculate specular value of light.
	vec3 viewDir = normalize(cameraPosition - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (spec * light.specularVal) * light.lightColour;

	vec3 result = ambient + diffuse + specular;

	//Set the fColour variable to the texture, using the input texture sampler and the texture coordinates.
	fColour = vec4(result, 1.0f);
}