#include "GameObject.h"

//Set default values. Get rid of any data that has previously been put in, then set model to the model that is passed in.
GameObject::GameObject(Model* model_) : model(nullptr)
{
	model = model_;
}

//Delete the model and deallocate the space if there is one.
GameObject::~GameObject()
{
	if (model)
	{
		delete model;
	}
	model = nullptr;
}

//Render the model by calling the model's render function.
void GameObject::Render(Camera* camera_)
{
	if (model)
	{
		model->Render(camera_);
	}
}
