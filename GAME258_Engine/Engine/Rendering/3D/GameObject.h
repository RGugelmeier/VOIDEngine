#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class GameObject
{
public:
	GameObject(Model* model_);
	GameObject(Model* model_, vec3 position_);
	~GameObject();

	void Update(const float deltaTime_);
	void Render(Camera* camera_);

	inline vec3 GetPosition() const { return position; }
	inline float GetAngle() const { return angle; }
	inline vec3 GetRotation() const { return rotation; }
	inline vec3 GetScale() const { return scale; }
	
	inline void SetPosition(vec3 position_);
	inline void SetAngle(float angle_);
	inline void SetRotation(vec3 rotation_);
	inline void SetScale(vec3 scale_);
private:
	Model* model;
	unsigned int modelInstance;
	vec3 position;
	float angle;
	vec3 rotation;
	vec3 scale;
};

#endif // !GAMEOBJECT_H