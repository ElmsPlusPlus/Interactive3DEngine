#pragma once
#include "GraphNode.h"

// Inheirits from GraphNode
class SceneNode : public GraphNode
{
public:
	SceneNode(wstring name);
	virtual ~SceneNode(void);

	virtual bool Initialise(void) {return true;};
	virtual void Update(XMMATRIX& currentWorldTransformation) {};
	virtual void Render(void) {};		
	virtual void Shutdown(void) {};
	virtual void Add(GraphNode* node) {};
	virtual void Remove(GraphNode* node) {};
	virtual	GraphNode* Find(wstring name);
};
