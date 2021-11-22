#ifndef PHYSICS_H
#define PHYSICS_H

#include "../Components/Component.h"
#include "../Engine/Math/Quaternion.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

//Adding this component to a game object will give it physics, such as the ability to move with proper acceleration, and be affected by gravity.
class Physics : public Component
{
private:
	Quaternion qAngularVel, orientation;
	//ANGLES NEED TO BE VECTORS. THIS IS 3D ROTATION, NOT 2D.
	float mass, rotationalInertia, angle, angularAcc, angularVel;
	vec3 accel, vel;
public:
	Physics();
	Physics(float mass_, float rotationalInertia_);
	~Physics();

	virtual void OnCreate(GameObject* parent_);
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render() const;

	void ApplyGravity();
	void ApplyGravity(vec3 forceOfGravity);
	void ApplyForce(vec3 force);
	void ApplyRotation(Quaternion orientation, vec3 angularVel);
	void ApplyTorque(vec3 torque);

	//Setters
	inline void SetVel(vec3 newVel_) { vel = newVel_; }
	inline void SetAccel(vec3 newAccel_) { accel = newAccel_; }

	//Getters
	inline vec3 GetVel() { return vel; }
	inline vec3 GetAccel() { return accel; }
};

#endif //!PHYSICS_H