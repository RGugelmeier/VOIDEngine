#include "TextureHandler.h"

//Redeclare static variables and set default values.
//Ask about this. Remind why we redeclare static variables?
std::unique_ptr<TextureHandler> TextureHandler::textureInstance = nullptr;
std::vector<Texture*> TextureHandler::textures = std::vector<Texture*>();

//Reserve space for the vector. Makes data insertion quicker once the space has already been reserved.
TextureHandler::TextureHandler()
{
	textures.reserve(10);
}

//Call on destroy.
TextureHandler::~TextureHandler()
{
	OnDestroy();
}

//Gets the instance of the texture. If this returns null, the testure has not been created yet so call reset to create it.
TextureHandler* TextureHandler::GetInstance()
{
	if (textureInstance.get() == nullptr)
	{
		textureInstance.reset(new TextureHandler);
	}
	return textureInstance.get();
}

//Clean up. If the vector has anything in it, delete the textures in the vector by using a GL function, and frees up the space used.
void TextureHandler::OnDestroy()
{
	if (textures.size() > 0)
	{
		for (auto t : textures)
		{
			glDeleteTextures(1, &t->textureID);
			delete t;
			t = nullptr;
		}
		textures.clear();
	}
}

void TextureHandler::CreateTexture(const std::string& textureName_, const std::string& textureFilePath_)
{
	//Return if the texture has already been created. Textures only need to be created once and can be used multiple times.
	if (GetTextureData(textureName_))
	{
		return;
	}

	//Create the texture object.
	Texture* t = new Texture();
	//Create the surface to put the texture onto.
	SDL_Surface* surface = nullptr;
	//Try to load the texture onto the surface.
	surface = IMG_Load(textureFilePath_.c_str()); //Ask about this. Is there a downside to using this over load BMP?

	//If the surface fails to get created, print to debug and delete the texture that was created and stop the function.
	if (surface == nullptr)
	{
		Debug::Error("Surface for texture " + textureName_ + " failed to be created.", "TextureHandler.cpp", __LINE__);
		delete t;
		t = nullptr;
		return;
	}

	//Generate and bind the texture using gl functions. 1 is gthe number of textures to generate, then give it the adress of the ID of the texture so openGL puts it in that space when created.
	glGenTextures(1, &t->textureID);
	glBindTexture(GL_TEXTURE_2D, t->textureID);

	//Set what mode to use. RGBA or RGB depending on the bytes per pixel.
	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

	//Generate the 2D texture given all parameters about the texture.
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	//Set the texture wrapping options for s and t axes. Set to repeat.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Set the mipmap options for scaling up and down the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generate the mipmap.
	glGenerateMipmap(GL_TEXTURE_2D);
	//After setting all of the details about the texture, unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	//Set the width, height, and name of the texture structure.
	t->width = static_cast<float>(surface->w);
	t->width = static_cast<float>(surface->h);
	t->textureName = textureName_;

	//Push the texture onto the textures vector.
	textures.push_back(t);

	//Clear up the surface and then set it to null.
	SDL_FreeSurface(surface);
	surface = nullptr;
}

//Get the texture id by the texture name that is passed in. Return 0 if the texture we're looking for deos not exist.
const GLuint TextureHandler::GetTexture(const std::string& textureName_)
{
	for (auto t : textures)
	{
		if (t->textureName == textureName_)
		{
			return t->textureID;
		}
	}
	return 0;
}

//Get the texture itself for use of all texture data by the texture name that is passed in. Return null if the texture we're looking for does not exist.
const Texture* TextureHandler::GetTextureData(const std::string textureName_)
{
	for (auto t : textures)
	{
		if (t->textureName == textureName_)
		{
			return t;
		}
	}
	return nullptr;
}
