#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../../Components/Component.h"
#include "Model.h"

#include <list>

class OctNode;
class CollisionHandler;

class GameObject
{
public:
	GameObject(vec3 position_ = vec3());
	GameObject(Model* model_, vec3 position_ = vec3(), bool moveable_ = false);
	~GameObject();

	void OnCreate();
	virtual void Update(const float deltaTime_);
	void Render(Camera* camera_);

	//Getters
	inline vec3 GetScale() const { return scale; }
	inline string GetTag() const { return name; }
	inline BoundingBox GetBoundingBox() const { return boundingBox; }
	inline BoundingBox* GetBoundingBoxPtr() { return &boundingBox; }
	inline bool GetHit() const { return hit; }
	inline Model* GetModel() const {  if(model) return model; }
	inline vec3 GetPosition() const { return position; }
	inline bool GetGroundCheck() const { return groundCheck; }
	vector<vec3> GetVertices();

	//Setters
	void SetPosition(vec3 position_);
	void SetAngle(float angle_);
	void SetRotation(vec3 vRotation_);
	void SetRotation(quat qRotation_);
	void SetScale(vec3 scale_);
	void SetTag(string tag_);
	void SetHit(bool hit_, int buttonType_);
	void SetGroundCheck(bool groundCheck_);

	void UpdateVectors(float yaw, float pitch);

	//Component functions
	//Templated function. Allows for any object type to be used as an argument passed into this function.
	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		ComponentTemplate* comp = new ComponentTemplate(std::forward<Args>(args_)...);
		if (!dynamic_cast<Component*>(comp)) {
			///Trying to add a component that is not a base class of Component class
			delete comp;
			comp = nullptr;
			return;
		}
		if (GetComponent<ComponentTemplate>()) {

			///Trying to add a component type that is already added
			delete comp;
			comp = nullptr;
			return;
		}
		components.push_back(comp);
		comp->OnCreate(this);
	}

	//Gets the component of the given type (declared in <> when calling this function) and returns it.
	template<typename ComponentTemplate>
	ComponentTemplate* GetComponent() {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component)) {
				return dynamic_cast<ComponentTemplate*>(component);
			}
		}
		return nullptr;
	}

	//Search the component list for the given component type (declared in <> when calling function) and deletes it, then removes it from the component list.
	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i])) {
				delete components[i];
				components[i] = nullptr;
				components.erase(components.begin() + i);
				break;
			}
		}
	}

	vec3 position;
	float angle;
	vec3 vRotation;
	quat qRotation;
	list<OctNode*> collidedNodes;
	list<OctNode*>::iterator collidedNodesIterator;
	bool moveable;
	vec3 forward, right, up, worldUp;
private:
	vector<vec3> FillVertices();
	Model* model;
	unsigned int modelInstance;
	vec3 scale;
	//float yaw, pitch;
	string name;

	BoundingBox boundingBox;

	bool hit, setNewOctNodes, groundCheck;

	vector<Component*> components;
	vector<vec3> modelVertices;

	CollisionHandler* collisionHandlerInstance;
};

#endif // !GAMEOBJECT_H