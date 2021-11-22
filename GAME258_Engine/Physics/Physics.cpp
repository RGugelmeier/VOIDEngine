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
	//Update position.
	//TODO change this to vectors.
	parent->position.x += vel.x * deltaTime + 0.5f * accel.x * deltaTime * deltaTime;
	vel.x += accel.x * deltaTime;
	parent->position.y += vel.y * deltaTime + 0.5f * accel.y * deltaTime * deltaTime;
	vel.y += accel.y * deltaTime;
	parent->position.z += vel.z * deltaTime + 0.5f * accel.z * deltaTime * deltaTime;
	vel.z += accel.z * deltaTime;

	//Update angle.
	angularVel += angularAcc * deltaTime;
	parent->angle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

	parent->SetPosition(parent->position);
	parent->SetRotation(parent->vRotation);
	//cout << parent->GetTag() << ": " << parent->vRotation.x << ", " << parent->vRotation.y << ", " << parent->vRotation.z << "\n\n\n";
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