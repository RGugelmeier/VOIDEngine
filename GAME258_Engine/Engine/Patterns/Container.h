#ifndef CONTAINER_H
#define CONTAINER_H

#include <iostream>
#include <vector>
#include "../Rendering/3D/GameObject.h"

//Templated container class. Allows for this class to define functions for a container of any type.
template <typename T>
class Container
{
public:
	Container() {}
	~Container() {};

	//Add a new item to the container.
	void AddToContainer(T itemToAdd_)
	{
		container.push_back(itemToAdd_);
	}

	//Removes an item specified by the value passed in.
	void RemoveFromContainer(T itemToRemove_)
	{
		container.erase(std::remove(container.begin(), container.end(), itemToRemove_), container.end());
	}

	//Prints all items in the container to the console.
	void ListItems()
	{
		for (auto item : container)
		{
			std::cout << item << "\n";
		}
	}
private:
	std::vector<T> container;
};

#endif // !CONTAINER_H