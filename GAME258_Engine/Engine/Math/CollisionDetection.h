#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"
#include "../../Physics/Simplex.h"

#include <glm/gtx/rotate_vector.hpp>

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

	static bool RayOBBIntersetion(Ray* ray_, BoundingBox* box_, vec3& pointOfContact, vec3& contactNormal);
	static bool RayOBBIntersetion(Ray ray_, BoundingBox box_, vec3& pointOfContact, vec3& contactNormal);
	static bool DynamicOBBOBBIntersects(GameObject* dynamicObj, GameObject* staticObj, vec3& contactPoint, vec3& contactNormal, float& intersetionDist_, Ray& testRay);
	static bool OBBOBBIntersection(BoundingBox box1_, BoundingBox box2_);

	static bool GJKDetection(GameObject* obj1, GameObject* obj2);

	inline static bool SameSign(float a, float b)
	{
		return a * b >= 0.0f;
	}
};

#endif // !COLLISIONDETECTION_H