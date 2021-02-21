#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

//Taken from the VAO and VBO. Locations and variable types match what we set up in the Vertex struct. See Mesh.cpp GenerateBuffers() function.
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 colour;

//Variables to send to the fragment shader.
out vec3 Normal;
out vec2 TexCoords;
out vec3 Colour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position is specific to vertex shaders. Takes the position that is passed in and tells OpenGL where the vertex should be.
	//gl_Position is a vec4 so we need to convert the position to a vec4. Set the w to 1.0f.
	//We multiply the in position variable by the matricis to make sure the transformation applied to shapes are shown and continuously rendered properly.
	gl_Position = projection * view * model * vec4(position, 1.0f);

	//Set the out Colour to equal the in colour for the fragment shader.
	Colour = colour;
}