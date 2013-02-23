#pragma once
#include "scenenode.h"
#include "Framework.h"
class BoundingShapeNode
{
public:
	BoundingShapeNode(void);
	virtual ~BoundingShapeNode(void);

	virtual bool IsIntersecting(BoundingShapeNode* otherBShape) = 0;
	virtual void UpdateBoundingShape(XMMATRIX* worldMat) = 0;;
};

