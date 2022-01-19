#ifndef SKYBOX_H
#define SKYBOX_H

#include <SDL_image.h>

#include "../Graphics/ShaderHandler.h"
#include "3D/Mesh.h"
#include "3D/Model.h"
#include "../../Engine/Rendering/SceneGraph.h"
#include "../Core/CoreEngine.h"

class Skybox
{
public:
	Skybox();
	Skybox(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ);
	~Skybox();

	void Update(const float deltaTime);
	void OnCreate();
	void OnDestroy();
	void Render() const;
	GLuint skyboxTextureID;
private:
	Model* modelPtr;
	
	bool LoadTextures(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ);
};

#endif // !SKYBOX_H