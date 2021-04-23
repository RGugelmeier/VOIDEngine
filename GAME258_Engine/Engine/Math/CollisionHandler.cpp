#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

//Redeclare and define static variables.
unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
vector<GameObject*> CollisionHandler::prevCollision = vector<GameObject*>();
vector<GameObject*> CollisionHandler::colliders = vector<GameObject*>();

CollisionHandler::CollisionHandler()
{
	colliders.reserve(10);
	prevCollision.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}

//In there is no instance of a collision handler, create one. Then return the collision handler instance.
CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr)
	{
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnCreate()
{
	colliders.clear();
	prevCollision.clear();
}

//Add the passed game objecxt into the colliders list.
void CollisionHandler::AddObject(GameObject* go_)
{
	colliders.push_back(go_);
}

void CollisionHandler::MouseUpdate(vec2 mousePosition_, int buttonType_)
{
	//Create a ray at the mouse's click world position.
	Ray mouseRay = CollisionDetection::MousePosToWorldRay(mousePosition_, CoreEngine::GetInstance()->GetScreenWidth(), CoreEngine::GetInstance()->GetScreenHeight(), CoreEngine::GetInstance()->GetCamera());

	//Create a hit result variable to use later.
	GameObject* hitResult = nullptr;
	float shortestDistance = FLT_MAX;

	//Check if the ray has collided with any of the game objects in the scene.
	for (auto g : colliders)
	{
		BoundingBox tmp = g->GetBoundingBox();
		if (mouseRay.IsColliding(&tmp))
		{
			//Make sure the closest object to the camera that has been hit by the ray is returned as the shortest distance.
			if (mouseRay.intersectionDist < shortestDistance)
			{
				hitResult = g;
				shortestDistance = mouseRay.intersectionDist;
			}
		}
	}

	//If there has been a collision, set hit to true and send what button was pressed to to so.
	if (hitResult)
	{
		hitResult->SetHit(true, buttonType_);
	}

	//Reset all previous hit objects to be no-longer hit.
	for (auto c : prevCollision)
	{
		if (hitResult != c && c != nullptr)
		{
			c->SetHit(false, buttonType_);
		}
	}
	prevCollision.clear();

	//Add the newly hit object to previous collision list.
	if (hitResult)
	{
		prevCollision.push_back(hitResult);
	}
}

//Clean up the lists.
void CollisionHandler::OnDestroy()
{
	for (auto entry : colliders)
	{
		entry = nullptr;
	}
	colliders.clear();

	for (auto entry : prevCollision)
	{
		entry = nullptr;
	}
	prevCollision.clear();
}


