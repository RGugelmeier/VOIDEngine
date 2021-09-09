#include "Camera.h"

#include "../Core/CoreEngine.h"

//Zero out values first, then set default values of the camera.
Camera::Camera() : position(vec3()), fieldOfView(0.0f), forward(vec3()), up(vec3()), right(vec3()), worldUp(vec3()), nearPlane(0.0f), farPlane(0.0f), yaw(0.0f), pitch(0.0f), perspective(mat4()),
orthographic(mat4()), view(mat4())
{
	fieldOfView = 45.0f;
	forward = vec3(0.0f, 0.0f, -1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	nearPlane = 0.5f;
	farPlane = 50.0f;
	yaw = -90.0f;
	pitch = 0.0f;

	//Changes depending on fieldOfView
	perspective = glm::perspective(fieldOfView, CoreEngine::GetInstance()->GetScreenWidth() / CoreEngine::GetInstance()->GetScreenHeight(), nearPlane, farPlane);

	//Set the orthographic "walls" to be at the edges of the screen on the sides and top/bottom.
	//Never changes.
	orthographic = ortho(0.0f, CoreEngine::GetInstance()->GetScreenWidth(), 0.0f, CoreEngine::GetInstance()->GetScreenHeight(), -1.0f, 1.0f);

	//Initial set of the camera's vectors.
	UpdateCameraVectors();
}

Camera::~Camera()
{
	if (lights.size() > 0)
	{
		for (auto light : lights)
		{
			delete light;
			light = nullptr;
		}
		lights.clear();
	}
}

//Process mouse inputs. (Movement and zoon)
void Camera::ProcessMouseMovement(vec2 offset_)
{
	//Change mouse yaw and pitch. Rotate the camera.
	offset_ *= 0.05f;

	yaw += offset_.x;
	pitch += offset_.y;

	//Make sure pitch does not go out of bounds. If it does, it will flip the screen. No good.
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	//Make sure yaw does not go out of bounds. This makes it so we always have a positive yaw, and it always stays inside the range of one full circle.
	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}

	//Update the camera data.
	UpdateCameraVectors();
	Frustum::GetInstance()->UpdatePlanes(perspective * view);
}

void Camera::ProcessMouseZoom(int y_)
{
	//Depending on direction of scrolling, change position of camera.
	if (y_ < 0 || y_ > 0)
	{
		position += static_cast<float>(y_) * (forward * 2.0f);
	}

	//Update the camera data.
	UpdateCameraVectors();
	Frustum::GetInstance()->UpdatePlanes(perspective * view);
}

//This function gets the forward, right, and up vectors, then sets the camera to look at the correct spot.
void Camera::UpdateCameraVectors()
{
	//Set the forward vector.
	forward.x = cos(radians(yaw)) * cos(radians(pitch));
	forward.y = sin(radians(pitch));
	forward.z = sin(radians(yaw)) * cos(radians(pitch));

	//Normalize all vectors.
	forward = normalize(forward);

	right = normalize(cross(forward, worldUp));

	up = normalize(cross(right, forward));
	
	//Set the view matrix.
	view = lookAt(position, position + forward, up);
}

void Camera::AddLight(LightSource* light_)
{
	lights.push_back(light_);
}