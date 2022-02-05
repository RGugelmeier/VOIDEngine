//Includes done here with forward declaration to avoid circular dependencies.
#include "CollisionDetection.h"
#include "Ray.h"
#include "../Core/CoreEngine.h"

CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::MousePosToWorldRay(vec2 mouseCoords_, float screenWidth_, float screenHeight_, Camera* camera_)
{
	//Ask about this: Can you explain NDC space again? 
	//Start and end position in NDC (normalised device coordinate) space.
	vec4 rayStartNDC((mouseCoords_.x / screenWidth_ - 0.5f) * 2.0f, (mouseCoords_.y / screenHeight_ - 0.5f) * 2.0f, -1.0f, 1.0f);
	vec4 rayEndNDC((mouseCoords_.x / screenWidth_ - 0.5f) * 2.0f, (mouseCoords_.y / screenHeight_ - 0.5f) * 2.0f, 0.0f, 1.0f);

	mat4 inverse = glm::inverse(camera_->GetPerspective() * camera_->GetView());

	//Convert from NDC to world space.
	vec4 rayStartWorld = inverse * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;

	vec4 rayEndWorld = inverse * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	//Set and normalise direction vector.
	vec3 rayDirWorld(rayEndWorld - rayStartWorld);
	rayDirWorld = normalize(rayDirWorld);

	return Ray(vec3(rayStartWorld), rayDirWorld, 0.0f); //Mayeb remove the 0.0f idk. I needed it because there is no constructor that takes only start pos and direction.
}

bool CollisionDetection::RayOBBIntersetion(Ray* ray_, BoundingBox* box_)
{
	//Get box and ray values.
	mat4 modelMatrix = box_->transform;
	vec3 rayOrigin = ray_->origin;
	vec3 rayDirection = ray_->direction;
	vec3 boxMin = box_->minVert;
	vec3 boxMax = box_->maxVert;
	//Get far and near plane for tMin and tMax initially. This makes it so this wont detect collisions for anything that is outside of the camera frustum.
	float tMin = CoreEngine::GetInstance()->GetCamera()->GetNearPlane();
	float tMax = CoreEngine::GetInstance()->GetCamera()->GetFarPlane();
	//Get the world position from the last column of the box's model matrix.
	vec3 worldPos(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	//Get the difference of the ray's origin and the world position.
	vec3 delta = worldPos - rayOrigin;

	//X axis collision check.
	//Get x axis from the model matirx's first column.
	vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
	//Dot product of delta and dir on the x axis.
	float dotDelta = dot(delta, xAxis);
	float dotDir = dot(rayDirection, xAxis);

	//If statement makes sure we dont divide by zero or anything too close to zero.
	if (fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + boxMax.x) / dotDir;
		float t2 = (dotDelta + boxMin.x) / dotDir;
		
		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2 < tMax)
		{
			tMax = t2;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1 > tMin)
		{
			tMin = t1;
		}

		//If the tamx is less than the tMin, return false because we already know there is no collision.
		if (tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		//If the ray is parralel or almost parralel with the box, return false as we already know there will be no collision
		if (-dotDelta + boxMin.x > 0.0f || -dotDelta + boxMax.x < 0.0f)
		{
			return false;
		}
	}

	//Y axis collision check.
	//Get y axis from the model matirx's second column.
	vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
	//Dot product of delta and dir on the y axis.
	dotDelta = dot(delta, yAxis);
	dotDir = dot(rayDirection, yAxis);

	//If statement makes sure we dont divide by zero or anything too close to zero.
	if (fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + boxMax.y) / dotDir;
		float t2 = (dotDelta + boxMin.y) / dotDir;

		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2 < tMax)
		{
			tMax = t2;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1 > tMin)
		{
			tMin = t1;
		}

		//If the tamx is less than the tMin, return false because we already know there is no collision.
		if (tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		//If the ray is parralel or almost parralel with the box, return false as we already know there will be no collision
		if (-dotDelta + boxMin.y > 0.0f || -dotDelta + boxMax.y < 0.0f)
		{
			return false;
		}
	}

	//Z axis collision check.
	//Get z axis from the model matirx's third column.
	vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
	//Dot product of delta and dir on the z axis.
	dotDelta = dot(delta, zAxis);
	dotDir = dot(rayDirection, zAxis);

	//If statement makes sure we dont divide by zero or anything too close to zero.
	if (fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + boxMax.z) / dotDir;
		float t2 = (dotDelta + boxMin.z) / dotDir;

		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2 < tMax)
		{
			tMax = t2;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1 > tMin)
		{
			tMin = t1;
		}

		//If the tamx is less than the tMin, return false because we already know there is no collision.
		if (tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		//If the ray is parralel or almost parralel with the box, return false as we already know there will be no collision
		if (-dotDelta + boxMin.z > 0.0f || -dotDelta + boxMax.z < 0.0f)
		{
			return false;
		}
	}

	//Set the distance from the collision and return true.
	ray_->intersectionDist = tMin;
	return true;
}

bool CollisionDetection::OBBOBBIntersection(BoundingBox box1_, BoundingBox box2_)
{
	//Get box and ray values.
	mat4 modelMatrix1 = box1_.transform;
	mat4 modelMatrix2 = box2_.transform;
	vec3 box1Min = box1_.minVert;
	vec3 box2Min = box2_.minVert;
	vec3 box1Max = box1_.maxVert;
	vec3 box2Max = box2_.maxVert;

	//Check x 
	if (dot(box1Max.x, box2Min.x) > 0.0f || dot(box1Max.y, box2Min.y) > 0.0f || dot(box1Max.z, box2Min.z) > 0.0f)
	{
		return true;
	}

	return true;
}

bool CollisionDetection::GJKDetection(GameObject* obj1, GameObject* obj2)
{
	Simplex simplex;
	//vec3 direction = Simplex::GetDirectionBetweenShapes(obj1, obj2);

	//Calculate and add the first simplex point using the direction between the two shapes.
	vec3 support = Simplex::Support(obj1, obj2, vec3(1.0f, 0.0f, 0.0f));
	simplex.push_front(support);

	//Set the new direction to be opposite the first simplex point.
	vec3 direction = -support;

	while (true)
	{
		//Calculate next simplex point
		support = Simplex::Support(obj1, obj2, direction);

		//There is no collision already. End.
		if (dot(support, direction) <= 0)
		{
			return false;
		}

		//Add next simplex point.
		simplex.push_front(support);

		if (simplex.NextSimplex(simplex.m_points, direction))
		{
			return true;
		}
	}
}