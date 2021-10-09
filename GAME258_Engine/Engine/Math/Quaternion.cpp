#include "Quaternion.h"

//Null quaternion.
Quaternion::Quaternion() : w(0), v(vec3())
{
}

//Quaternion with passed in scalar (w_) and vec3 (v_)
Quaternion::Quaternion(float w_, vec3 v_) : w(0.0f), v(vec3())
{
	w = w_;
	v = v_;
}

Quaternion::Quaternion(float w_, float x_, float y_, float z_) : w(0.0f), v(vec3())
{
	w = w_;
	v = vec3(x_, y_, z_);
}

Quaternion::~Quaternion()
{
}

//Adds two quaternions together.
Quaternion Quaternion::Add(Quaternion otherQuaternion_)
{
	return Quaternion(w + otherQuaternion_.w, v + otherQuaternion_.v);
}

//Subtract two quaternions.
Quaternion Quaternion::Subtract(Quaternion otherQuaternion_)
{
	return Quaternion(w - otherQuaternion_.w, v - otherQuaternion_.v);
}

//Multiply two quaternions.
Quaternion Quaternion::Multiply(Quaternion otherQuaternion_)
{
	return Quaternion(w * otherQuaternion_.w - v.x * otherQuaternion_.v.x - v.y * otherQuaternion_.v.y - v.z * otherQuaternion_.v.z,
						w * otherQuaternion_.v.x + v.x * otherQuaternion_.w + v.y * otherQuaternion_.v.z - v.z * otherQuaternion_.v.y,
						w * otherQuaternion_.v.y + v.y * otherQuaternion_.w + v.z * otherQuaternion_.v.x - v.x * otherQuaternion_.v.z,
						w * otherQuaternion_.v.z + v.z * otherQuaternion_.w + v.x * otherQuaternion_.v.y - v.y * otherQuaternion_.v.x);
}

Quaternion Quaternion::Divide(float dividingNumber)
{
	//q.w / s, q.x / s, q.y / s, q.z / s
	return Quaternion(w / dividingNumber, v.x / dividingNumber, v.y / dividingNumber, v.z / dividingNumber);
}

//Get the dot product of two quaternions.
float Quaternion::Dot(Quaternion otherQuaternion_)
{
	return w * otherQuaternion_.w + v.x * otherQuaternion_.v.x + v.y * otherQuaternion_.v.y + v.z * otherQuaternion_.v.z;
}

//Get the quaternion's magnitude.
Quaternion Quaternion::Mag(Quaternion otherQuaternion_)
{
	return Quaternion(sqrt(pow(w, 2)), sqrt(pow(v.x, 2)), sqrt(pow(v.y, 2)), sqrt(pow(v.z, 2)));
}

//Get the quaternion's conjugate.
Quaternion Quaternion::Conjugate(Quaternion otherQuaternion_)
{
	return Quaternion(w, -v);
}

//Normalizes the quaternion.
Quaternion Quaternion::Normalize()
{
	return Quaternion(w / sqrt(pow(w, 2)), v.x / sqrt(pow(v.x, 2)), v.y / sqrt(pow(v.y, 2)), v.z / sqrt(pow(v.z, 2)));
}
