#pragma once
#include "scenenode.h"
#include "Framework.h"
#include "Structures.h"
class Cube : public SceneNode
{
public:
	Cube(wstring name);
	virtual ~Cube(void);

	////// SceneNode methods overrides //////

	// Passes in the buffers, texture and world transformation of the cube into the renderer
	// and calls the renderer to render
	virtual void Render(void);
	// Releases the COM interfaces called by the cube
	virtual void Shutdown(void);

	protected:
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
};

