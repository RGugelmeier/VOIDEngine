#ifndef MESH_H
#define MESH_H

#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Camera/Camera.h"
#include "../../Graphics/MaterialHandler.h"

using namespace glm;
using namespace std;

//A vertex is any point in space with these four basic properties for rendering.
struct Vertex
{
	vec3 position;
	vec3 normal;
	vec2 textureCoordinates;
};

//A SubMesh is what holds the verticies and texture ID of the mesh. This can end up holding more traits depending on how complex the mesh is.
struct SubMesh
{
	vector<Vertex> vertexList;
	vector<unsigned int> meshIndicies;
	Material material;
};

class Mesh
{
public:
	//Takes in a vector that holds vertices. A vector is a dynamic container, allowing for any number of vertices to be stored.
	//Passes in the vector by reference rather than value because a vector can hold a large number of vertices, so passing by value could take a lot of resources.
	Mesh(SubMesh& subMesh_, GLuint shaderProgram_);
	~Mesh();

	void Render(Camera* camera_, vector<mat4>& instances_);
	void SkyboxRender(Camera* camera_);
private:
	void GenerateBuffers();
	GLuint VAO, VBO;
	SubMesh subMesh;
	GLuint shaderProgram;
	//Model and camera uniform locations.
	GLuint modelLocation, viewLocation, projectionLocation;
	//Material uniform locations.
	GLuint diffuseMapLoc, shininessLoc, transparencyLoc, ambientLoc, diffuseLoc, specularLoc;
	//Light uniform locations.
	//GLuint viewPos, lightPos, lightAmbientVal, lightSpecularVal, lightDiffuseVal, lightColour;
	GLuint viewPos;
	//Point light values
	GLuint pointLightPos[10], pointLightAmbientVal[10], pointLightSpecularVal[10], pointLightDiffuseVal[10], pointLightColour[10], pointLightConstant[10], pointLightLinear[10], pointLightQuadratic[10];
	//Directional light values.
	GLuint dirLightDir[10], dirLightAmbientVal[10], dirLightSpecularVal[10], dirLightDiffuseVal[10], dirLightColour[10];
	//Spot light values
	GLuint spotLightPos[10], spotLightDir[10], spotLightCutOff[10], spotLightOuterCutOff[10], spotLightConstant[10], spotLightLinear[10], spotLightQuadratic[10], spotLightColour[10],
		spotLightAmbient[10], spotLightDiffuse[10], spotLightSpecular[10];

	vector<PointLight*> pointLights;
	vector<DirectionalLight*> directionalLights;
	vector<SpotLight*> spotLights;
};

#endif // !MESH_H