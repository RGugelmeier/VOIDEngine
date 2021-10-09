#ifndef AI_H
#define AI_H

#include "Component.h"
#include "../Engine/Rendering/3D/GameObject.h"
#include "../Physics/Physics.h"

#include <glm/glm.hpp>

using namespace glm;

struct SteeringOutput
{
	vec3 linear;
	float angular;
};

class AI : public Component
{
public:
	AI();
	~AI();

	virtual void OnCreate(GameObject* parent_);
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render() const;

	void Seek(vec3 target, float maxAccel);
	void Arrive(vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget);
	void Align(vec3 target, float MaxRotation, float targetRadius, float slowRadius, float timeToTarget);
	//void Face
	void KinWander(float maxSpeed, float maxRotation);
	void Wander(float wanderOffset, float wanderRadius, float wanderRate, float wanderOrientation, float maxAccel);

	inline void GetSteering() {  };
private:
	SteeringOutput* result;

	//Arrive variables
	float arriveDistance, targetSpeed;
	vec3 targetVel, direction;
};
#endif // !AI_H