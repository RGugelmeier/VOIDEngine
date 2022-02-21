#ifndef RAY_H
#define RAY_H

#include "BoundingBox.h"
#include "CollisionDetection.h"

struct Ray {
	vec3 origin;
	vec3 direction;
	vec3 intersectionPoint, intersectionNormal;
	float intersectionDist;

	//Constructor. Set default values to empty or 0.
	inline Ray() 
	{
		origin = vec3();
		direction = vec3();
		intersectionDist = 0.0f;
	}

	//Constructor. Set values to values passed in.
	inline Ray(vec3 origin_, vec3 direction_, float intersectionDist_)
	{
		origin = origin_;
		direction = direction_;
		intersectionDist = intersectionDist_;
	}

	//Overload = to be able to set the values of one ray to this one.
	inline Ray& operator =(const Ray& ray_)
	{
		origin = ray_.origin;
		direction = ray_.direction;
		intersectionDist = 0.0f;

		return *this;
	}

	//Check if this ray collides with the provided bounding box.
	inline bool IsColliding(BoundingBox* box_)
	{
		intersectionDist = -1.0f;
		return CollisionDetection::RayOBBIntersetion(this, box_, intersectionPoint, intersectionNormal);
	}
};

#endif // !RAY_H
