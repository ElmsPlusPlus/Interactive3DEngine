#pragma once
#include "Framework.h"
#include "SceneNode.h"
#include "DirectXBoundingVolumes.h"

class ModelNode : public SceneNode
{
public:
	ModelNode(wstring name, string modelFile, int type,  string defaultTextureName = "");
	virtual ~ModelNode(void);
	virtual bool Initialise(void);
	virtual void Render(void);
	virtual void Shutdown(void);
	void BBoxRender(void);
	void BSphereRender(void);

	BoundingBox GetBoundingBox(void);
	BoundingSphere GetBoundingSphere(void);

protected:
	string					    _modelFile;
	string					    _defaultTextureName;
	ResourceId				    _meshResourceId;
	ResourceId					_defaultMaterialResourceId;
	BoundingBox					_bBox;
	BoundingBox					_transBBox;
	BoundingSphere				_bSphere;
	BoundingSphere				_transBSphere;
	int							_type;

	BoundingBox CreateBoundingBox(ResourceId meshResourceId);
	BoundingSphere CreateBoundingSphere(ResourceId meshResourceId);



};

