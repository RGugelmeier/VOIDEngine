#include "GameObject.h"

//Set default values. Set model to be the model passed in. Check if the model passed in is not nullptr, and then create the instance.
GameObject::GameObject(Model* model_) : model(nullptr), position(vec3()), angle(0.0f), rotation(vec3(0.0f, 1.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0)
{
	model = model_;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
	}
}

//Do the same as above, but take and set custom position as well.
GameObject::GameObject(Model* model_, vec3 position_) : model(nullptr), position(vec3()), angle(0.0f), rotation(vec3(0.0f, 1.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0)
{
	model = model_;
	position = position_;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
	}
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

void GameObject::Update(const float deltaTime_)
{
	SetAngle(angle + 0.005f);
}

//Render the model by calling the model's render function.
void GameObject::Render(Camera* camera_)
{
	if (model)
	{
		model->Render(camera_);
	}
}

inline void GameObject::SetPosition(vec3 position_)
{
	position = position_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
	}
}

inline void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
	}
}

inline void GameObject::SetRotation(vec3 rotation_)
{
	rotation = rotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
	}
}

inline void GameObject::SetScale(vec3 scale_)
{
	scale - scale_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
	}
}
