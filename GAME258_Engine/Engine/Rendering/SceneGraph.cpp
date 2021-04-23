#include "SceneGraph.h"

//Redeclaration of static variables.
unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
unordered_map<GLuint, vector<Model*>> SceneGraph::sceneModels =
	unordered_map<GLuint, vector<Model*>>();
map<string, GameObject*> SceneGraph::sceneGameObjects =
	map<string, GameObject*>();

SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
	OnDestroy();
}

//Instance getter. Create a new instance if one does not exist already. Return instance if it does.
SceneGraph* SceneGraph::GetInstance()
{
	if (sceneGraphInstance.get() == nullptr)
	{
		sceneGraphInstance.reset(new SceneGraph);
	}

	return sceneGraphInstance.get();
}

//Clean up the two maps.
void SceneGraph::OnDestroy()
{
	//Clean out game object map if the size is > 0.
	if (sceneGameObjects.size() > 0)
	{
		for (auto gameObject : sceneGameObjects)
		{
			delete gameObject.second;
			gameObject.second = nullptr;
		}
		sceneGameObjects.clear();
	}

	//Clean out model map if the size is > 0.
	if (sceneModels.size() > 0)
	{
		for (auto entry : sceneModels)
		{
			if (entry.second.size() > 0)
			{
				for (auto m : entry.second)
				{
					delete m;
					m = nullptr;
				}
				entry.second.clear();
			}
		}
		sceneModels.clear();
	}
}

//If the model's shader exists as a key already, add the model to the list with the key.
//If the model's shader does not exist as a key, create a new entry to the map with the model's key and value, then add that entry to the sceneModel's vector.
void SceneGraph::AddModel(Model* model_)
{
	GLuint shader = model_->GetShaderProgram();
	if (sceneModels.find(shader) == sceneModels.end())
	{
		sceneModels.insert(pair<GLuint, vector<Model*>>(
			shader, vector<Model*>()));
		sceneModels[shader].reserve(10);
	}
	sceneModels[shader].push_back(model_);
}

//Add a game object to the scene graph.
void SceneGraph::AddGameObject(GameObject* gameObject_, bool collidable_, string tag_)
{
	//If the object does not have a tag, create it with a default tag of "GameObject 'x'" where x is a number.
	if (tag_ == "")
	{
		string newTag = "GameObject" + to_string(sceneGameObjects.size() + 1);
		gameObject_->SetTag(newTag);
		sceneGameObjects[newTag] = gameObject_;
	}
	//If the object does have a tag and does not already exist, create the object.
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end())
	{
		gameObject_->SetTag(tag_);
		sceneGameObjects[tag_] = gameObject_;
	}
	//If the object's tag does already exist i nthe vector, print an error to the log and create the object with the default tag mentioned above.
	else
	{
		Debug::Error("Trying to add a GameObject with a tag " + tag_ + " that already exists.", "SceneGraph.cpp", __LINE__);
		string newTag = "GameObject" + to_string(sceneGameObjects.size() + 1);
		gameObject_->SetTag(newTag);
		sceneGameObjects[newTag] = gameObject_;
	}
	if (collidable_)
	{
		//If this game object being added is collidable, add it to the collisioin handler so it can check for collisions on this object.
		CollisionHandler::GetInstance()->AddObject(gameObject_);
	}
	
}

//Return the object with the provided tag, or return nullptr if the object being looked for does not exist.
GameObject* SceneGraph::GetGameObject(string tag_)
{
	if (sceneGameObjects.find(tag_) != sceneGameObjects.end())
	{
		return sceneGameObjects[tag_];
	}
	return nullptr;
}

//Run each game object's update.
void SceneGraph::Update(const float deltaTime_)
{
	for (auto gameObject : sceneGameObjects)
	{
		gameObject.second->Update(deltaTime_);
	}
}

//Render each model with their given shader.
void SceneGraph::Render(Camera* camera_)
{
	for (auto entry : sceneModels)
	{
		glUseProgram(entry.first);
		for (auto m : entry.second)
		{
			//if (Frustum::GetInstance()->SeenCheck(m))
			//{
				m->Render(camera_);
			//}
		}
	}
}
