#ifndef MODEL_H
#define MODEL_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "LoadOBJModel.h"

using namespace std;
using namespace glm;

class Model
{
public:
	//Set default values for a model. Rotation variable determines what axis to rotate on, x, y, or z.
	Model(const string& objPath_, const string& matPath_, GLuint shaderProgram_);
	~Model();

	void SkyboxRender(Camera* camera_);
	void Render(Camera* camera_);
	void AddMesh(Mesh* mesh_);
	unsigned int CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	void UpdateInstance(unsigned int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	void UpdateInstance(unsigned int index_, vec3 position_, quat rotation_, vec3 scale_);
	mat4 GetTransform(unsigned int index_) const;

	//Getters
	inline GLuint GetShaderProgram() const { return shaderProgram; }
	inline BoundingBox GetBoundingBox() const { return boundingBox; }
	vector<vec3> GetVertices() const;

	mat4 CreateTransform(vec3 Position_, float angle_, vec3 rotation_, vec3 scale_) const;
	mat4 CreateTransform(vec3 Position_, quat rotation_, vec3 scale_) const;
private:
	void LoadModel();


	vector<Mesh*> meshes;
	GLuint shaderProgram;
	vector<mat4> modelInstances;
	LoadOBJModel* obj;

	BoundingBox boundingBox;
};

#endif // !MODEL_H