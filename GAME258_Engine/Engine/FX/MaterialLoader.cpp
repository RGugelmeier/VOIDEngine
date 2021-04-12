#include "MaterialLoader.h"

MaterialLoader::~MaterialLoader()
{
}

void MaterialLoader::LoadMaterial(string filePath_)
{
	//Try to open mtl file and error check.
	ifstream in(filePath_.c_str(), ios::in);
	if (!in)
	{
		Debug::Error("Cannot open MTL file: " + filePath_, "MaterialLoader.cpp", __LINE__);
		return;
	}

	//Cretae new material object.
	Material m = Material();
	string matName = "";
	string line;
	//Read through file...
	while (getline(in, line))
	{
		//...until it reads "newmtl "
		if (line.substr(0, 7) == "newmtl ")
		{
			//Then check if a material ahs already been created (for mtl files with multiple materials in them) and set up to make a new one if true.
			if (m.diffuseMap != 0)
			{
				MaterialHandler::GetInstance()->AddMaterial(m);
				m = Material();
			}
			//Run LoadTexture on the texture's name and set return to the material's diffuseMap.
			//Then set the material's name for later use.
			matName = line.substr(7);
			m.diffuseMap = LoadTexture(matName);
			m.name = matName;
		}
		//Same as above but for diffuse data.
		else if (line.substr(0, 4) == "\tKd ")
		{
			stringstream kd(line.substr(4));
			float x, y, z;
			kd >> x >> y >> z;
			m.diffuse = vec3(x, y, z);
		}
		//Specular data.
		else if (line.substr(0, 4) == "\tKs ")
		{
			stringstream ks(line.substr(4));
			float x, y, z;
			ks >> x >> y >> z;
			m.specular = vec3(x, y, z);
		}
		//Ambient data.
		else if (line.substr(0, 4) == "\tKa ")
		{
			stringstream ka(line.substr(4));
			float x, y, z;
			ka >> x >> y >> z;
			m.ambient = vec3(x, y, z);
		}
		//Transparency data.
		else if (line.substr(0, 3) == "\td ")
		{
			stringstream d(line.substr(3));
			float transparency_;
			d >> transparency_;
			m.transparency = transparency_;
		}
		//Shininess data.
		else if (line.substr(0, 2) == "\tNs")
		{
			stringstream ns(line.substr(0, 4));
			float shininess_;
			ns >> shininess_;
			m.shininess = shininess_;
		}
	}

	//Add the material to the materials list if nothing went wrong.
	if (m.diffuseMap != 0)
	{
		MaterialHandler::GetInstance()->AddMaterial(m);
	}

	//Close the file.
	in.close();
}

//Loads in and returns the given texture. The texture's file must be in the written directory.
GLuint MaterialLoader::LoadTexture(string fileName_)
{
	GLuint currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);
	if (currentTexture == 0)
	{
		//Hard coded file path. File must be in this directory to be found and used.
		TextureHandler::GetInstance()->CreateTexture(fileName_, "./Resources/Textures/" + fileName_ + ".png");
		currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);
	}
	return currentTexture;
}
