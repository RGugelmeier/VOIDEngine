#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	//Getters
	inline mat4 GetView() const { return view; }
	inline mat4 GetPerspective() const { return perspective; }
	inline mat4 GetOrthograpic() const { return orthographic; }
	inline vec3 GetPosition() const { return position; }

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


private:
	void UpdateCameraVectors();
	vec3 position;
	mat4 perspective, orthographic, view;
	float fieldOfView;
	float yaw, pitch;
	float nearPlane, farPlane;
	vec3 forward, up, right, worldUp;
};

#endif // !CAMERA_H