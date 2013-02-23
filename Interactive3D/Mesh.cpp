#include "Mesh.h"

// Material methods

Material::Material(XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, ID3D11ShaderResourceView * texture )
{
	_diffuseColour = diffuseColour;
	_specularColour = specularColour;
	_shininess = shininess;
    _texture = texture;
}

Material::~Material(void)
{
}

XMFLOAT4 Material::GetDiffuseColour(void)
{
	return _diffuseColour;
}

XMFLOAT4 Material::GetSpecularColour(void)
{
	return _specularColour;
}

float Material::GetShininess(void)
{
	return _shininess;
}

ID3D11ShaderResourceView * Material::GetTexture(void)
{
    return _texture;
}

// SubMesh methods

SubMesh::SubMesh(ID3D11Buffer * vertexBuffer, 
				 ID3D11Buffer * indexBuffer, 
				 ID3D11Buffer * constantBuffer,
				 unsigned int numberOfIndices,
				 ResourceId material,
				 XMFLOAT3* vertices,
				 unsigned int numberOfVertices)
{
	_vertexBuffer = vertexBuffer;
	_indexBuffer = indexBuffer;
	_constantBuffer = constantBuffer;
	_numberOfIndices = numberOfIndices;
	_material = material;
	_subMeshVerts = vertices;
	_numberOfVerts = numberOfVertices;
}

SubMesh::~SubMesh(void)
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
}

ID3D11Buffer * SubMesh::GetVertexBuffer(void)
{
	return _vertexBuffer;
}

ID3D11Buffer * SubMesh::GetIndexBuffer(void)
{
	return _indexBuffer;
}

ID3D11Buffer * SubMesh::GetConstantBuffer(void)
{
	return _constantBuffer;
}

ResourceId SubMesh::GetMaterial(void)
{
	return _material;
}

unsigned int SubMesh::GetNumberOfIndices(void)
{
	return _numberOfIndices;
}

XMFLOAT3* SubMesh::GetSubMeshVertices(void)
{
	return _subMeshVerts;
}

unsigned int SubMesh::GetNumberOfVertices(void)
{
	return _numberOfVerts;
}

// Mesh methods

Mesh::Mesh(void)
{
	_subMesh = new vector<SubMesh *>();
}

Mesh::~Mesh(void)
{
	while (_subMesh->size())
	{
		_subMesh->pop_back();
	}
}

unsigned int Mesh::GetSubMeshCount(void)
{
	return _subMesh->size();
}

SubMesh * Mesh::GetSubMesh(unsigned int i)
{
	if (i >= 0 && i < _subMesh->size())
	{
		return _subMesh->at(i);
	}
	else
	{
		return NULL;
	}
}

void Mesh::AddSubMesh(SubMesh * subMesh)
{
	_subMesh->push_back(subMesh);
}
