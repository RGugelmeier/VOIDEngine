#include "AI.h"

AI::AI() : result(nullptr), arriveDistance(0.0f), targetSpeed(0.0f)
{
	result = new SteeringOutput();
}

AI::~AI()
{
	delete result;
	result = nullptr;

}

void AI::OnCreate(GameObject* parent_)
{
	parent = parent_;
}

void AI::OnDestroy()
{
}

void AI::Update(const float deltaTime_)
{
}

void AI::Render() const
{
}

//Call to get a character with AI to seek out a target.
void AI::Seek(vec3 target, float maxAccel)
{
	//Get the direction from the NPC to the target.
	result->linear = target - parent->position;

	//Normalize the direction to make it a direction vector.
	result->linear = normalize(result->linear);
	//Apply force to the NPC.
	parent->GetComponent<Physics>()->ApplyForce(result->linear);

	result->angular = 0;
}

//Call to get the character with AI to approach and arrive at a target.
void AI::Arrive(vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget)
{
	//Get the direction from the NPC to the target.
	result->linear = target - parent->position;

	//Get how far away the NPC is from the target.
	arriveDistance = glm::length(result->linear);

	//Check if we are inside the arrive radius and stop if we are...
	if (arriveDistance < targetRadius)
	{
		parent->GetComponent<Physics>()->SetVel(vec3(0.0f, 0.0f, 0.0f));
		return;
	}

	//...if we are, check if we are not in the slow radius and use max speed if we are not...
	if (arriveDistance > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	//...if we are, use slowed down speed.
	else
	{
		targetSpeed = maxSpeed * arriveDistance / slowRadius;
	}

	//Set the target velocity to be in the direction from the NPC to the target...
	targetVel = normalize(result->linear);
	//...then set the speed of the NPC.
	targetVel *= targetSpeed;

	//
	result->linear = targetVel - parent->GetComponent<Physics>()->GetVel();
	result->linear /= timeToTarget;

	if (arriveDistance > maxAccel)
	{
		result->linear = normalize(result->linear);
		result->linear *= maxAccel;
	}

	parent->GetComponent<Physics>()->ApplyForce(result->linear);

	result->angular = 0;
}

void AI::Align(vec3 targetOrientation, float MaxRotation, float targetRadius, float slowRadius, float timeToTarget)
{
	//set the rotation by a quaternion and change the quaternion to quaternion every tick.
	//apply a change to the angle by the orientation variable. Idk man this is hard.
	
}

void AI::Face(vec3 target)
{

}

void AI::KinWander(float maxSpeed, float maxRotation)
{

}

void AI::Wander(float wanderOffset, float wanderRadius, float wanderRate, float wanderOrientation, float maxAccel)
{

}

void AI::FollowAPath(Path* pathToFollow, Grid* gridToFollow, vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget)
{
	//If the path has not been calculated, print a debug error.
	if (!pathToFollow)
	{
		Debug::Error(parent->GetTag() + " tried to follow a path, but the path was never created. Call AI::AStar() to calculate a path", "AI.cpp", __LINE__);
		return;
	}

	vec3 targetPos;

	
}

//BEGIN PATHFINDING ALGORITHMS//

//Struct used to compare priortiy queue weights for pathfinding.
struct ComparePriority
{
	bool operator()(PriorityQNode& leftHandSide, PriorityQNode& rightHandSide)
	{
		return leftHandSide.priority > rightHandSide.priority;
	}
};

//A* pathfinding. This will find a path from one node to another on a given grid and tell the AI to move from the startNode (usually the current position) to the goalNode.
vector<int> AI::AStar(Graph* graph, int startNode, int goalNode)
{
	//Create a vec3 to hold the path finder's current node.
	float newCost;
	int currentNode;

	//Declare current node we will be expanding and use it to initialize the frontier.
	PriorityQNode* currentNodeAndPriority;
	currentNodeAndPriority = new PriorityQNode(startNode, 0.0f);

	//Set up the frontier as a priority queue of nodes.
	priority_queue<PriorityQNode, deque<PriorityQNode>, ComparePriority> frontier;
	//priority_queue<PriorityQNode> frontier;
	frontier.push(*currentNodeAndPriority);

	//Create a container to hold the previously visited node so the AI can follow the path later.
	vector<int> cameFrom;
	cameFrom.resize(graph->numNodes());

	//Create a container to hold the cost so far at each node.
	map<int, float> costSoFar;
	costSoFar[startNode] = 0.0f;

	//!Enter pathfinding loop!//
	//While the frontier is not empty.
	while (frontier.empty() == false)
	{
		//Set the current to to be the top node of the frontier.
		currentNode = frontier.top().node;
		frontier.pop();

		//Check to see if the path is finished, and break out of the while loop. We're done!
		if (currentNode == goalNode)
		{
			break;
		}

		//Loop through all neighbours of the current node
		for (auto next : graph->neighbours(currentNode))
		{
			newCost = costSoFar[currentNode] + graph->GetCost(currentNode, next);
			//Check if the next node being checked is not in the costSoFar map by checking if the next node's value is a value inside the map, or if the newCost is less than the costSoFar of the next node.
			//
			//find_if searches a list and returns an iterator to the found value if found, and an iterator to the end of the container if not found.
			//Here we use this to see if the next node is not in the costSoFar by checking if find_if returns an iterator to the end of the container costSoFar.
			if (find_if(costSoFar.begin(), costSoFar.end(), [next](const auto& mo) {return mo.second == next; }) == costSoFar.end() || newCost < costSoFar[next])
			{
				costSoFar[next] = newCost;
				currentNodeAndPriority->priority = newCost + graph->heuristic(next, goalNode);

				//Create a new Priortiy Queue Node to add to be filled with data and added to the frontier.
				PriorityQNode* nodeAndPriorityToAdd;
				//Fill it with data.
				nodeAndPriorityToAdd = new PriorityQNode(next, currentNodeAndPriority->priority);
				//Add it to the frontier.
				frontier.push(*nodeAndPriorityToAdd); //TODO Clean this up somehow. Maybe use smart pointers so when the AI object gets deleted, these delet as well?
													  //I can't just delete them in the deconstructor because frontier only exists in this function and I need to loop through it to delete all the nodes.

				cameFrom[next] = currentNode;
			}
		}
	}
	
	//Figure out the path.
	vector<int> path;
	int i = goalNode;
	path.push_back(goalNode);
	cout << "Path from " << startNode << "to " << goalNode << ":\n";
	while (i != startNode)
	{
		cout << cameFrom[i] << "\n";
		path.push_back(cameFrom[i]);
		i = cameFrom[i];
	}

	return path;
}

void Path::incrementNode(int incrementValue)
{
	currentNode += incrementValue;
}

//This function will get the position of the next tile to go to by getting the tile's node number and matching it to the one we are looking for.
//vec3 Path::getNextNodePos()
//{
//	//Set target to be the position of the next node in the path.
//	//for (int i = 0; i < path_.size(); i++)
//	//{
//	//
//	//	if (gridToFollow->tiles[i].node == lastVisitedNode)
//	//	{
//	//		gridToFollow->GetTileByNum(i)->pos
//	//	}
//	//}
//}
