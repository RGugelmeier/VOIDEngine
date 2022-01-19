#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

out vec4 fragColour;

in vec3 textureCoords;

uniform samplerCube skyboxTexture;

void main()
{
	vec3 textureCoordsInvertX = textureCoords;
	textureCoordsInvertX *= -1.0;
	fragColour = texture(skyboxTexture, textureCoords);
}