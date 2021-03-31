#include "Model.h"

//Set default value for the mesh list.
Model::Model(const string& objPath_, const string& matPath_, GLuint shaderProgram_) : meshes(vector<Mesh*>()), shaderProgram(0), modelInstances(vector<mat4>())
{
	shaderProgram = shaderProgram_;
	meshes.reserve(10);
	modelInstances.reserve(5);
	obj = new LoadOBJModel();
	obj->LoadModel(objPath_, matPath_);
	LoadModel();
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

	if (modelInstances.size() > 0)
	{
		modelInstances.clear();
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
		m->Render(camera_, modelInstances);
	}
}

//Adds a mesh to the model.
void Model::AddMesh(Mesh* mesh_)
{
	meshes.push_back(mesh_);
}

//Create instance then return the size of model instances with a 0 base so we can get index properly in other unctions.
unsigned int Model::CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_)
{
	modelInstances.push_back(CreateTransform(position_, angle_, rotation_, scale_));
	return modelInstances.size() - 1;
}

//Update the instance at index.
void Model::UpdateInstance(unsigned int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_)
{
	modelInstances[index_] = CreateTransform(position_, angle_, rotation_, scale_);
}

//Transform getter.
mat4 Model::GetTransform(unsigned int index_) const
{
	return modelInstances[index_];
}

//CReate the transform of the model based on given values.
mat4 Model::CreateTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_) const
{
	mat4 model;
	model = translate(model, position_);
	model = rotate(model, angle_, rotation_);
	model = scale(model, scale_);
	return model;
}

//Load the all obj file data into a list of sub meshes, then delete the obj loader to free up space.
void Model::LoadModel()
{
	for (int i = 0; i < obj->GetSubMeshes().size(); i++)
	{
		meshes.push_back(new Mesh(obj->GetSubMeshes()[i], shaderProgram));
	}
	delete obj;
	obj = nullptr;
}