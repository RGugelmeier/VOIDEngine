#include "GameObject.h"

//Use this constructor if the GameObject has no model.
GameObject::GameObject(vec3 position_) : model(NULL), position(vec3()), angle(0.0f), vRotation(vec3(0.0f, 0.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0), hit(false)
{
	position = position_;
}

//Set default values. Set model to be the model passed in. Check if the model passed in is not nullptr, and then create the instance and set it's bounding box values.
GameObject::GameObject(Model* model_, vec3 position_) : model(nullptr), position(vec3()), angle(0.0f), vRotation(vec3(0.0f, 0.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0), hit(false)
{
	model = model_;
	position = position_;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, vRotation, scale);
		boundingBox = model->GetBoundingBox();
		boundingBox.transform = model->GetTransform(modelInstance);
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
	}
}

//Deallocate the model's take space.
GameObject::~GameObject()
{
	model = nullptr;
}

void GameObject::Update(const float deltaTime_)
{
	for (auto component : components)
	{
		component->Update(deltaTime_);
		//if (dynamic_cast<Camera*>(component))
		//{
		//	component.
		//}
	}
}

//Render the model by calling the model's render function.
void GameObject::Render(Camera* camera_)
{
	if (model)
	{
		model->Render(camera_);
	}
}

vector<vec3> GameObject::GetVertices()
{
	vector<vec3> v;

	for (int i = 0; i < model->GetVertices().size(); i++)
	{
		v.push_back(model->GetVertices()[i] + position);
	}

	return v;
}

//Setters. Set the model's data as well as the bounding box's data, if there is a model.
void GameObject::SetPosition(vec3 position_)
{
	position = position_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetRotation(vec3 rotation_)
{
	vRotation = rotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetRotation(quat qRotation_)
{
	qRotation = qRotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, qRotation, scale);
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
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
		boundingBox.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		boundingBox.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
	}
}
//end setters.

//Set the object's tag.
void GameObject::SetTag(string tag_)
{
	name = tag_;
}

void GameObject::SetHit(bool hit_, int buttonType_)
{
	//Set if the object is hit.
	hit = hit_;
	//Print a message if the GameObject is hit.
	if (hit)
	{
		cout << name << " was hit.\n";
	}
}