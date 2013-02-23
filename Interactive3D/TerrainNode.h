#pragma once
#include "scenenode.h"
#include "Structures.h"
#include "Framework.h"
class TerrainNode : public SceneNode
{
public:
	TerrainNode(wstring name, string heightFile, LPCWSTR textureFile, int cellsWidth, int cellsLength, int terrainStretch);
	virtual ~TerrainNode(void);

	bool Initialise(void);
	void Update(XMMATRIX& currentWorldTransformation);
	void Render(void);
	void Shutdown(void);
	float PlaceOnTerrain(float x, float z, float offset);

private:
	string _heightFile;
	LPCWSTR _textureFile;
	unsigned int _numberOfVertices;
	unsigned int _numberOfIndices;
	int _cellsWidth;
	int _cellsLength;
	float _halfWidth;
	float _halfLength;
	VERTEX * _vertices;
	int * _indices;
	int		TERRAIN_STRETCH;

	// Device and DeviceContext interface passed from the Framework
	ID3D11Device *				_device;
	ID3D11DeviceContext *		_deviceContext;

	// Interfaces called by the texture 
	ID3D11Buffer *				_vertexBuffer;
	ID3D11Buffer *				_indexBuffer;
	ID3D11Buffer *				_constantBuffer;
	ID3D11ShaderResourceView *	_texture;

	// Pointer to a renderer, will be passed the value of Framework's renderer
	Renderer *					_renderer;
};

