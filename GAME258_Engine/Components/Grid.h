#ifndef GRID_H
#define GRID_H

#include "Component.h"
#include "../Engine/Rendering/3D/GameObject.h"
#include "../Engine/Math/Graph.h"

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

class Tile
{
public:
	Tile(int node, float tileSize, vec3 pos_);
	~Tile();

	int node;
	float tileSize;
	vec3 pos;
};

class Grid : public Component
{
public:
	Grid();
	Grid(const float tileSize_, const vec3 numTiles_);
	~Grid();

	virtual void OnCreate(GameObject* parent_);
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render() const;

	inline Graph* getGraph() { return pathfindingGraph; }
	Tile* GetTileByNum(int tileToGet);

	vector<vector<vector <Tile*>>> tiles;
private:
	//Vec3 to hold the size of the grid's tiles and the amount of tiles in the grid.
	float tileSize;
	vec3 gridSize;

	//x, y, and z
	int rows, layers, cols;

	Graph* pathfindingGraph;
};

#endif // !GRID_H