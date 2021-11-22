#include "Grid.h"

Grid::Grid() : tileSize(0.0f), gridSize(vec3()), rows(0), cols(0), layers(0)
{
}

Grid::Grid(float tileSize_, vec3 gridSize_) : tileSize(tileSize_), gridSize(gridSize_), rows(0), cols(0), layers(0)
{
	//Reserve space in the tiles vector to hold all of the tiles.
	//tiles.reserve(sizeof(numTiles * (float)sizeof(Tile)));
}

Grid::~Grid()
{
	for (auto vec : tiles)
	{
		for (auto vec : vec)
		{
			for (auto tile : vec)
			{
				delete tile;
				tile = nullptr;
			}
		}
	}

	if (pathfindingGraph)
	{
		//delete pathfindingGraph;
		//pathfindingGraph = nullptr;
	}
}

//Setup tiles at the correct positions
void Grid::OnCreate(GameObject* parent_)
{
	parent = parent_;

	cols = static_cast<int>(gridSize.x / tileSize);
	layers = static_cast<int>(gridSize.y / tileSize);
	rows = static_cast<int>(gridSize.z / tileSize);

	tiles.resize(rows);
	for (int i = 0; i < rows; i++)
	{
		tiles[i].resize(cols);
		for (int j = 0; j < cols; j++)
		{
			tiles[i][j].resize(layers);
		}
	}

	int i, j, k, node;
	i = 0;
	j = 0;
	k = 0;
	node = 0;

	//Rows
	for (float z = tileSize / 2; z < gridSize.y; z += tileSize)
	{
		//Layers
		for (float y = tileSize / 2; y < gridSize.x; y += tileSize)
		{
			//Collums
			for (float x = tileSize / 2; x < gridSize.z; x += tileSize)
			{
				Tile* t;
				vec3 tilePos = vec3(x, y, z);
				t = new Tile(node, tileSize, parent->position + tilePos);
				tiles[i][j][k] = t;
				k++;
				node++;
			}
			k = 0;
			j++;
		}
		j = 0;
		i++;
	}

	//Claculate weights for graph
	pathfindingGraph = new Graph(rows * cols * layers);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			for (int k = 0; k < layers; k++)
			{
				//Connections for rows
				if (i > 0)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i - 1][j][k]->node, tileSize);
				}
				if (i < rows - 1)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i + 1][j][k]->node, tileSize);
				}

				//Connections for cols
				if (j > 0)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i][j - 1][k]->node, tileSize);
				}
				if (j < cols - 1)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i][j + 1][k]->node, tileSize);
				}

				//Connections for layers
				if (k > 0)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i][j][k - 1]->node, tileSize);
				}
				if (k < layers - 1)
				{
					pathfindingGraph->addWeightedConnection(tiles[i][j][k]->node, tiles[i][j][k + 1]->node, tileSize);
				}
			}
			k = 0;
		}
		j = 0;
	}

	for (auto tilee : tiles)
	{
		for (auto tile : tilee)
		{
			for (auto tileee : tile)
			{
				cout << "Tile with node number: " << tileee->node << " at: " << tileee->pos.x << " " << tileee->pos.y << " " << tileee->pos.z << "\n";
			}
		}
	}
}

//Clean up
void Grid::OnDestroy()
{
}

//Nothing to update
void Grid::Update(const float deltaTime_)
{
}

//Nothing to render
void Grid::Render() const
{
}

Tile* Grid::GetTileByNum(int tileToGet)
{
	for (auto i : tiles)
	{
		for (auto j : i)
		{
			for (auto k : j)
			{
				if (k->node == tileToGet)
				{
					return k;
				}
			}
		}
	}
}

Tile::Tile(int node_, float tileSize_, vec3 pos_)
{
	node = node_;
	tileSize = tileSize_;
	pos = pos_;
}

Tile::~Tile()
{
}
