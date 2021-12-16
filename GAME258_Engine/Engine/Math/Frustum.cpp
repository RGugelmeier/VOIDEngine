#include "Frustum.h"
#include "../Core/CoreEngine.h"
#include "../Camera/Camera.h"	//Included in cpp to avoid circular dependency in camera class. The camera need to have a frustum, but the frustum needs to get the camera's matrices.
#include "../Rendering/3D/Model.h"
#include "../Rendering/3D/GameObject.h"

unique_ptr<Frustum> Frustum::frustumInstance = nullptr;

Frustum::Frustum()
{
	//Set default values and extract planes from the camera's view.
	engineInstance = CoreEngine::GetInstance();
	cam_ = engineInstance->GetCamera();
	UpdatePlanes(cam_->GetView() * cam_->GetPerspective());
}

//Clean up.
Frustum::~Frustum()
{
	cam_ = nullptr;
	engineInstance = nullptr;
}

//Gets the frustum instance if one exists. If not, create one and return that instead.
Frustum* Frustum::GetInstance()
{
	if (frustumInstance.get() == nullptr)
	{
		frustumInstance.reset(new Frustum);
	}

	return frustumInstance.get();
}

//Checks if the model that is passed in is inside of the frustum by using the dot product of a point on the bounding box and each frustum wall.
bool Frustum::SeenCheck(GameObject* objectToCheck_)
{
	//Normalize all of the planes.
	PlaneNormalization(nearPlane);
	PlaneNormalization(farPlane);
	PlaneNormalization(leftPlane);
	PlaneNormalization(rightPlane);
	PlaneNormalization(topPlane);
	PlaneNormalization(bottomPlane);
	//Check will hold the distance from a point to a frustum plane.
	float check;
	//Get the bounding box of the model we are checking.
	BoundingBox box = objectToCheck_->GetBoundingBox();
	
	float mag; 

	//Left plane check;
	check = leftPlane.a * box.maxVert.x + leftPlane.b * box.maxVert.y + leftPlane.c * box.maxVert.z + leftPlane.d;
	if (check < 0)
	{
		return false;
	}
	
	//Right plane check;
	check = rightPlane.a * box.minVert.x + rightPlane.b * box.minVert.y + rightPlane.c * box.minVert.z + rightPlane.d;
	if (check < 0)
	{
		return false;
	}
	
	//Top plane check;
	check = topPlane.a * box.minVert.x + topPlane.b * box.minVert.y + topPlane.c * box.minVert.z + topPlane.d;
	if (check < 0)
	{
		return false;
	}
	
	//Bottom plane check;
	check = bottomPlane.a * box.maxVert.x + bottomPlane.b * box.maxVert.y + bottomPlane.c * box.maxVert.z + bottomPlane.d;
	if (check < 0)
	{
		return false;
	}
	
	//Near plane check;
	check = nearPlane.a * box.maxVert.x + nearPlane.b * box.maxVert.y + nearPlane.c * box.maxVert.z + nearPlane.d;
	if (check < 0)
	{
		return false;
	}
	
	//Far plane check;
	check = farPlane.a * box.minVert.x + farPlane.b * box.minVert.y + farPlane.c * box.minVert.z + farPlane.d;
	if (check < 0)
	{
		return false;
	}

	return true;
}

//This extracts the frustum planes from the camera.
void Frustum::UpdatePlanes(mat4 targetMatrix)
{
	//mat4 =	[0].x [0].y [0].z [0].w		0 = a
	//			[1].x [1].y [1].z [1].w		1 = b
	//			[2].x [2].y [2].z [2].w		2 = c
	//			[3].x [3].y [3].z [3].w		3 = d

	//Extract x planes\\
	//Extract left plane.
	leftPlane.a = targetMatrix[0][3] + targetMatrix[0][0];
	leftPlane.b = targetMatrix[1][3] + targetMatrix[1][0];
	leftPlane.c = targetMatrix[2][3] + targetMatrix[2][0];
	leftPlane.d = targetMatrix[3][3] + targetMatrix[3][0];

	//Extract right plane.
	rightPlane.a = targetMatrix[0][3] - targetMatrix[0][0];
	rightPlane.b = targetMatrix[1][3] - targetMatrix[1][0];
	rightPlane.c = targetMatrix[2][3] - targetMatrix[2][0];
	rightPlane.d = targetMatrix[3][3] - targetMatrix[3][0];

	//Extract y planes.\\
	//Extract top plane.
	topPlane.a = targetMatrix[0][3] - targetMatrix[0][1];
	topPlane.b = targetMatrix[1][3] - targetMatrix[1][1];
	topPlane.c = targetMatrix[2][3] - targetMatrix[2][1];
	topPlane.d = targetMatrix[3][3] - targetMatrix[3][1];

	//Extract bottom plane.
	bottomPlane.a = targetMatrix[0][3] + targetMatrix[0][1];
	bottomPlane.b = targetMatrix[1][3] + targetMatrix[1][1];
	bottomPlane.c = targetMatrix[2][3] + targetMatrix[2][1];
	bottomPlane.d = targetMatrix[3][3] + targetMatrix[3][1];

	//Extract z planes.\\
	//Extract near plane.
	nearPlane.a = targetMatrix[0][3] + targetMatrix[0][2];
	nearPlane.b = targetMatrix[1][3] + targetMatrix[1][2];
	nearPlane.c = targetMatrix[2][3] + targetMatrix[2][2];
	nearPlane.d = targetMatrix[3][3] + targetMatrix[3][2];

	//Extract far plane.
	farPlane.a = targetMatrix[0][2];
	farPlane.b = targetMatrix[1][2];
	farPlane.c = targetMatrix[2][2];
	farPlane.d = targetMatrix[3][2];
}

void Frustum::PlaneNormalization(Plane plane_)
{
	float mag;

	mag = sqrt(plane_.a * plane_.a + plane_.b * plane_.b + plane_.c * plane_.c);
	plane_.a /= mag;
	plane_.b /= mag;
	plane_.c /= mag;
	plane_.d /= mag;
}
