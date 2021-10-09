#include "AI.h"

AI::AI() : result(nullptr), arriveDistance(0.0f), targetSpeed(0.0f)
{
	result = new SteeringOutput();
}

AI::~AI()
{
	delete result;
	result = nullptr;
}

void AI::OnCreate(GameObject* parent_)
{
	parent = parent_;
}

void AI::OnDestroy()
{
}

void AI::Update(const float deltaTime_)
{
}

void AI::Render() const
{
}

//Call to get a character with AI to seek out a target.
void AI::Seek(vec3 target, float maxAccel)
{
	//Get the direction from the NPC to the target.
	result->linear = target - parent->position;

	//Normalize the direction to make it a direction vector.
	result->linear = normalize(result->linear);
	//Apply force to the NPC.
	parent->GetComponent<Physics>()->ApplyForce(result->linear);

	result->angular = 0;
}

//Call to get the character with AI to approach and arrive at a target.
void AI::Arrive(vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget)
{
	//Get the direction from the NPC to the target.
	result->linear = target - parent->position;

	//Get how far away the NPC is from the target.
	arriveDistance = glm::length(result->linear);

	//Check if we are inside the arrive radius and stop if we are...
	if (arriveDistance < targetRadius)
	{
		parent->GetComponent<Physics>()->SetVel(vec3(0.0f, 0.0f, 0.0f));
		return;
	}

	//...if we are, check if we are not in the slow radius and use max speed if we are not...
	if (arriveDistance > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	//...if we are, use slowed down speed.
	else
	{
		targetSpeed = maxSpeed * arriveDistance / slowRadius;
	}
	//cout << arriveDistance << "\n";
	//Set the target velocity to be in the direction from the NPC to the target...
	targetVel = normalize(result->linear);
	//...then set the speed of the NPC.
	targetVel *= targetSpeed;

	result->linear = targetVel - parent->GetComponent<Physics>()->GetVel();
	result->linear /= timeToTarget;

	if (arriveDistance > maxAccel)
	{
		result->linear = normalize(result->linear);
		result->linear *= maxAccel;
	}

	parent->GetComponent<Physics>()->ApplyForce(result->linear);

	cout << arriveDistance << "\n";

	result->angular = 0;
}

void AI::Align(vec3 targetOrientation, float MaxRotation, float targetRadius, float slowRadius, float timeToTarget)
{

}

void AI::KinWander(float maxSpeed, float maxRotation)
{

}

void AI::Wander(float wanderOffset, float wanderRadius, float wanderRate, float wanderOrientation, float maxAccel)
{
	                                                 
}
