#pragma once
#include "scenenode.h"
#include "Framework.h"
#include "Structures.h"

// TexturedCube inheirits from SceneNode 
class TexturedCube : public SceneNode
{
public:
	TexturedCube(wstring name, float xPos, float yPos, float zPos, bool terrainPlaces);
	~TexturedCube(void);

////// SceneNode methods overrides //////

	// Initialises the vertex buffer, index buffer, constant buffer and texture
	// Also gets the pointer to the renderer in Framework
	bool Initialise(void);
	// Updates the textured cube
	void Update(XMMATRIX& currentWorldTransformation);
	// Passes in the buffers, texture and world transformation of the cube into the renderer
	// and calls the renderer to render
	void Render(void);
	// Releases the COM interfaces called by the cube
	void Shutdown(void);

private:
	// Device and DeviceContext interface passed from the Framework
	ID3D11Device *				_device;
	ID3D11DeviceContext *		_deviceContext;

	// Interfaces called by the cube 
	ID3D11Buffer *				_vertexBuffer;
	ID3D11Buffer *				_indexBuffer;
	ID3D11Buffer *				_constantBuffer;
	ID3D11ShaderResourceView *	_texture;

	// Pointer to a renderer, will be passed the value of Framework's renderer
	Renderer *					_renderer;

	// Values for transforming the cube
	float						degrees;
	XMMATRIX					yRotMat;
	XMMATRIX					transMat;
	float						x;
	float						y;
	float						z;
	bool						onTerrain;
	float						offset;
};
