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


bool CollisionDetection::RayOBBIntersetion(Ray* ray_, BoundingBox* box_, vec3& pointOfContact, vec3& contactNormal)
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
	float t1x = FLT_MIN, t2x = FLT_MAX, t1y = FLT_MIN, t2y = FLT_MAX, t1z = FLT_MIN, t2z = FLT_MAX;
	//Get the world position from the last column of the box's model matrix.
	vec3 worldPos(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
	//Get the difference of the ray's origin and the world position.
	vec3 delta = worldPos - rayOrigin;

	//Y axis collision check.
	//Get y axis from the model matirx's second column.
	vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
	//Dot product of delta and dir on the y axis.
	float dotDelta = dot(delta, xAxis);
	float dotDir = dot(rayDirection, xAxis);

	//If statement makes sure we dont divide by zero or anything too close to zero.
	if (fabs(dotDir) > 0.001f)
	{
		t1x = (dotDelta + boxMax.x) / dotDir;
		t2x = (dotDelta + boxMin.x) / dotDir;

		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1x > t2x)
		{
			float w = t1x;
			t1x = t2x;
			t2x = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2x < tMax)
		{
			tMax = t2x;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1x > tMin)
		{
			tMin = t1x;
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
		t1y = (dotDelta + boxMax.y) / dotDir;
		t2y = (dotDelta + boxMin.y) / dotDir;

		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1y > t2y)
		{
			float w = t1y;
			t1y = t2y;
			t2y = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2y < tMax)
		{
			tMax = t2y;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1y > tMin)
		{
			tMin = t1y;
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
	float tMinZ = FLT_MIN;
	float tMaxZ = FLT_MAX;
	//Dot product of delta and dir on the z axis.
	dotDelta = dot(delta, zAxis);
	dotDir = dot(rayDirection, zAxis);

	//If statement makes sure we dont divide by zero or anything too close to zero.
	if (fabs(dotDir) > 0.001f)
	{
		t1z = (dotDelta + boxMax.z) / dotDir;
		t2z = (dotDelta + boxMin.z) / dotDir;

		//Make sure t2 is larger than t1. This is because t2 is supposed to corespond the tMax and t1 is supposed to corespond the tMin initially.
		//If it is not, swap the values.
		if (t1z > t2z)
		{
			float w = t1z;
			t1z = t2z;
			t2z = w;
		}

		//Check if the t2 is less than tMax. This is so we can get the smallest tMax for use later in our collision check.
		if (t2z < tMax)
		{
			tMax = t2z;
		}

		//Check if the t2 is less than tMax. This is so we can get the greatest tMin for use later in our collision check.
		if (t1z > tMin)
		{
			tMin = t1z;
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

	vec3 invdir = 1.0f / ray_->direction;

	//Set the distance from the collision and return true.
	ray_->intersectionDist = tMin;
	

	pointOfContact = ray_->origin + tMin * ray_->direction;

	//contactNormal = normalize(pointOfContact); //Make either 0 0 1, 1 0 0, or 0 1 0.

	//If collision on the x axis wall
	if(t1x > t1y && t1x > t1z)
	{
		if (invdir.x < 0.0f)
		{
			contactNormal = vec3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			contactNormal = vec3(-1.0f, 0.0f, 0.0f);
		}
	}
	
	else if (t1y > t1z && t1y > t1x)
	{
		if (invdir.y < 0.0f)
		{
			contactNormal = vec3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			contactNormal = vec3(0.0f, -1.0f, 0.0f);
		}
	}

	if (t1z > t1y && t1z > t1x)
	{
		if (invdir.z < 0.0f)
		{
			contactNormal = vec3(0.0f, 0.0f, 1.0f);
		}
		else
		{
			contactNormal = vec3(0.0f, 0.0f, -1.0f);
		}
	}

	rayDirection = normalize(rayDirection);

	return true;
}

bool CollisionDetection::DynamicOBBOBBIntersects(GameObject* dynamicObj, GameObject* staticObj, vec3& contactPoint, vec3& contactNormal, float& intersectionDist_, Ray& testRay_)
{
	Physics* dynamicPhysics = dynamicObj->GetComponent<Physics>();

	vec3 dynamicObjVel = dynamicPhysics->GetVel();

	//BoundingBox *expandedBox = new BoundingBox(staticObj->GetBoundingBox().maxVert, staticObj->GetBoundingBox().minVert, staticObj->GetBoundingBox().transform);
	BoundingBox *expandedBox = new BoundingBox(staticObj->GetBoundingBox().maxVert + (staticObj->GetBoundingBox().size), staticObj->GetBoundingBox().minVert - (staticObj->GetBoundingBox().size), staticObj->GetBoundingBox().transform);
	//Create and cast a ray from the 
	//The ray is not using the right velocity. The player's vel is the same always in only one direction, which ruins the rayobb collision code when getting the dotDir.

	//TODO find a way to del;ete this ray. MEMORY LEAK OH NO.
	Ray* testRay = new Ray(dynamicObj->GetPosition(), dynamicObjVel * CoreEngine::GetInstance()->GetMainTimer()->GetDeltaTime(), 0.0f);
	Ray* groundTest = new Ray(dynamicObj->GetPosition(), vec3(0.01f, -1.0f, 0.01f) * CoreEngine::GetInstance()->GetMainTimer()->GetDeltaTime(), 0.0f);
	groundTest->direction = normalize(groundTest->direction);

	if (RayOBBIntersetion(groundTest, expandedBox, contactPoint, contactNormal))
	{
		if (groundTest->intersectionDist >= 0.0f && groundTest->intersectionDist <= 1.0f)
		{
			dynamicObj->GetComponent<Physics>()->SetVel(dynamicObj->GetComponent<Physics>()->GetVel() + (contactNormal * abs(dynamicObj->GetComponent<Physics>()->GetVel())));
			//dynamicObj->GetComponent<Physics>()->SetVel(vec3(dynamicObjVel.x, 0.0f, dynamicObjVel.z));// dynamicObj->GetComponent<Physics>()->GetVel() + (contactNormal * abs(dynamicObj->GetComponent<Physics>()->GetVel())));
			dynamicObj->SetGroundCheck(true);
		}
		else
		{
			dynamicObj->SetGroundCheck(false);
		}
	}

	//Check to see if the ray collides with the other object. If it does, send the contactPoint, normal, and time forward.
	if (RayOBBIntersetion(testRay, expandedBox, contactPoint, contactNormal))
	{
		intersectionDist_ = testRay->intersectionDist;
		//If the contactTime is between 0 and 1, there is a collision in the next frame. The collision is resolved in CollisionHandler.
		testRay_ = *testRay;
		return (testRay->intersectionDist >= 0.0f && testRay->intersectionDist <= 1.0f);
	}

	return false;
}

bool CollisionDetection::OBBOBBIntersection(BoundingBox box1_, BoundingBox box2_)
{
	//Get box values.
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

	return false;
}

bool CollisionDetection::GJKDetection(GameObject* obj1, GameObject* obj2)
{
	GameObject* largerModel;
	mat4 m;
	if (obj1->GetModel()->GetVertices().size() > obj2->GetModel()->GetVertices().size())
	{
		largerModel = obj1;
	}
	else
	{
		largerModel = obj2;
	}
#pragma region GetSimplexPoints
	Simplex* simplex = new Simplex();
	vec3 direction = Simplex::GetDirectionBetweenShapes(obj1, obj2);
	//Get the first point of the simplex. d = direction from center of shape 1 to 2.
	simplex->a = Simplex::Support(obj1, obj2, direction);
	//Get the second point of the simplex. d = inverse direction from center of shape 1 to 2.
	simplex->b = Simplex::Support(obj1, obj2, -direction);
	//Get the third point of the simplex. d = Perpendicular line of the line created from previous two points that points towards the origin (0, 0, 0).
	simplex->c = Simplex::Support(obj1, obj2, cross(cross((simplex->b - simplex->a), simplex->a - vec3(0.0f, 0.0f, 0.0f)), (simplex->b - simplex->a)));
	//Get the fourth point of the simplex. d = normal of triangle created by previous 3 points (abc), directed towards the origin (0, 0 ,0).
	vec3 N = cross((simplex->b - simplex->a), (simplex->c - simplex->a));
	//Check if the dot product of the normal and the line from the center of the triangle to the origin is positive. If it is, use N, if not, use -N.
	if (dot(N, vec3(0.0f, 0.0f, 0.0f) - (vec3((simplex->a + simplex->b + simplex->c) / 3.0f))) > 0)
	{
		simplex->d = Simplex::Support(obj1, obj2, N);
	}
	else
	{
		simplex->d = Simplex::Support(obj1, obj2, -N);
	}
#pragma endregion
#pragma region CheckForCollisionWithSimplex
	int check = 0;
	while (check <= 2)
	{
		check++;
#pragma region CheckDeterminates
		//Get determinate 0.
		m = mat4(simplex->a.x, simplex->a.y, simplex->a.z, 1,
			simplex->b.x, simplex->b.y, simplex->b.z, 1,
			simplex->c.x, simplex->c.y, simplex->c.z, 1,
			simplex->d.x, simplex->d.y, simplex->d.z, 1);
		float D0 = determinant(m);
		//Get determinate 1
		m = mat4(0, 0, 0, 1,
			simplex->b.x, simplex->b.y, simplex->b.z, 1,
			simplex->c.x, simplex->c.y, simplex->c.z, 1,
			simplex->d.x, simplex->d.y, simplex->d.z, 1);
		float D1 = determinant(m);
		//Get determinate 2
		m = mat4(simplex->a.x, simplex->a.y, simplex->a.z, 1,
			0, 0, 0, 1,
			simplex->c.x, simplex->c.y, simplex->c.z, 1,
			simplex->d.x, simplex->d.y, simplex->d.z, 1);
		float D2 = determinant(m);
		//Get determinate 3
		m = mat4(simplex->a.x, simplex->a.y, simplex->a.z, 1,
			simplex->b.x, simplex->b.y, simplex->b.z, 1,
			0, 0, 0, 1,
			simplex->d.x, simplex->d.y, simplex->d.z, 1);
		float D3 = determinant(m);
		//Get determinate 4
		m = mat4(simplex->a.x, simplex->a.y, simplex->a.z, 1,
			simplex->b.x, simplex->b.y, simplex->b.z, 1,
			simplex->c.x, simplex->c.y, simplex->c.z, 1,
			0, 0, 0, 1);
		float D4 = determinant(m);
#pragma endregion
		//Check if they're all the same sign. If they are, we're done. Collision has happened.
		if (SameSign(D0, D1) && SameSign(D0, D2) && SameSign(D0, D3) && SameSign(D0, D4))
		{
			return true;
		}
		//1 does not match
		if (SameSign(D0, D2) && SameSign(D0, D3) && SameSign(D0, D4))
		{
			N = cross((simplex->b - simplex->d), (simplex->c - simplex->d));
			if (dot(N, vec3(0.0f, 0.0f, 0.0f) - vec3((simplex->b + simplex->c + simplex->d) / 3.0f)) > 0)
			{
				simplex->a = Simplex::Support(obj1, obj2, N);
			}
			else
			{
				simplex->a = Simplex::Support(obj1, obj2, -N);
			}
		}
		//2 does not match
		else if (SameSign(D0, D1) && SameSign(D0, D3) && SameSign(D0, D4))
		{
			N = cross((simplex->d - simplex->a), (simplex->c - simplex->a));
			if (dot(N, vec3(0.0f, 0.0f, 0.0f) - vec3((simplex->a + simplex->c + simplex->d) / 3.0f)) > 0)
			{
				simplex->b = Simplex::Support(obj1, obj2, N);
			}
			else
			{
				simplex->b = Simplex::Support(obj1, obj2, -N);
			}
		}
		//3 does not match
		else if (SameSign(D0, D1) && SameSign(D0, D2) && SameSign(D0, D4))
		{
			N = cross((simplex->b - simplex->a), (simplex->d - simplex->a));
			//Check if the dot product of the normal and the line from the center of the triangle to the origin is positive. If it is, use N, if not, use -N.
			if (dot(N, vec3(0.0f, 0.0f, 0.0f) - vec3((simplex->a + simplex->b + simplex->d) / 3.0f)) > 0)
			{
				simplex->c = Simplex::Support(obj1, obj2, N);
			}
			else
			{
				simplex->c = Simplex::Support(obj1, obj2, -N);
			}
		}
		//4 does not match
		else if (SameSign(D0, D1) && SameSign(D0, D2) && SameSign(D0, D3))
		{
			N = cross((simplex->b - simplex->a), (simplex->c - simplex->a));
			//Check if the dot product of the normal and the line from the center of the triangle to the origin is positive. If it is, use N, if not, use -N.
			if (dot(N, vec3(0.0f, 0.0f, 0.0f) - vec3((simplex->a + simplex->b + simplex->c) / 3.0f)) > 0)
			{
				simplex->d = Simplex::Support(obj1, obj2, N);
			}
			else
			{
				simplex->d = Simplex::Support(obj1, obj2, -N);
			}
		}
	}
#pragma endregion
	delete simplex;
	simplex = nullptr;
	return false;
}


