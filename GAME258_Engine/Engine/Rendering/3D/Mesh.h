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

class Mesh
{
public:
	//Takes in a vector that holds vertices. A vector is a dynamic container, allowing for any number of vertices to be stored.
	//Passes in the vector by reference rather than value because a vector can hold a large number of vertices, so passing by value could take a lot of resources.
	Mesh(vector<Vertex>& vertexList_, GLuint shaderProgram_);
	~Mesh();

	void Render(mat4 transform_, Camera* camera_);

private:
	void GenerateBuffers();
	GLuint VAO, VBO;
	vector<Vertex> vertexList;
	GLuint shaderProgram;
	GLuint modelLocation, viewLocation, projectionLocation;
};

#endif // !MESH_H