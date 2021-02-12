#include "Model.h"

//Set default value for the mesh list.
Model::Model() : meshes(vector<Mesh*>())
{
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

//Call the render function for each mesh in the model.
void Model::Render()
{
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
