#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

//Variables from the vertex shader.
in vec3 Normal;
in vec3 TexCoords;
in vec3 Colour;

//The fragment colour to render the vertex. This must always be in a fragment shader.
out vec4 fColour;

void main()
{
	//Set the fColour variable to the Colour we took in, then convert to a vec4 with 1.0 as the w.
	fColour = vec4(Colour, 1.0);
}