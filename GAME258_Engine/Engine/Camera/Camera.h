#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

//#include "../Math/Frustum.h"
#include "../FX/LightSource.h"
#include "../../Components/Component.h"

using namespace glm;
using namespace std;

class Skybox;

class Camera : public Component
{
public:
	Camera();
	~Camera();

	virtual void Update(const float deltaTime);
	virtual void OnCreate(GameObject* parent_);
	virtual void OnDestroy();
	virtual void Render() const;

	void AddLight(LightSource* light_);
	void AddLight(PointLight* light_);
	void AddLight(DirectionalLight* light_);
	void AddLight(SpotLight* light_);

	//Getters
	inline mat4 GetView() const { return view; }
	inline mat4 GetPerspective() const { return perspective; }
	inline mat4 GetOrthograpic() const { return orthographic; }
	inline vec3 GetPosition() const { return position; }
	inline float GetNearPlane() const { return nearPlane; }
	inline float GetFarPlane() const { return farPlane; }
	//inline Frustum GetFrustum() const { return frustum; }
	inline vector<LightSource*> GetLights() const { return lights; }
	inline vector<SpotLight*> GetSpotLights() const { return spotLights; }
	inline vector<DirectionalLight*> GetDirectionalLights() const { return directionalLights; }
	inline vector<PointLight*> GetPointLights() const { return pointLights; }
	inline void setSkybox(Skybox* skyBox_) { skyBox = skyBox_; }

	//Setters
	//Set the given value, then update the camera's vectors to be proper.
	inline void SetPosition(vec3 position_) {
		position = position_;
		UpdateCameraVectors();
	}
	inline void SetRotation(float yaw_, float pitch_) {
		yaw = yaw_; 
		pitch = pitch_;
		UpdateCameraVectors();
	}

	void ProcessMouseMovement(vec2 offset_);
	void ProcessMouseZoom(int y_);
	void ProcessCameraMovement(float x_, float y_, float z_);
	void UpdateCameraVectors();
private:
	vec3 position;
	mat4 perspective, orthographic, view;
	float fieldOfView;
	float yaw, pitch;
	float nearPlane, farPlane;
	vec3 forward, up, right, worldUp;
	
	//Frustum* frustum;
	vector<LightSource*> lights;
	vector<PointLight*> pointLights;
	vector<DirectionalLight*> directionalLights;
	vector<SpotLight*> spotLights;
	Skybox* skyBox;
};

#endif // !CAMERA_H