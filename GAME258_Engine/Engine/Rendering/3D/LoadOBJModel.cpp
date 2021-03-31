#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel() : vertices(vector<vec3>()), normals(vector<vec3>()), textureCoords(vector<vec2>()), indices(vector<unsigned int>()), meshVertices(vector<Vertex>()), subMeshes(vector<SubMesh>()), currentTexture(0)
{
	vertices.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	normalIndices.reserve(200);
	textureIndices.reserve(200);
	meshVertices.reserve(200);
	subMeshes.reserve(10);
}

LoadOBJModel::~LoadOBJModel()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	subMeshes.clear();
}

//Load in both the material library file and the obj file.
void LoadOBJModel::LoadModel(const string& objFilePath_, const string& mtlFilePath_)
{
	LoadMaterialLibrary(mtlFilePath_);
	LoadModel(objFilePath_);
}

//subMeshes list getter.
vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

//Create all vertices by looping for every index and setting all vertex properties. Set all vertices into a sub mesh, and clear some used lists.
//Does not clear all because a of how an obj file is formatted. 
//This is all done after everything is loaded in for a mesh..
void LoadOBJModel::PostProcessing()
{
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.textureCoordinates = textureCoords[textureIndices[i]];
		meshVertices.push_back(vert);
	}

	SubMesh mesh;
	mesh.vertexList = meshVertices;
	mesh.meshIndicies = indices;
	mesh.textureID = currentTexture;

	subMeshes.push_back(mesh);

	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();

	currentTexture = 0;
}

//This actually loads in the OBJ file.
void LoadOBJModel::LoadModel(const string& filePath_)
{
	ifstream in(filePath_.c_str(), ios::in);
	//Error check if file can be found.
	if (!in)
	{
		Debug::Error("Cannot open OBJ file: " + filePath_, "LoadPBJFile.cpp", __LINE__);
		return;
	}

	string line;

	//Read each line.
	while (getline(in, line))
	{
		//Read vertex data. "v" = vertex in obj file.
		if (line.substr(0, 2) == "v ")
		{
			stringstream v(line.substr(2));
			float x, y, z;
			//Set the first float it can find and put into x, second float into y, and third float into z.
			v >> x >> y >> z;
			//Push vertex positon to vertices list.
			vertices.push_back(vec3(x, y, z));
		}

		//Read normal data. "vn" == vertex normal in obj file.
		else if (line.substr(0, 3) == "vn ")
		{
			stringstream vn(line.substr(3));
			float x, y, z;
			//Set the first float it can find and put into x, second float into y, and third float into z.
			vn >> x >> y >> z;
			//Push normal into normals list.
			normals.push_back(vec3(x, y, z));
		}

		//Read texture data. "vt" = vertex texture in obj file.
		else if (line.substr(0, 3) == "vt ")
		{
			stringstream vt(line.substr(3));
			float x, y;
			//Set the first float it can find and put into x, and second float into y.
			vt >> x >> y;
			//Push texture data into textureCoords list.
			textureCoords.push_back(vec2(x, y));
		}

		else if (line.substr(0, 2) == "f ")
		{
			stringstream f(line.substr(2));

			unsigned int vertexOne[3], vertexTwo[3], vertexThree[3];
			//Set face values to get.
			for (int i = 0; i < 3; i++)
			{
				f >> vertexOne[i];
				vertexOne[i] -= 1;
				f.ignore(1, vertexOne[i]);
			}
			for (int i = 0; i < 3; i++)
			{
				f >> vertexTwo[i];
				vertexTwo[i] -= 1;
				f.ignore(1, vertexTwo[i]);
			}
			for (int i = 0; i < 3; i++)
			{
				f >> vertexThree[i];
				vertexThree[i] -= 1;
				f.ignore(1, vertexThree[i]);
			}
			//Push back all face data.
			indices.push_back(vertexOne[0]);	textureIndices.push_back(vertexOne[1]);		normalIndices.push_back(vertexOne[2]);
			indices.push_back(vertexTwo[0]);	textureIndices.push_back(vertexTwo[1]);		normalIndices.push_back(vertexTwo[2]);
			indices.push_back(vertexThree[0]);	textureIndices.push_back(vertexThree[1]);	normalIndices.push_back(vertexThree[2]);
		}

		//New mesh to load. "usemtl " = new mesh will follow.
		else if (line.substr(0, 7) == "usemtl ")
		{
			//Makes sure we call post processing only if there has been a full mesh loaded.
			if (indices.size() > 0)
			{
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}
	}
	//Call post processing because meshes are now loaded.
	in.close();
	PostProcessing();
}

//If the given texture does not exist yet, load the given matName_ into the currentTexture.
//MAKE SURE EVERY TEXTURE IS A PNG. FILE PATH IS HARD CODED.
void LoadOBJModel::LoadMaterial(const string& matName_)
{
	currentTexture = TextureHandler::GetInstance()->GetTexture(matName_);
	if (currentTexture == 0)
	{
		TextureHandler::GetInstance()->CreateTexture(matName_, "Resources/Textures/" + matName_ + ".png");
		currentTexture = TextureHandler::GetInstance()->GetTexture(matName_);
	}
}

//Read the mtl file and run LoadMaterial on the line that contains the material's name.
void LoadOBJModel::LoadMaterialLibrary(const string& matFilePath_)
{
	ifstream in(matFilePath_.c_str(), ios::in);

	//Error check.
	if (!in)
	{
		Debug::Error("Cannot open MTL file: " + matFilePath_, "LoadOBJModel.cpp", __LINE__);
		return;
	}

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 7) == "newmtl ")
		{
			LoadMaterial(line.substr(7));
		}
	}
	in.close();
}
