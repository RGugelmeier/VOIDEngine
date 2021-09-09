#include "Frustum.h"
#include "../Core/CoreEngine.h"
#include "../Camera/Camera.h"	//Included in cpp to avoid circular dependency in camera class. The camera need to have a frustum, but the frustum needs to get the camera's matrices.
#include "../Rendering/3D/Model.h"

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
//TODO change this to take a gameobject, not a model.
bool Frustum::SeenCheck(Model* modelToCheck_)
{
	//Check will hold the distance from a point to a frustum plane.
	float check;
	//Get the bounding box of the model we are checking.
	BoundingBox box = modelToCheck_->GetBoundingBox();
	
	//Left plane check;
	check = leftPlane.a * box.maxVert.x + leftPlane.b * box.maxVert.y + leftPlane.c * box.maxVert.z + leftPlane.d;
	if (check < 0)
	{
		cout << "Left: " << leftPlane.a << " " << leftPlane.b << " " << leftPlane.c << "\n\n";
		return false;
	}
	
	//Right plane check;
	check = rightPlane.a * box.minVert.x + rightPlane.b * box.minVert.y + rightPlane.c * box.minVert.z + rightPlane.d;
	if (check < 0)
	{
		cout << "Right: " << rightPlane.a << " " << rightPlane.b << " " << rightPlane.c << "\n\n";
		return false;
	}
	
	//Top plane check;
	check = topPlane.a * box.minVert.x + topPlane.b * box.minVert.y + topPlane.c * box.minVert.z + topPlane.d;
	if (check < 0)
	{
		cout << "Top: " << topPlane.a << " " << topPlane.b << " " << topPlane.c << "\n\n";
		return false;
	}
	
	//Bottom plane check;
	check = bottomPlane.a * box.maxVert.x + bottomPlane.b * box.maxVert.y + bottomPlane.c * box.maxVert.z + bottomPlane.d;
	if (check < 0)
	{
		cout << "Bottom: " << rightPlane.a << " " << rightPlane.b << " " << rightPlane.c << "\n\n";
		return false;
	}
	
	//Near plane check;
	check = nearPlane.a * box.maxVert.x + nearPlane.b * box.maxVert.y + nearPlane.c * box.maxVert.z + nearPlane.d;
	if (check < 0)
	{
		cout << "Near: " << nearPlane.a << " " << nearPlane.b << " " << nearPlane.c << "\n\n";
		return false;
	}
	
	//Far plane check;
	check = farPlane.a * box.minVert.x + farPlane.b * box.minVert.y + farPlane.c * box.minVert.z + farPlane.d;
	if (check < 0)
	{
		cout << "Far: " << farPlane.a << " " << farPlane.b << " " << farPlane.c << "\n\n";
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
	//			[3].x [3].y [3].z [3].w		3 - d

	//Extract x planes\\
	//Extract left plane.
	leftPlane.a = targetMatrix[3][0] + targetMatrix[0][0];
	leftPlane.b = targetMatrix[3][1] + targetMatrix[0][1];
	leftPlane.c = targetMatrix[3][2] + targetMatrix[0][2];
	leftPlane.d = targetMatrix[3][3] + targetMatrix[0][3];

	//Extract right plane.
	rightPlane.a = targetMatrix[3][0] - targetMatrix[0][0];
	rightPlane.b = targetMatrix[3][1] - targetMatrix[0][1];
	rightPlane.c = targetMatrix[3][2] - targetMatrix[0][2];
	rightPlane.d = targetMatrix[3][3] - targetMatrix[0][3];

	//Extract y planes.\\
	//Extract top plane.
	topPlane.a = targetMatrix[3][0] - targetMatrix[1][0];
	topPlane.b = targetMatrix[3][1] - targetMatrix[1][1];
	topPlane.c = targetMatrix[3][2] - targetMatrix[1][2];
	topPlane.d = targetMatrix[3][3] - targetMatrix[1][3];

	//Extract bottom plane.
	bottomPlane.a = targetMatrix[3][0] + targetMatrix[1][0];
	bottomPlane.b = targetMatrix[3][1] + targetMatrix[1][1];
	bottomPlane.c = targetMatrix[3][2] + targetMatrix[1][2];
	bottomPlane.d = targetMatrix[3][3] + targetMatrix[1][3];

	//Extract z planes.\\
	//Extract near plane.
	nearPlane.a = targetMatrix[3][0] + targetMatrix[2][0];
	nearPlane.b = targetMatrix[3][1] + targetMatrix[2][1];
	nearPlane.c = targetMatrix[3][2] + targetMatrix[2][2];
	nearPlane.d = targetMatrix[3][3] + targetMatrix[2][3];

	//Extract far plane.
	farPlane.a = targetMatrix[3][0] - targetMatrix[2][0];
	farPlane.b = targetMatrix[3][1] - targetMatrix[2][1];
	farPlane.c = targetMatrix[3][2] - targetMatrix[2][2];
	farPlane.d = targetMatrix[3][3] - targetMatrix[2][3];

	//TODO Normalize all planes.
}