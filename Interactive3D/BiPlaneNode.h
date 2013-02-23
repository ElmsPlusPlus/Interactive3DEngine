#pragma once
#include "modelnode.h"

class BiPlaneNode : public ModelNode
{
public:
	BiPlaneNode(wstring name, string modelFile, int type, float xPos, float yPos, float zPos, float move,  string defaultTextureName = "");
	~BiPlaneNode(void);

	void Update(XMMATRIX& currentWorldTransformation);

private:

	float degrees, _degInc;
	XMMATRIX rotMat;
	XMMATRIX transMat;
	float x;
	float y;
	float z;
	float _move;
};

