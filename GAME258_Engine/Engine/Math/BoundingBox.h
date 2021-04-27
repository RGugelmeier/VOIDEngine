#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

//Right now, only one type of collision object exists. To create more, just make a new struct and give it the data it needs.

//Oriented bounding box.
struct BoundingBox {
	vec3 maxVert, minVert;
	mat4 transform;

	//Empty constructor. Make a box that has empty data.
	inline BoundingBox()
	{
		maxVert = minVert = vec3();
		transform = mat4();
	}

	//Manual constructor. Sets passed data in for the box.
	inline BoundingBox(vec3 maxVert_, vec3 minVert_, mat4 transform_)
	{
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	//This check to see if this box intersects with another box when both are transormed to be AABBs (Axis alligned bounding boxes).
	inline bool Intersects(BoundingBox* box_)
	{
		//Transforms the bounding box from OBB to AABB.
		vec3 minCorner = GetTransformedPoint(minVert, transform);
		vec3 maxCorner = GetTransformedPoint(maxVert, transform);

		//Transforms the other bounding box from OBB to AABB.
		vec3 otherMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
		vec3 otherMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);

		//Check for a collision (either in the positive side, negativeside , or inside) on each axis.
		bool xCollision = minCorner.x >= otherMinCorner.x && minCorner.x <= otherMaxCorner.x	//Check for positive side x collision.
			|| maxCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x				//Check for negative side x collision.
			|| minCorner.x >= otherMinCorner.x && maxCorner.x <= otherMaxCorner.x;				//Check for inside x collision.
		bool yCollision = minCorner.y >= otherMinCorner.y && minCorner.y <= otherMaxCorner.y	//Check for positive y side collision.
			|| maxCorner.y <= otherMaxCorner.y && maxCorner.y >= otherMinCorner.y 				//Check for negative y side collision.
			|| minCorner.y >= otherMinCorner.y && maxCorner.y <= otherMaxCorner.y;				//Check for inside y collision.
		bool zCollision = minCorner.z >= otherMinCorner.z && minCorner.z <= otherMaxCorner.z	//Check for positive z side collision.
			|| maxCorner.z <= otherMaxCorner.z && maxCorner.z >= otherMinCorner.z 				//Check for negative z side collision.
			|| minCorner.z >= otherMinCorner.z && maxCorner.z <= otherMaxCorner.z;				//Check for inside z collision.

		//If all three collision checks are true, there has been a collision. Reutrn true. Otherwise, false.
		return xCollision && yCollision && zCollision;
	}

private:
	//Converts an OBB (Oriented bounding box) to an AABB (Axis alligned bounding box)
	inline vec3 GetTransformedPoint(vec3 point_, mat4 transform_)
	{
		//Last collumn of a transformation matrix is the position.
		return vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_;
	}
};

#endif // !BOUNDINGBOX_H