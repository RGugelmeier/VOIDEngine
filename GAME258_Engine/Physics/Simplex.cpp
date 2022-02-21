#include "Simplex.h"

Simplex::Simplex() : a(vec3()), b(vec3()), c(vec3()), d(vec3()), m_points(array<vec3, 4>()), size(0)
{
	
}

Simplex::~Simplex()
{
}

//TODO Put this function somewhere that it fits better. This can be used for more than just simplex. Maybe in a math library?
vec3 Simplex::GetDirectionBetweenShapes(GameObject* s1, GameObject* s2)
{
	//Get center of each object.
	vec3 c1, c2, d;
	vec3 sumOfVertices = vec3();

	//Add together all vertices of shapes and divide by amount of vertices to get center of object.
	//Center of chape 1
	for (unsigned int i = 0; i < s1->GetVertices().size(); i++)
	{
		sumOfVertices += s1->GetVertices()[i];
	}
	c1 = sumOfVertices / float(s1->GetVertices().size());
	sumOfVertices = vec3();
	//Center of shape 2
	for (unsigned int i = 0; i < s2->GetVertices().size(); i++)
	{
		sumOfVertices += s2->GetVertices()[i];
	}
	c2 = sumOfVertices / float(s2->GetVertices().size());

	//Get dirrection between the two shapes.
	d = c1 - c2;

	return d;
}

//Add a point to the simplex. Removes the points stored in [3]. Sets the size to be proper. Does not let the size go above 4.
void Simplex::push_front(vec3 point_)
{
	m_points = { point_, m_points[0], m_points[1], m_points[2] };
	size = std::min(size + 1, 4);
}

vec3 Simplex::GetFurthestPointInDirection(GameObject* s_, vec3 d_)
{
	vec3 furthest = vec3();
	float largest;
	float test;
	
	largest = -FLT_MAX;

	//Maybe get rid of the & if something goes wrong.
	for (const vec3& vertex : s_->GetVertices())
	{
		float test = dot(vertex, d_);
		if (test > largest)
		{
			largest = test;
			furthest = vertex;
		}
	}

	return furthest;
}

vec3 Simplex::GetClosestPointInDirection(GameObject* s_, vec3 d_)
{
	vec3 closest = vec3();
	float largest = 0.0f;
	float test;
	//Loop through all vertices
	for (unsigned int i = 0; i < s_->GetVertices().size(); i++)
	{
		//Check if the dot product of the current vertex and the direction is greater than the previously largest recorded dot product.
		test = dot(s_->GetVertices()[i], d_);
		if (test < largest)
		{
			//Record the new largest dot product and the new furthest vertex.
			largest = test;
			closest = s_->GetVertices()[i];
		}
	}

	return closest;
}

//Gets a point of the simplex given the two shapes and the direction.
vec3 Simplex::Support(GameObject* s1_, GameObject* s2_, vec3 d_)
{
	return GetFurthestPointInDirection(s1_, d_) - GetFurthestPointInDirection(s2_, -d_);
}
