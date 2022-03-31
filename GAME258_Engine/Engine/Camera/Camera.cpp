#include "Camera.h"

#include "../Rendering/SceneGraph.h"
#include "../Rendering/Skybox.h"
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
	farPlane = 500.0f;
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

void Camera::Update(const float deltaTime)
{
	if (position != SceneGraph::GetInstance()->GetGameObject("Player")->position)
	{
		CoreEngine::GetInstance()->MoveCamera(vec3(SceneGraph::GetInstance()->GetGameObject("Player")->position.x, SceneGraph::GetInstance()->GetGameObject("Player")->position.y + 1.0f, SceneGraph::GetInstance()->GetGameObject("Player")->position.z));
	}
}

void Camera::OnCreate(GameObject* parent_)
{
	parent = parent_;
}

void Camera::OnDestroy()
{
}

void Camera::Render() const
{
	//If there is a skybox, render it.
	if (skyBox)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		GLuint programID = ShaderHandler::GetInstance()->GetShader("skyboxShader");

		glUseProgram(programID);

		mat4 skyboxView = mat4(mat3(GetView()));

		glUniformMatrix4fv(1, 1, GL_FALSE, value_ptr(perspective));
		glUniformMatrix4fv(0, 1, GL_FALSE, value_ptr(skyboxView));

		skyBox->Render();

		glUseProgram(0);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
}

//Process mouse inputs. (Movement and zoom)
void Camera::ProcessMouseMovement(vec2 offset_)
{
	//Change mouse yaw and pitch. Rotate the camera.
	offset_ *= 0.05f;

	yaw += offset_.x;
	pitch -= offset_.y;

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

	if (SceneGraph::GetInstance()->GetGameObject("Player"))
	{
		SceneGraph::GetInstance()->GetGameObject("Player")->UpdateVectors(yaw, pitch);
	}
	else
	{
		Debug::Warning("There is no player game object for the camera to use.", "Camera.cpp", __LINE__);
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

//This actually moves the camera. x y and z are how much the camera should move each call.
//Multiplies the x y and z args with the neg forward, up, and right vectors to move acurately with the camera rotated.
void Camera::ProcessCameraMovement(float x_, float y_, float z_)
{
	position += static_cast<vec3>(x_ * right, y_ * up, z_ * -forward);
	position += right * x_;
	UpdateCameraVectors();

	if (SceneGraph::GetInstance()->GetGameObject("Player"))
	{
		SceneGraph::GetInstance()->GetGameObject("Player")->UpdateVectors(yaw, pitch);
		SceneGraph::GetInstance()->GetGameObject("Player")->SetPosition(position);
	}
	else
	{
		Debug::Warning("There is no player game object for the camera to use.", "Camera.cpp", __LINE__);
	}

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

//Add lights to camera.
void Camera::AddLight(LightSource* light_)
{
	lights.push_back(light_);
}

void Camera::AddLight(PointLight* light_)
{
	pointLights.push_back(light_);
}

void Camera::AddLight(DirectionalLight* light_)
{
	directionalLights.push_back(light_);
}

void Camera::AddLight(SpotLight* light_)
{
	spotLights.push_back(light_);
}