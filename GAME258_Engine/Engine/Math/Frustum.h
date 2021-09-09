#ifndef FRUSTUM_H
#define FRUSTUM_H

//Forward declare so the reference can be deleted in the destructor.
class Camera;	
class CoreEngine;
class Model;

#include <memory>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

//A plane struct for creating the frustum planes.
struct Plane {
	float a, b, c, d;
};



class Frustum
{
public:
	//Next 4 lines prevent use of copy or move constructor. Ensures only one engine will exist at any time.
	Frustum(const Frustum&) = delete;
	Frustum(Frustum&&) = delete;
	Frustum& operator=(const Frustum&) = delete;
	Frustum& operator=(Frustum&&) = delete;
	
	Frustum();
	~Frustum();
	
	//Getters
	inline Plane GetTop() const { return topPlane; }
	inline Plane GetBottom() const { return bottomPlane; }
	inline Plane GetLeft() const { return leftPlane; }
	inline Plane GetRight() const { return rightPlane; }
	inline Plane GetNear() const { return nearPlane; }
	inline Plane GetFar() const { return farPlane; }

	bool SeenCheck(Model* modelToCheck_);

	static Frustum* GetInstance();

	//Used to extract the walls and build the frustum.
	void UpdatePlanes(mat4 targetMatrix);
private:
	static unique_ptr<Frustum> frustumInstance;
	friend default_delete<Frustum>;
	
	//Normalize a plane.
	void PlaneNormalization();
	
	
	Camera* cam_;
	CoreEngine* engineInstance;
	//The 6 planes of the frustum.
	Plane topPlane, bottomPlane, leftPlane, rightPlane, nearPlane, farPlane;
};

#endif // !FRUSTUM_H