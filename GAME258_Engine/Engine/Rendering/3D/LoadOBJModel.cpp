#include "LoadOBJModel.h"

LoadOBJModel::LoadOBJModel() : vertices(vector<vec3>()), normals(vector<vec3>()), textureCoords(vector<vec2>()), indices(vector<unsigned int>()), meshVertices(vector<Vertex>()), subMeshes(vector<SubMesh>()), currentMaterial(Material())
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
	mesh.material = currentMaterial;

	subMeshes.push_back(mesh);

	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();

	currentMaterial = Material();
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
	//Give initial value to the bounding box min and max.
	boundingBox.maxVert = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	boundingBox.minVert = vec3(FLT_MAX, FLT_MAX, FLT_MAX);

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

			//Setup bounding box size for the OBJ.
			if (x > boundingBox.maxVert.x)
			{
				boundingBox.maxVert.x = x;
			}
			else if (x < boundingBox.minVert.x)
			{
				boundingBox.minVert.x = x;
			}

			if (y > boundingBox.maxVert.y)
			{
				boundingBox.maxVert.y = y;
			}
			else if (y < boundingBox.minVert.y)
			{
				boundingBox.minVert.y = y;
			}

			if (z > boundingBox.maxVert.z)
			{
				boundingBox.maxVert.z = z;
			}
			else if (z < boundingBox.minVert.z)
			{
				boundingBox.minVert.z = z;
			}
			//end bounding box size setup.
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

		//TODO Use char to read in the slashes and ignore them rather than sstream ignore.
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

//Get the material from the material handler and set it to currentMaterial.
void LoadOBJModel::LoadMaterial(const string& matName_)
{
	currentMaterial = MaterialHandler::GetInstance()->GetMaterial(matName_);
}

//Run the material loader's LoadMaterial which loads the material.
void LoadOBJModel::LoadMaterialLibrary(const string& matFilePath_)
{
	MaterialLoader::LoadMaterial(matFilePath_);
}
