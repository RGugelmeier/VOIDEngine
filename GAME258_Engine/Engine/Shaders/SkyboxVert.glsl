#version 450 core //Match the glsl version to the OpenGl version. Ex. OpenGl 4.5 = glsl version 450 and set to core profile.

layout (location = 0) in vec4 vVertex;

out vec3 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 cameraRotationMatrix;

void main()
{
	textureCoords = vVertex.xyz;

	gl_Position = projectionMatrix * cameraRotationMatrix * vVertex;
}