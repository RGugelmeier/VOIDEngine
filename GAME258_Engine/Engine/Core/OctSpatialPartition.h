//TODO Add a function that allows deletion of gameobject from a cell, and functionality to move an object from one cell to another.
//These are needed to update the spatialpartition system. For example if an object moves from one cell to the other, the system will not know...
//and it can cause the collision to not be detected properly.

#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../Math/Ray.h"
#include "../Rendering/3D/GameObject.h"

//Represents how many children a OctNode can have.
constexpr auto CHILDREN_NUMBER = 8;

//Represents each child node.
/*
* T = TOP
* R = RIGHT / REAR
* B = BOTTOM
* L = LEFT
*/
enum class OctChildren
{
	OCT_TLF,
	OCT_BLF,
	OCT_BRF,
	OCT_TRF,
	OCT_TLR,
	OCT_BLR,
	OCT_BRR,
	OCT_TRR,
};

//Represent a node in an octree.
class OctNode
{
public:
	OctNode(vec3 position_, float size_, OctNode* parent_);
	~OctNode();
	void Octify(int depth_);
	void AddCollisionObject(GameObject* obj_);

	//Getters
	inline OctNode* GetChild(OctChildren childPos_) { return children[static_cast<int>(childPos_)]; }
	inline OctNode* GetParent() { return parent; }
	inline int GetObjectCount() const { return objectList.size(); }
	inline BoundingBox* GetBoundingBox() const { return octBounds; }
	inline int GetChildCount() const { return childNum; }
	inline vector<GameObject*> GetCollidedObjects() const { return objectList; }
	//Return if the node is a leaf (has no children) or not.
	bool IsLeaf() const;

	
private:
	//This lets OctSpatialPartition access OctNode's private variables.
	friend class OctSpatialPartition;

	OctNode* children[CHILDREN_NUMBER];
	BoundingBox* octBounds;
	OctNode* parent;
	vector<GameObject*> objectList;
	float size;
	static int childNum;
};

//Contains functionality to use the spatial partition with objects.
class OctSpatialPartition {
public:
	OctSpatialPartition(float worldSize_, int timesToOctify_);
	~OctSpatialPartition();
	void AddObject(GameObject* obj_);
	GameObject* GetCollision(Ray ray_);
	inline const vector<OctNode*> GetLeafNodes() { return leafNodes; }
	void RemoveObjectFromCell(GameObject* obj_);
private:
	OctNode* root;
	vector<OctNode*> rayIntersectionList;
	vector<OctNode*> leafNodes;
	void AddObjectToCell(OctNode* cell_, GameObject* obj_);
	void PrepareCollisionQuery(OctNode* cell_, Ray ray_);
	void FillLeafList(OctNode* cell_);
};

#endif // !OCTSPATIALPARTITION_H