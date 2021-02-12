#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model();
	~Model();

	void Render();
	void AddMesh(Mesh* mesh_);

private:
	vector<Mesh*> meshes;
};

#endif // !MODEL_H