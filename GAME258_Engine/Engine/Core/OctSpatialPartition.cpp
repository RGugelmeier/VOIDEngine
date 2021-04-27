#include "OctSpatialPartition.h"

//Redeclare and define childNum.
int OctNode::childNum = 0;

//Initialise values.
OctNode::OctNode(vec3 position_, float size_, OctNode* parent_) : octBounds(nullptr), parent(nullptr), children(), objectList(vector<GameObject*>())
{
	//Change this if there will be more objects in a scene.
	objectList.reserve(10);

	//set the min and max for the bounding box of this node.
	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + vec3(size_);

	size = size_;
	parent = parent_;
	
	//Initialise all children to be null. Stays this way if the node is a leaf.
	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		children[i] = nullptr;
	}
}

//Clean up.
OctNode::~OctNode()
{
	//Delete the bounding box.
	delete octBounds;
	octBounds = nullptr;

	//Clear up the objectList list. The actualy game objects will be deleted by the SceneGraph.
	if (objectList.size() > 0)
	{
		for (auto obj : objectList)
		{
			obj = nullptr;
		}
		objectList.clear();
	}

	//Clear up the children list.
	for (int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if (children[i] != nullptr)
		{
			delete children[i];
		}
		children[i] = nullptr;
	}
}

//Recursive function. Calls itself to create children nodes in an OctNode. Creates as many OctNodes as requested by the depth_ variable passed into Octify().
void OctNode::Octify(int depth_)
{
	//If we are not at the bottom (depth_) of the OctNode tree...
	if (depth_ > 0 && this)
	{
		float half = size / 2.0f;
		//Create all children.
		children[static_cast<int>(OctChildren::OCT_TLF)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);
		children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode(vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);
		children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode(vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		//Keep track of how many children have been created.
		childNum += 8;
	}

	//If we are not at the bottom (depth_) of the OctNode tree...
	if (depth_ > 0 && this)
	{
		//Loop through each child and octify the next node, until we have reached the depth requested on first Octify() call.
		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			children[i]->Octify(depth_ -1);
		}
	}
}

//Add an object to the object list.
void OctNode::AddCollisionObject(GameObject* obj_)
{
	objectList.push_back(obj_);
}

bool OctNode::IsLeaf() const
{
	if (children[0] == nullptr)
	{
		return true;
	}
	return false;
}

//Initialise partitioning values.
OctSpatialPartition::OctSpatialPartition(float worldSize_) : root(nullptr), rayIntersectionList(vector<OctNode*>())
{
	//Create a root node with the size passed in for the world.
	//The way this root is created, it puts the position (bottom back left corner) at the correct spot.
	//This makes the middle of the root node the middle of the world properly.
	root = new OctNode(vec3(-(worldSize_ / 2.0f)), worldSize_, nullptr);
	//Octify the root node 3 times.
	root->Octify(3);
	cout << "There are " << root->GetChildCount() << " child nodes." << endl;

	//Reserve space for list.
	rayIntersectionList.reserve(20);
}

//Clean up.
OctSpatialPartition::~OctSpatialPartition()
{
	//If there are intersectiuons stored in the intersection list, nullify the pointers used and clear the list.
	//The game objects they use are deleted in SceneGraph.
	if (rayIntersectionList.size() > 0)
	{
		for (auto cell : rayIntersectionList)
		{
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}

	//Clean up root node space. When the root is deleted, it deletes all of it's children.
	delete root;
	root = nullptr;
}

//Adds the object to 
void OctSpatialPartition::AddObject(GameObject* obj_)
{
	AddObjectToCell(root, obj_);
}

GameObject* OctSpatialPartition::GetCollision(Ray ray_)
{
	//Cleans out the intersection list if it is not already empty. This is so we can store any new intersections in here.
	if (rayIntersectionList.size() > 0)
	{
		for (auto cell : rayIntersectionList)
		{
			cell = nullptr;
		}
		rayIntersectionList.clear();
	}

	//Refill ray intersection list with cells that the ray has collided with.
	PrepareCollisionQuery(root, ray_);

	//--BEGIN COLLISION DETECTION--//
	//Create a gameobject to hold the collided game object.
	GameObject* result = nullptr;
	//Set the shortest distance to be maximum float.
	float shortestDistance = FLT_MAX;
	//For every cell the ray collided wit...
	for (auto cell : rayIntersectionList)
	{
		//...check each object in the cell...
		for (auto obj : cell->objectList)
		{
			//...and check to see if the ray has collided with any bounding boxes.
			BoundingBox tmp = obj->GetBoundingBox();
			if (ray_.IsColliding(&tmp))
			{
				//Check if the bounding box is closer than a previously collided with bounding box in this same check. If it has, this is now the closest object to the screen that has been collided with.
				if (ray_.intersectionDist < shortestDistance)
				{
					result = obj;
					shortestDistance = ray_.intersectionDist;
				}
			}
		}
		//Return the bounding box that was collided with if there was a collision.
		if (result != nullptr)
		{
			return result;
		}
		//Return nothing because there was no collision.
	}
	//--END COLLISION DETECTION--//
	return nullptr;
}

//Adds an object to a cell.
void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* obj_)
{
	//If the object's bounding box intersects the cell passed in...
	if (cell_->IsLeaf())
	{
		//...and the cell is a leaf...
		if (obj_->GetBoundingBox().Intersects(cell_->GetBoundingBox()))
		{
			//...add the object to the cell.
			cell_->AddCollisionObject(obj_);
			cout << obj_->GetTag() << " was added to a cell with position: vec3(" << cell_->octBounds->maxVert.x << ", " << cell_->octBounds->maxVert.y << ", " << cell_->octBounds->maxVert.z << ")\n";
		}
	}
	//...and the cell is not a leaf...
	else
	{
		//...call this function on each of the cell's children to check if they are leaves and add the object to them if they are and are colliding with the object's bounding box.
		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			AddObjectToCell(cell_->children[i], obj_);
		}
	}
}

//Adds all leaf cells that a cast ray collides with to a list. This is so when we check for collisions, we only check against the cells that the ray actually collides with.
void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	//If the cell is a leaf...
	if (cell_->IsLeaf())
	{
		//...and the ray is colliding with the cell's bounding box...
		if (ray_.IsColliding(cell_->GetBoundingBox()))
		{
			//...add the cell to the intersection list.
			rayIntersectionList.push_back(cell_);
		}
	}
	//If the cell is not a leaf...
	else
	{
		//...call this function on each of the cell's children to check if they are leaves. If they are and the ray collides with them, add them to the intersection list.
		for (int i = 0; i < CHILDREN_NUMBER; i++)
		{
			PrepareCollisionQuery(cell_->children[i], ray_);
		}
	}
}
