#pragma once
#include "GraphNode.h"
#include <vector>

// Inheirits from GraphNode
class SceneGraph : public GraphNode
{
public:
	SceneGraph(wstring name);
	~SceneGraph(void);

	// Overrides GraphNodes functions
	bool Initialise(void);
	void Update(XMMATRIX& currentWorldTransformation);
	void Render(void);		
	void Shutdown(void);

	// Overrides GraphNodes functions
	//Tree control methods
	void Add(GraphNode* node);
	void Remove(GraphNode* node);
	GraphNode* Find(wstring name);

private:
	// A collection of all the objects in the Scene, all objects are derived classes so they can be 
	// stored in the vector of GraphNode* 
	std::vector<GraphNode*> _children;
};

