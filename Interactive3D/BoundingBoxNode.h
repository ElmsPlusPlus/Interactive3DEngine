#pragma once
#include "boundingshapenode.h"
class BoundingBoxNode :
	public BoundingShapeNode
{
public:
	BoundingBoxNode(void);
	~BoundingBoxNode(void);

	bool IsIntersecting(BoundingShapeNode* otherBShape);
	void UpdateBoundingShape(XMMATRIX* worldMat);
};

