#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "../Engine/Rendering/3D/GameObject.h"
#include <array>

using namespace glm;

class Simplex
{
public:
	Simplex();
	~Simplex();

	//The points that will make up the simplex.

	vec3 a, b, c, d;
	array<vec3, 4> m_points;
	int size;

	void push_front(vec3 point_);
	bool NextSimplex(array<vec3, 4> points, vec3 direction_);
	bool SameDirection(const vec3 direction_, vec3 ao_);
	bool Line(array<vec3, 4> points_, vec3 direction_);
	bool Triangle(array<vec3, 4> points_, vec3 direction_);
	bool Tetrahedron(array<vec3, 4> points_, vec3 direction_);
	static vec3 Support(GameObject* s1_, GameObject* s2_, vec3 d_);
	static vec3 GetDirectionBetweenShapes(GameObject* s1_, GameObject* s2_);
private:
	//TODO Make the simplex be created from bounding boxes rather than the actual model. Using the can take a lot more time due to the amount of vertices a model can have.
	//If you use a bounding box instead it will only ever have to check 8 verices.
	static vec3 GetFurthestPointInDirection(GameObject* s_, vec3 d_);
	static vec3 GetClosestPointInDirection(GameObject* s_, vec3 d);

};

#endif // !SIMPLEX_H