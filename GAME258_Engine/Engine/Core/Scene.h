#ifndef SCENE_H
#define SCENE_H

//Abstract class used for inheriting from when creating a scene.
class Scene {
public:
	//Destructor is virtual because...
	Scene() {}
	virtual ~Scene() {}

	//All member functions are virtual and = 0. This makes it so when a class that inherits from this class tries to use these functions, it MUST define these classes.
	virtual bool OnCreate() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render() = 0;

	bool canZoom = false;
};

#endif // !SCENE_H