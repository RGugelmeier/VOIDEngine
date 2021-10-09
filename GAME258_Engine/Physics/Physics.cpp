#include "Physics.h"
#include "../Engine/Rendering/3D/GameObject.h"

Physics::Physics() : Component(), mass(1.0f), accel(vec3(0.0f,0.0f,0.0f)), vel(vec3(0.0f,0.0f,0.0f))
{
	parent = nullptr;
}

Physics::~Physics()
{
}

void Physics::Update(const float deltaTime)
{
	//Update position.
	parent->position.x += vel.x * deltaTime + 0.5f * accel.x * deltaTime * deltaTime;
	vel.x += accel.x * deltaTime;
	parent->position.y += vel.y * deltaTime + 0.5f * accel.y * deltaTime * deltaTime;
	vel.y += accel.y * deltaTime;
	parent->position.z += vel.z * deltaTime + 0.5f * accel.z * deltaTime * deltaTime;
	vel.z += accel.z * deltaTime;

	//Update angle.
	//angularVel += angularAcc * deltaTime;
	//angle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

	parent->SetPosition(parent->position);
	//parent->SetRotation(parent->rotation + angle);
}

void Physics::OnCreate(GameObject* parent_)
{
	parent = parent_;
}

void Physics::OnDestroy()
{
}

void Physics::Render() const
{
}

//This function applies gravity by the force of gravity on earth.
void Physics::ApplyGravity()
{
}

//This function applies gravity by the force of gravity given by the forceOfGravity argument.
void Physics::ApplyGravity(vec3 forceOfGravity)
{
}

void Physics::ApplyForce(vec3 force)
{
	accel.x = force.x / mass;
	accel.y = force.y / mass;
	accel.z = force.z / mass;
}

void Physics::ApplyRotation(vec3 axisOfRotation, Quaternion orientation, vec3 angularVel)
{
	Quaternion qAngularVel = Quaternion(0.0f, angularVel);

	orientation = orientation.Add(qAngularVel.Multiply(orientation).Divide(2));

	parent->SetRotation(vec3(parent->rotation.x + orientation.v.x, parent->rotation.y + orientation.v.y, parent->rotation.z + orientation.v.z));
}