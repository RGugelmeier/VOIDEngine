#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"

//Structure forward declaration.
struct Ray;
struct BoundingBox;

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
};

#endif // !COLLISIONDETECTION_H