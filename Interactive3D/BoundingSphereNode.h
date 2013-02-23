#pragma once
#include "boundingshapenode.h"
class BoundingSphereNode :
	public BoundingShapeNode
{
public:
	BoundingSphereNode(void);
	~BoundingSphereNode(void);

	bool IsIntersecting(BoundingShapeNode* otherBShape);
	void UpdateBoundingShape(XMMATRIX* worldMat);
};

