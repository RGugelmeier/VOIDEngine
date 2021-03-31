#ifndef MESH_H
#define MESH_H

#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Camera/Camera.h"

using namespace glm;
using namespace std;

//A vertex is any point in space with these four basic properties for rendering.
struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 textureCoordinates;
	vec3 colour;
};

//A SubMesh is what holds the verticies and texture ID of the mesh. This can end up holding more traits depending on how complex the mesh is.
struct SubMesh
{
	vector<Vertex> vertexList;
	vector<unsigned int> meshIndicies;
	GLuint textureID;
};

class Mesh
{
public:
	//Takes in a vector that holds vertices. A vector is a dynamic container, allowing for any number of vertices to be stored.
	//Passes in the vector by reference rather than value because a vector can hold a large number of vertices, so passing by value could take a lot of resources.
	Mesh(SubMesh& subMesh_, GLuint shaderProgram_);
	~Mesh();

	void Render(Camera* camera_, vector<mat4>& instances_);

private:
	void GenerateBuffers();
	GLuint VAO, VBO;
	SubMesh subMesh;
	GLuint shaderProgram;
	GLuint modelLocation, viewLocation, projectionLocation, textureLocation;
	GLuint viewPos, lightPos, lightAmbientVal, lightSpecularVal, lightDiffuseVal, lightColour;
};

#endif // !MESH_H