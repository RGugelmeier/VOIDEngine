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
};

#endif // !BOUNDINGBOX_H