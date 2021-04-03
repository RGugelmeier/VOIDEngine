#ifndef MATERIALHANDLER_H
#define MATERIALHANDLER_H

#include <glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

#include "../Core/Debug.h"

using namespace glm;
using namespace std;

//Default values set for material struct.
struct Material
{
	GLuint diffuseMap = 0;		//newmtl 

	float shininess = 32.0f;	//Ns
	float transparency = 1.0f;	//d

	vec3 ambient = vec3(1.0f);		//Ka
	vec3 diffuse = vec3(1.0f);		//Kd
	vec3 specular = vec3(1.0f);		//Ks

	string name = "";
};

class MaterialHandler
{
public:
	MaterialHandler(const MaterialHandler&) = delete;
	MaterialHandler(MaterialHandler&&) = delete;
	MaterialHandler& operator=(const MaterialHandler&) = delete;
	MaterialHandler& operator=(MaterialHandler&&) = delete;

	static MaterialHandler* GetInstance();
	void AddMaterial(Material mat_);
	Material GetMaterial(const string& matName_) const;
	void OnDestroy();
private:
	MaterialHandler();
	~MaterialHandler();

	static unique_ptr<MaterialHandler> materialInstance;
	friend default_delete<MaterialHandler>;

	static vector<Material> materials;
};
#endif // !MATERIALHANDLER_H