#include "BiPlaneNode.h"


BiPlaneNode::BiPlaneNode(wstring name, string modelFile, int type, float xPos, float yPos, float zPos, float move, string defaultTextureName) : 
	ModelNode(name, modelFile, type, defaultTextureName)
{
	//x = 5.0f;
	//y = 300.0f;
	x = xPos;
	y = yPos;
	z = zPos;
	degrees = 0;
	_move = move;
	_type = type;

}

BiPlaneNode::~BiPlaneNode(void)
{
}

void BiPlaneNode::Update(XMMATRIX& currentWorldTranformation)
{
	//degrees += _degInc;
	x += _move;
	rotMat = XMMatrixRotationY(XMConvertToRadians(degrees));
	transMat = XMMatrixTranslation(x,y,z);
	//_worldTransformation = (transMat * rotMat);
	_worldTransformation = transMat;
	_bBox.Transform(_transBBox, _worldTransformation);
	_bSphere.Transform(_transBSphere, _worldTransformation);

	BiPlaneNode* otherNode;
	BiPlaneNode* otherNode2;

	if (_name == L"BiPlane1")
	{
		otherNode = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane2"));

		if(!(_transBBox.Intersects(otherNode->GetBoundingBox())))
		{
			_move = 0;
		}
	}

	if (_name == L"BiPlane2")
	{
		otherNode = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane1"));
		otherNode2 = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane3"));


		if(!(_transBBox.Intersects(otherNode->GetBoundingBox())))
		{
			_move = 0.5f;
		}

		if(_transBBox.Intersects(otherNode2->GetBoundingSphere()))
		{
			_move = 0.0f;
		}
	}

	if (_name == L"BiPlane3")
	{
		otherNode = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane2"));
		otherNode2 = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane4"));


		/*if(!(_transBBox.Intersects(otherNode->GetBoundingBox())))
		{
			_move = 0.5f;
		}*/

		if((otherNode->GetBoundingBox().Intersects(_transBSphere)))
		{
			_move = 0.5f;
		}

		if(_transBSphere.Intersects(otherNode2->GetBoundingSphere()))
		{
			_move = 0.0f;
		}
	}

	if (_name == L"BiPlane4")
	{
		otherNode = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane3"));

		if((otherNode->GetBoundingSphere().Intersects(_transBSphere)))
		{
			_move = 0.5f;
		}

	}

	//else
	//{
	//	otherNode = dynamic_cast<BiPlaneNode*>(Framework::This()->GetSceneGraph()->Find(L"BiPlane"));

	//	if(!(_transBBox.Intersects(otherNode->GetBoundingBox())))
	//	{
	//		_degInc = -(_degInc);
	//	}

	//	if(_transBSphere.Intersects(otherNode->GetBoundingSphere()))
	//	{
	//		//_degInc = -(_degInc);
	//	}

	//	if(_transBBox.Intersects(otherNode->GetBoundingSphere()))
	//	{
	//		//_degInc = -(_degInc);
	//	}
	//}
}