#include "SceneGraph.h"
#include "Framework.h"

// Constructor
SceneGraph::SceneGraph(wstring name) : GraphNode(name)
{
}


SceneGraph::~SceneGraph(void)
{
}

bool SceneGraph::Initialise(void)
{
	// Loops through all the children and calls their initialise functions
	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		_children[i]->Initialise();
	}

	return true;
}

void SceneGraph::Update(XMMATRIX& currentWorldTransformation)
{
	// Loops through all the children and calls their update functions
	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		_children[i]->Update(currentWorldTransformation);
	}
}

void SceneGraph::Render(void)
{
	// Loops through all the children and calls their render functions
	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		_children[i]->Render();
	}
}

void SceneGraph::Shutdown(void)
{
	// Loops through all the children and calls their shutdown methods
	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		_children[i]->Shutdown();
	}
}


void SceneGraph::Add(GraphNode* node)
{
	// Adds a child to the vector list
	_children.push_back(node);
}

void SceneGraph::Remove(GraphNode* node)
{
	// Removes a child from the vector list 
	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		if (_children[i] == node)
		{
			_children.erase(_children.begin() + i);
			break;
		}
	}
}

GraphNode* SceneGraph::Find(wstring name)
{

	unsigned int size = _children.size();

	for (unsigned int i = 0; i < size; i++)
	{
		if(_children[i]->GetName() == name)
		{
			return _children[i];
		}		
	}

	return NULL;

}
