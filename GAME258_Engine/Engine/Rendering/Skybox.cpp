#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::Skybox(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ) : modelPtr(nullptr), skyboxTextureID(NULL)
{
}

Skybox::~Skybox()
{
	OnDestroy();
}

void Skybox::Update(const float deltaTime)
{
}

void Skybox::OnCreate()
{
	//Load the cube that the skybox will render onto. Make sure to use skybox shader for this model. This does not need to be sent to the scene graph because the camera handles it's rendering.
	modelPtr = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("skyboxShader"));

	//Load the textures of the skybox.
	bool status = LoadTextures("Resources/Textures/negx.jpg", "Resources/Textures/posx.jpg", "Resources/Textures/posy.jpg", "Resources/Textures/negy.jpg", "Resources/Textures/negz.jpg", "Resources/Textures/posz.jpg");

	//If the textures could not be loaded, print an error to the debug file.
	if (status == false)
	{
		Debug::FatalError("Skybox could not load textures.", "Skybox_h", __LINE__);
	}
}

void Skybox::OnDestroy()
{
	if (modelPtr)
	{
		delete modelPtr;
	}
}

void Skybox::Render() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	modelPtr->SkyboxRender(CoreEngine::GetInstance()->GetCamera());
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

bool Skybox::LoadTextures(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ)
{
	//Generate textures and store them into skyboxTextureID
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

	//Positive X
	SDL_Surface* textureSurface = IMG_Load(posX);
	if (textureSurface == nullptr)
	{
		return false;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	//Positive y
	textureSurface = IMG_Load(posY);
	if (textureSurface == nullptr)
	{
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	//Positive z
	textureSurface = IMG_Load(posZ);
	if (textureSurface == nullptr)
	{
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	
	//Negative x
	textureSurface = IMG_Load(negX);
	if (textureSurface == nullptr)
	{
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	//Negative y
	textureSurface = IMG_Load(negY);
	if (textureSurface == nullptr)
	{
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	//Negative z
	textureSurface = IMG_Load(negZ);
	if (textureSurface == nullptr)
	{
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}
