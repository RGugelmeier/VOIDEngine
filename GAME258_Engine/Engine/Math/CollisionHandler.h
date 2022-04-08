#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Ray.h"
#include "../Rendering/3D/GameObject.h"
#include "../Core/OctSpatialPartition.h"

#include<glm/gtx/rotate_vector.hpp>

class CollisionHandler
{
public:
	//Next 4 lines prevent use of copy or move constructor. Ensures only one engine will exist at any time.
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void OnCreate(float worldSize_);
	void AddObject(GameObject* go_);
	void MouseUpdate(vec2 mousePosition_, int buttonType_);
	void CheckObjCollisions();
	void OnDestroy();
private:
	CollisionHandler();
	~CollisionHandler();

	static unique_ptr<CollisionHandler> collisionInstance;
	friend default_delete<CollisionHandler>;

	//List of collided with game objects by the last ray fired.
	static vector<GameObject*> prevCollision;
	vector<OctNode*> leafNodes;
	//Hold the spactial partition system.
	static OctSpatialPartition* scenePartition;
};

#endif // !COLLISIONHANDLER_H