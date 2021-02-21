#include "Model.h"

//Set default value for the mesh list.
Model::Model(GLuint shaderProgram_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_) : meshes(vector<Mesh*>()), shaderProgram(0), position(vec3()), angle(0.0f), rotation(vec3(0.0f, 1.0f, 0.0f)), scale(vec3(1.0f))
{
	shaderProgram = shaderProgram_;
	position = position_;
	angle = angle_;
	rotation = rotation_;
	scale = scale_;
}

//Delete and deallocate space used for the meshes, then clear the meshes list.
Model::~Model()
{
	if (meshes.size() > 0)
	{
		for (auto m : meshes)
		{
			delete m;
			m = nullptr;
		}
		meshes.clear();
	}
}

//Set which shader program to use, then call the render function for each mesh in the model.
void Model::Render(Camera* camera_)
{
	//glUseProgram tells OpenGL to switch what shader program to use. This is good in the case of a model for switching what shader to use for the model.
	//For example, a mirror model is going to use a more reflective shader than any other shader program.
	glUseProgram(shaderProgram);
	for (auto m : meshes)
	{
		m->Render(GetTransform(), camera_);
	}
}

//Adds a mesh to the model.
void Model::AddMesh(Mesh* mesh_)
{
	meshes.push_back(mesh_);
}

//Gets and performs the transformation of the model.
mat4 Model::GetTransform() const
{
	mat4 model;
	model = translate(model, position);
	model = rotate(model, angle, rotation);
	model = glm::scale(model, scale);
	return model;
}
