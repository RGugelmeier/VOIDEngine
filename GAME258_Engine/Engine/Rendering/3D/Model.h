#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Model
{
public:
	//Set default values for a model. Rotation variable determines what axis to rotate on, x, y, or z.
	Model(GLuint shaderProgram_, vec3 position_ = vec3(), float angle_ = 0.0f, vec3 rotation_ = vec3(0.0f,1.0f,0.0f), vec3 scale_ = vec3(1.0f));
	~Model();

	void Render(Camera* camera_);
	void AddMesh(Mesh* mesh_);

	//Getters
	inline vec3 GetPosition() const { return position; }
	inline float GetAngle() const { return angle; }
	inline vec3 GetRotation() const { return rotation; }
	inline vec3 GetScale() const { return scale; }

	//Setters
	inline void SetPosition(vec3 position_) { position = position_; }
	inline void SetAngle(float angle_) { angle = angle_; }
	inline void SetRotation(vec3 rotation_) { rotation = rotation_; }
	inline void SetScale(vec3 scale_) { scale = scale_; }

private:
	vector<Mesh*> meshes;
	GLuint shaderProgram;

	vec3 position;
	float angle;
	vec3 rotation;
	vec3 scale;
	mat4 GetTransform() const;
};

#endif // !MODEL_H