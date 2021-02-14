#include "Model.h"

//Set default value for the mesh list.
Model::Model(GLuint shaderProgram_) : meshes(vector<Mesh*>()), shaderProgram(0)
{
	shaderProgram = shaderProgram_;
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
void Model::Render()
{
	//glUseProgram tells OpenGL to switch what shader program to use. This is good in the case of a model for switching what shader to use for the model.
	//For example, a mirror model is going to use a more reflective shader than any other shader program.
	glUseProgram(shaderProgram);
	for (auto m : meshes)
	{
		m->Render();
	}
}

//Adds a mesh to the model.
void Model::AddMesh(Mesh* mesh_)
{
	meshes.push_back(mesh_);
}
