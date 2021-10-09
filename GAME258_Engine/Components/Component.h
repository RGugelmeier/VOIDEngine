#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

//This class is only here so components can inherit from it making them a component. This allows components of different types to fit into the same container on a GameObject.
class Component
{
public:
	inline Component() : parent(nullptr) {};
	inline virtual ~Component() { parent = nullptr; };

	virtual void Update(const float deltaTime) = 0;
	virtual void OnCreate(GameObject* parent_) = 0;
	virtual void OnDestroy() = 0;
	virtual void Render() const = 0;

protected:
	GameObject* parent;
private:
	//bool hasPosition;
};

#endif // !COMPONENT_H