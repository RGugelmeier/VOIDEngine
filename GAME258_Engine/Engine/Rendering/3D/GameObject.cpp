#include "GameObject.h"
#include "../../Core/OctSpatialPartition.h"
#include "../../Math/CollisionHandler.h"
#include "../Skybox.h"

//Use this constructor if the GameObject has no model.
GameObject::GameObject(vec3 position_) : model(NULL), position(vec3()), angle(0.0f), vRotation(vec3(0.0f, 0.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0), hit(false), setNewOctNodes(false),
										 forward(vec3()), right(vec3()), up(vec3()), worldUp(vec3()), collidedNodes(list<OctNode*>()), moveable(false), groundCheck(false)
{
	position = position_;
	forward = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	collisionHandlerInstance = CollisionHandler::GetInstance();

	if (components.size() > 0)
	{
		for (auto component : components)
		{
			component->OnCreate(this);
		}
	}
}

//Set default values. Set model to be the model passed in. Check if the model passed in is not nullptr, and then create the instance and set it's bounding box values.
GameObject::GameObject(Model* model_, vec3 position_, bool moveable_) : model(nullptr), position(vec3()), angle(0.0f), vRotation(vec3(0.0f, 0.0f, 0.0f)), scale(vec3(1.0f)), modelInstance(0), hit(false), setNewOctNodes(false),
														forward(vec3()), right(vec3()), up(vec3()), worldUp(vec3()), collidedNodes(list<OctNode*>()), moveable(false), groundCheck(false)
{
	model = model_;
	position = position_;
	moveable = moveable_;
	forward = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	collisionHandlerInstance = CollisionHandler::GetInstance();
	modelVertices = FillVertices();

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

void GameObject::OnCreate()
{
}

void GameObject::Update(const float deltaTime_)
{
	for (auto component : components)
	{
		component->Update(deltaTime_);
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

	for (int i = 0; i < modelVertices.size(); i++)
	{
		v.push_back(modelVertices[i] + position);
	}

	return v;
}

vector<vec3> GameObject::FillVertices()
{
	vector<vec3> v;

	for (int i = 0; i < model->GetVertices().size(); i++)
	{
		v.push_back(model->GetVertices()[i]);
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

	collidedNodesIterator = collidedNodes.begin();

	//Loop through each OctNode that this obj is in...
	while (collidedNodesIterator != collidedNodes.end())
	{
		//If the obj is no longer collided with the node being checked...
		if (!boundingBox.Intersects((*collidedNodesIterator)->GetBoundingBox()))
		{
			//...remove it from the list.
			(*collidedNodesIterator)->RemoveCollisionObject(this);

			//And remove it from the list here as well.
			collidedNodesIterator = collidedNodes.erase(collidedNodesIterator);

			setNewOctNodes = true;
		}
		else
		{
			collidedNodesIterator++;
		}
	}

	if (setNewOctNodes)
	{
		collisionHandlerInstance->AddObject(this);
		setNewOctNodes = false;
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
	scale = scale_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, vRotation, scale);
		boundingBox.transform = model->GetTransform(modelInstance);
		boundingBox.minVert.x *= scale.x > 1.0f ? scale.x : (scale.x / 2.0f);
		boundingBox.maxVert.x *= scale.x > 1.0f ? scale.x : (scale.x / 2.0f);
		
		boundingBox.minVert.y *= scale.y > 1.0f ? scale.y : (scale.y / 2.0f);
		boundingBox.maxVert.y *= scale.y > 1.0f ? scale.y : (scale.y / 2.0f);
		
		boundingBox.minVert.z *= scale.z > 1.0f ? scale.z : (scale.z / 2.0f);
		boundingBox.maxVert.z *= scale.z > 1.0f ? scale.z : (scale.z / 2.0f);
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

//Sets the variable that checks if the object is colliding with something underneath it. Used to check if gravity should be applied.
void GameObject::SetGroundCheck(bool groundCheck_)
{
	groundCheck = groundCheck_;
}

//Update the up, right and forward vectors.
//Also updates yaw and pitch to be used to update the vectors.
void GameObject::UpdateVectors(float yaw, float pitch)
{
	//Set the forward vector.
	forward.x = cos(radians(yaw)) * cos(radians(pitch));
	forward.y = sin(radians(pitch));
	forward.z = sin(radians(yaw)) * cos(radians(pitch));

	//Normalize forward vector then set the up and right vectors by getting the normal of the cross of the two other vectors.
	forward = normalize(forward);
	right = normalize(cross(forward, worldUp));
	up = normalize(cross(right, forward));
}


