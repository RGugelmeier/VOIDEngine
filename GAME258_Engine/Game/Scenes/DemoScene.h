#ifndef DEMOSCENE_H
#define DEMOSCENE_H

#include "../../Engine/Patterns/Container.h"
#include "../../Engine/Core/CoreEngine.h"
#include "../../Physics/Physics.h"
#include "../../Engine/Rendering/Skybox.h"

//Inherit scene.
class DemoScene : public Scene
{
public:
	DemoScene();
	virtual ~DemoScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;
private:
	SceneGraph* sceneInstance;

	Window* window;
	Skybox* citySkybox;

	Model* streetModel;
	Model* houseModel;
	Model* streetLightModel;
};

#endif // !DEMOSCENE_H