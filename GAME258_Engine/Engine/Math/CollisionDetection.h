#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"
#include "../../Physics/Simplex.h"

//Structure forward declaration.
struct Ray;
struct BoundingBox;

using namespace glm;

//Fully static class. Cannot be created as an object.
class CollisionDetection
{
public:
	//Singleton. Prevent move or copy constructors.
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator=(const CollisionDetection&) = delete;
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	//Delete default constructor.
	CollisionDetection() = delete;
	~CollisionDetection();

	static Ray MousePosToWorldRay(vec2 mouseCoords_, float screenWidth_, float screenHeight_, Camera* camera_);
	static bool RayOBBIntersetion(Ray* ray_, BoundingBox* box_);

	static bool GJKDetection(GameObject* obj1, GameObject* obj2);

	//TODO Put this in a math library. It just checks if the two passed values reside on the same side of 0.
	template<typename valueType>
	static bool SameSign(valueType x, valueType y);
};

#endif // !COLLISIONDETECTION_H