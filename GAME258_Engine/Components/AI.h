#ifndef AI_H
#define AI_H

#include "Component.h"
#include "../Engine/Rendering/3D/GameObject.h"
#include "../Physics/Physics.h"
#include "../Engine/Math/Graph.h"
#include "../Components/Grid.h"

#include <glm/glm.hpp>
#include <queue>

using namespace glm;

//Used in all steering algorithms as output for the AI's movement.
struct SteeringOutput
{
	vec3 linear;
	float angular;
};

//Used in pathfinding algorithms. Nodes represent nodes on a graph used for pathfinding.
//class Nodee
//{
//	int label;
//	inline Nodee(int label_) { label = label_; }
//};

//Pairs a node with a priority to be used in a priority queue for pathfinding.
class PriorityQNode
{
public:
	inline PriorityQNode(int node_, float priority_) { node = node_; priority = priority_; }
	int node;
	float priority;

};

class Path
{
public:
	//Stubs
	inline Path(vector<int> path_) { currentNode = 0; path = path_; }
	inline ~Path() { ; }

	//Holds the last visited node.
	int currentNode;

	vector<int> path;
	void incrementNode(int incrementValue);
	vec3 getNextNodePos();
};

class AI : public Component
{
public:
	AI();
	~AI();

	virtual void OnCreate(GameObject* parent_);
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render() const;

	//Steering algorithms
	void Seek(vec3 target, float maxAccel);
	void Arrive(vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget);
	void Align(vec3 target, float MaxRotation, float targetRadius, float slowRadius, float timeToTarget);
	void Face(vec3 target);
	void KinWander(float maxSpeed, float maxRotation);
	void Wander(float wanderOffset, float wanderRadius, float wanderRate, float wanderOrientation, float maxAccel);
	void FollowAPath(Path* pathToFollow, Grid* gridToFollow, vec3 target, float maxSpeed, float maxAccel, float targetRadius, float slowRadius, float timeToTarget);

	//jlkasdhfuiyasdgfuytavbsdgufhjvKHJBASDUFYIVBASDUIYTFVGASYUTDFV HEY RYAN
	//THIS IS WHERE YOU CAN GO BACK TO
	//HEY GO BACK HERE
	//STOP HERE RYAN
	//HEY STOP

	//Pathfinding algorithms
	vector<int> AStar(Graph* graph, int start, int goal);

	inline void GetSteering() {  };
private:
	SteeringOutput* result;

	//Arrive variables
	float arriveDistance, targetSpeed;
	vec3 targetVel, direction;
};
#endif // !AI_H