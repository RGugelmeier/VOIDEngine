#ifndef HOUSESCENE_H
#define HOUSESCENE_H

#include "../../Engine/Patterns/Container.h"
#include "../../Engine/Core/CoreEngine.h"
#include "../../Physics/Physics.h"
#include "../../Engine/Rendering/Skybox.h"

//Inherit scene.
class HouseScene : public Scene
{
public:
	HouseScene();
	virtual ~HouseScene();

	bool OnCreate() override;
	void Update(const float deltaTime_) override;
	void Render() override;
private:
	SceneGraph* sceneInstance;
};

#endif // !HOUSESCENE_H