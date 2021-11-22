#include "Graph.h"

Graph::Graph(int numNodes)
{
	//Initialize matrix of nodes to have correct number of nodes.
	nodes.resize(numNodes);

	//Initialize all nodes to have no connections. (each node's weight is 0.0)
	for (int i = 0; i < numNodes; i++)
	{
		nodes[i].resize(numNodes);
		for (int j = 0; j < numNodes; j++)
		{
			nodes[i][j] = 0.0f;
		}
	}
}

Graph::~Graph()
{
}

//Return amount of nodes in graph.
int Graph::numNodes()
{
	return nodes.size();
}

//Resets all weights to be 0 so weights can be recalculated if need be.
void Graph::clearWeights()
{
	for (unsigned i = 0; i < nodes.size(); i++)
	{
		for (unsigned j = 0; j < nodes.size(); j++)
		{
			nodes[i][j] = 0.0f;
		}
	}
}

//Adds a new weighted connection to the graph.
void Graph::addWeightedConnection(int fromNode, int toNode, float weight)
{
	nodes[fromNode][toNode] = weight;
}

//Returns a list of nodes that neighbour the fromNode passed in as an argument.
list<int> Graph::neighbours(int fromNode)
{
	//Create neighbour list.
	list<int> result = {};
	//Create vector that will represent all other nodes in the graph.
	vector<float> toNodes = nodes[fromNode];
	//Loop through all other nodes in the same row to see if the weight is greater than 0.0f. If it is, there is a connection and that node is a neighbour.
	for (unsigned j = 0; j < toNodes.size(); j++)
	{
		if (nodes[fromNode][j] > 0.0f)
		{
			result.push_back(j);
		}
	}
	return result;
}

float Graph::heuristic(int node, int goal)
{
	return abs(node - goal);
}

float Graph::GetCost(int from, int to)
{
	return nodes[from][to];
}

vector<vector<float>> Graph::GetNodes()
{
	return nodes;
}
