#pragma once
#include "core.h"
//#include "BoundingShapeNode.h"

using namespace std;

// Abstract base class for all nodes of the scene graph.  
// This scene graph implements the Composite Design Pattern

class GraphNode
{
public:
	GraphNode(wstring name) {_name = name; _worldTransformation = XMMatrixIdentity();};
	virtual ~GraphNode(void) {};

	// Core methods
    virtual bool Initialise(void) = 0;
	virtual void Update(XMMATRIX& currentWorldTransformation) = 0;
	virtual void Render(void) = 0;		
	virtual void Shutdown(void) = 0;
		
	// Although only required in the composite class, these are provided
	// in order to simplify the code base.
	virtual void Add(GraphNode* node) = 0;
	virtual void Remove(GraphNode* node) = 0;
	virtual	GraphNode* Find(wstring name) = 0;

	// Returns Name of Node
	wstring GetName(void) {return _name;};

protected:
	wstring		_name;
	XMMATRIX	_worldTransformation;
	//BoundingShapeNode _boundingNode;
};

