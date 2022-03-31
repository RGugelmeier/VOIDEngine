#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

//Redeclare and define static variables.
unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
vector<GameObject*> CollisionHandler::prevCollision = vector<GameObject*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
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

//Clear any previous colliders that may be in the list to ensure proper functionality.
//Then create the spatial partitioning system with the passed in world size.
void CollisionHandler::OnCreate(float worldSize_)
{
	prevCollision.clear();
	scenePartition = new OctSpatialPartition(worldSize_, 3);
	leafNodes = scenePartition->GetLeafNodes();
}

//Add the passed game object into the scene partition's object list.
void CollisionHandler::AddObject(GameObject* go_)
{
	//Check to make sure the scene partition has been created.
	if (scenePartition != nullptr)
	{
		scenePartition->AddObject(go_);
	}
}

void CollisionHandler::MouseUpdate(vec2 mousePosition_, int buttonType_)
{
	//Create a ray at the mouse's click world position.
	Ray mouseRay = CollisionDetection::MousePosToWorldRay(mousePosition_, CoreEngine::GetInstance()->GetScreenWidth(), CoreEngine::GetInstance()->GetScreenHeight(), CoreEngine::GetInstance()->GetCamera());

	//Check to make sure the scene partition has been created.
	if (scenePartition != nullptr)
	{
		//Create a hit result variable that gets any collision from the mouseRay, from the scene partition.
		GameObject* hitResult = scenePartition->GetCollision(mouseRay);

		if (hitResult)
		{
			hitResult->SetHit(true, buttonType_);
		}

		//Reset all previous hit objects to be no-longer hit.
		for (auto c : prevCollision)
		{
			if (c != hitResult && c != nullptr)
			{
				c->SetHit(false, buttonType_);
			}
			c = nullptr;
		}
		prevCollision.clear();

		//If there has been a collision, add it to the previous collisions list.
		if (hitResult)
		{
			prevCollision.push_back(hitResult);
		}
	}
}

void CollisionHandler::CheckObjCollisions()
{
	//Used to store values from DynamicOBBOBBIntersects
	//vec3 contactPoint, contactNormal;
	//Ray testRay;
	//float contactTime = 0.0f;
	//Do OBBOBB check first, then do GJK check inside OBBOBB algorithm.
	//for (auto obj1 : SceneGraph::GetInstance()->sceneGameObjects)
	//{
	//	for (auto obj2 : SceneGraph::GetInstance()->sceneGameObjects)
	//	{
	//		if (obj1 != obj2)
	//		{
	//			if (CollisionDetection::DynamicOBBOBBIntersects(obj1.second, obj2.second, contactPoint, contactNormal, contactTime, testRay))
	//			{
	//				cout << obj1.second->GetTag() << " collision with " << obj2.second->GetTag() << endl;
	//
	//				obj1.second->GetComponent<Physics>()->SetVel(vec3(obj1.second->GetComponent<Physics>()->GetVel().x + (contactNormal.x * abs(obj1.second->GetComponent<Physics>()->GetVel().x)), obj1.second->GetComponent<Physics>()->GetVel().y, obj1.second->GetComponent<Physics>()->GetVel().z + (contactNormal.z * abs(obj1.second->GetComponent<Physics>()->GetVel().z))));
	//			}
	//		}
	//	}
	//}
	

	//Loop through each leaf node.
	for (auto leafNode : leafNodes)
	{
		//Loop through all collided objects in the current node.
		for (auto obj1InNode : leafNode->GetCollidedObjects())
		{
			//Loop through all objs again so we have a way to check each obj with another.
			for (auto obj2InNode : leafNode->GetCollidedObjects())
			{
				//If the two objects being checked are not the same, check for collision on them.
				if (obj1InNode != obj2InNode)
				{
					//Used to store values from DynamicOBBOBBIntersects
					vec3 contactPoint, contactNormal;
					Ray testRay;
					float contactTime = 0.0f;
					//Do OBBOBB check first, then do GJK check inside OBBOBB algorithm.
					if (obj1InNode->GetTag() != "Street1" && obj1InNode->GetTag() != "Street1")
					{
						if (CollisionDetection::DynamicOBBOBBIntersects(obj1InNode, obj2InNode, contactPoint, contactNormal, contactTime, testRay))
						{
							//if (CollisionDetection::GJKDetection(obj1InNode, obj2InNode))
							//{
								//cout << obj1InNode->GetTag() << " collision with " << obj2InNode->GetTag() << endl;
	
								obj1InNode->GetComponent<Physics>()->SetVel(vec3(obj1InNode->GetComponent<Physics>()->GetVel().x + (contactNormal.x * abs(obj1InNode->GetComponent<Physics>()->GetVel().x)), obj1InNode->GetComponent<Physics>()->GetVel().y, obj1InNode->GetComponent<Physics>()->GetVel().z + (contactNormal.z * abs(obj1InNode->GetComponent<Physics>()->GetVel().z))));
								//If the player is colliding with the something underneath it, do not do anything to the y velocity
								//if (obj1InNode->GetGroundCheck())
								//{
								//}
								////If there is no downward collision, apply y velocity as well as all the rest
								//else
								//{
								//	obj1InNode->GetComponent<Physics>()->SetVel(obj1InNode->GetComponent<Physics>()->GetVel() + (contactNormal * abs(obj1InNode->GetComponent<Physics>()->GetVel())));
								//}
							//}
						}
					}
				}
			}
		}
	}
}

//Clean up.
void CollisionHandler::OnDestroy()
{
	delete scenePartition;
	scenePartition = nullptr;

	for (auto entry : prevCollision)
	{
		entry = nullptr;
	}
	prevCollision.clear();
}