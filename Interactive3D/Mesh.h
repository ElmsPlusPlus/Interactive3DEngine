#pragma once
#include "core.h"
#include <vector>

typedef int ResourceId;

// Core material class.  Ideally, this should be extended to include more material attributes that can be
// recovered from Assimp, but this handles the basics.

class Material
{
public:
	Material(XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, ID3D11ShaderResourceView * texture );
	~Material(void);

	XMFLOAT4	               GetDiffuseColour(void);
	XMFLOAT4	               GetSpecularColour(void);
	float		               GetShininess(void);
    ID3D11ShaderResourceView * GetTexture(void);

private:
	XMFLOAT4	                _diffuseColour;
	XMFLOAT4	                _specularColour;
	float		                _shininess;
    ID3D11ShaderResourceView *  _texture;
};

// Basic SubMesh class.  A Mesh consists of one or more sub-meshes.  The submesh provides everything that is needed to
// provide to the renderer in order to draw the sub-mesh.

class SubMesh
{
public:
	SubMesh(ID3D11Buffer * vertexBuffer, 
			ID3D11Buffer * indexBuffer, 
			ID3D11Buffer * constantBuffer,
			unsigned int numberOfIndices,
			ResourceId material,
			XMFLOAT3* vertices,
			unsigned int numberOfVertices);
	~SubMesh(void);

	ID3D11Buffer *				GetVertexBuffer(void);
	ID3D11Buffer *				GetIndexBuffer(void);
	ID3D11Buffer *				GetConstantBuffer(void);
	ResourceId					GetMaterial(void);
	unsigned int				GetNumberOfIndices(void);
	XMFLOAT3*					GetSubMeshVertices(void);
	unsigned int				GetNumberOfVertices(void);

private:
   	ID3D11Buffer *				_vertexBuffer;
	ID3D11Buffer *				_indexBuffer;
	ID3D11Buffer *				_constantBuffer;
	ResourceId					_material;
	unsigned int				_numberOfIndices;
	XMFLOAT3*					_subMeshVerts;
	unsigned int				_numberOfVerts;
};

// The core Mesh class.  A Mesh corresponds to a scene in ASSIMP. A mesh consists of one or more sub-meshes.

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	unsigned int GetSubMeshCount(void);
	SubMesh *	 GetSubMesh(unsigned int i);
	void		 AddSubMesh(SubMesh * subMesh);

private:
	vector<SubMesh *> *	_subMesh;

};


