#include "GameObject.h"

//Set default values. Set model to be the model passed in. Check if the model passed in is not nullptr, and then create the instance and set it's bounding box values.
GameObject::GameObject(Model* model_, vec3 position_) : model(nullptr), position(vec3()), angle(0.0f), rotation(vec3(0.0f, 1.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0)
{
	model = model_;
	position = position_;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
		boundingBox = model->GetBoundingBox();
		boundingBox.transform = model->GetTransform(modelInstance);

		cout << "Min: " << to_string(boundingBox.minVert) << ", Max: " << to_string(boundingBox.maxVert) << endl;
	}
}

//Deallocate the model's take space.
GameObject::~GameObject()
{
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

//Setters. Set the model's data as well as the bounding box's data.
void GameObject::SetPosition(vec3 position_)
{
	position = position_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetRotation(vec3 rotation_)
{
	rotation = rotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

//After updating the transformation matrix, change the min and max verticies of the box so the size is accurate.
//If scaling up (scale > 1) set to scale, if scaling down (scale < 1) divide the scale by 2 and set that.
void GameObject::SetScale(vec3 scale_)
{
	scale - scale_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
		boundingBox.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		boundingBox.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
	}
}
//end setters.

void GameObject::SetTag(string tag_)
{
	tag = tag_;
}
