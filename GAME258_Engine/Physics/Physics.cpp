#include "Physics.h"
#include "../Engine/Rendering/3D/GameObject.h"

Physics::Physics() : Component(), mass(1.0f), accel(vec3(0.0f,0.0f,0.0f)), vel(vec3(0.0f,0.0f,0.0f)), angle(0.0f), angularAcc(0.0f), angularVel(0.0f), rotationalInertia(1.0f)
{
	parent = nullptr;
}

Physics::Physics(float mass_, float rotationalInertia_) : Component(), accel(vec3(0.0f, 0.0f, 0.0f)), vel(vec3(0.0f, 0.0f, 0.0f)), angle(0.0f), angularAcc(0.0f), angularVel(0.0f), rotationalInertia(1.0f)
{
	mass = mass_;
	rotationalInertia = rotationalInertia_;
}

Physics::~Physics()
{
}

void Physics::Update(const float deltaTime)
{
	//Update velocity.
	vel += vec3(accel.x * deltaTime, accel.y * deltaTime, accel.z * deltaTime);
	//Update position.
	parent->position += vec3(vel.x * deltaTime + 0.5f * accel.x * deltaTime * deltaTime,
							 vel.y * deltaTime + 0.5f * accel.y * deltaTime * deltaTime,
							 vel.z * deltaTime + 0.5f * accel.z * deltaTime * deltaTime);

	//Update angle.
	angularVel += angularAcc * deltaTime;
	parent->angle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

	//Set the position of the parent GameObject to match the physics component.
	parent->SetPosition(parent->position);
	parent->SetRotation(parent->vRotation);
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
	//TODO change to vectors.
	accel.x = force.x / mass;
	accel.y = force.y / mass;
	accel.z = force.z / mass;
}

//Quaternion rotation.
void Physics::ApplyRotation(Quaternion orientation, vec3 angularVel_)
{
	Quaternion qAngularVel = Quaternion(1.0f, angularVel_);

	orientation = orientation.Add(qAngularVel.Multiply(orientation).Divide(2));

	orientation.Normalize();

	parent->SetRotation(vec3(parent->vRotation.x + orientation.v.x, parent->vRotation.y + orientation.v.y, parent->vRotation.z + orientation.v.z));
}

//Matrix rotation.
void Physics::ApplyTorque(vec3 torque)
{
	//angularAcc = torque / rotationalInertia;
}