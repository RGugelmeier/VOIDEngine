#ifndef GRAPH_H
#define GRAPH_H

#include <queue>
#include <vector>
#include <list>
#include <map>

using namespace std;

//Templated node struct. This is so the node can hold any type of data.
//For example for 3D pathfinding the data type will be a vec3 to hold the position of the node.
template<typename T>
struct Node
{
	T data;
};

class Graph
{
private:
	//2D Matrix to represent all nodes.
	vector<vector<float>> nodes;
public:
	Graph(int numNodes);
	~Graph();
	int numNodes();
	void clearWeights();
	void addWeightedConnection(int fromNode, int toNode, float weight);
	list<int> neighbours(int fromNode);
	float heuristic(int node, int goal);
	float GetCost(int from, int to);
	vector<vector<float>> GetNodes();
};

#endif // !GRAPH_H