#include "MaterialHandler.h"

//Redeclare and initialise static variables.
unique_ptr<MaterialHandler> MaterialHandler::materialInstance = nullptr;
vector<Material> MaterialHandler::materials = vector<Material>();

MaterialHandler::MaterialHandler()
{
	materials.reserve(10);  //Ask about reserve. Is it always right to use reserve even if you don't have any idea how big the vector will get?
							//Can a vector with reserved space become longer than the reserved space?
}

MaterialHandler::~MaterialHandler()
{
	OnDestroy();
}

//Gets the instance of the handler, or created the instance if it has not yet been made.
MaterialHandler* MaterialHandler::GetInstance()
{
	if (materialInstance.get() == nullptr)
	{
		materialInstance.reset(new MaterialHandler);
	}
	return materialInstance.get();
}

//Adds the provides material object to the materials list.
void MaterialHandler::AddMaterial(Material mat_)
{
	materials.push_back(mat_);
}

//Gets the provided material if it is in the list, otherwise, return a new material.
Material MaterialHandler::GetMaterial(const string& matName_) const
{
	for (auto mat : materials)
	{
		if (mat.name == matName_)
		{
			return mat;
		}
	}
	Debug::Warning("Could not find material with name" + matName_ + ". Check spelling on GetMaterial functions.", "MaterialHandler.cpp", __LINE__);
	return Material();
}

//Clean up if any materials exist.
void MaterialHandler::OnDestroy()
{
	if (materials.size() > 0)
	{
		materials.clear();
	}
}
