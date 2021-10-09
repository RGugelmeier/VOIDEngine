#ifndef QUATERNION_H
#define QUATERNION_H

#include <glm/glm.hpp>

using namespace glm;

class Quaternion
{
public:
	Quaternion();
	Quaternion(float w_, vec3 v_);
	Quaternion(float w_, float x_, float y_, float z_);
	~Quaternion();

	Quaternion Add(Quaternion otherQuaternion_);
	Quaternion Subtract(Quaternion otherQuaternion_);
	Quaternion Multiply(Quaternion otherQuaternion_);
	Quaternion Divide(float dividingNumber);
	float Dot(Quaternion otherQuaternion_);
	Quaternion Mag(Quaternion otherQuaternion_);
	Quaternion Conjugate(Quaternion otherQuaternion_);
	Quaternion Normalize();
	float w;
	vec3 v;
private:
};

#endif