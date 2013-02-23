#include "Cube.h"



Cube::Cube(wstring name) : SceneNode(name)
{
}


Cube::~Cube(void)
{
}

void Cube::Render(void)
{
	// Passes in pointers to the renderer 
	_renderer->SetVertexBuffer(_vertexBuffer);
	_renderer->SetIndexBuffer(_indexBuffer);
	_renderer->SetConstantBuffer(_constantBuffer);
	_renderer->SetTexture(_texture);
	// Sets the world transformation for the renderer to use
	_renderer->SetWorldTransformation(_worldTransformation);
	// Calls the renderer to render
	_renderer->Render(36, false);
}

void Cube::Shutdown(void)
{
	// Releases the COM interfaces used
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_constantBuffer);
	SAFE_RELEASE(_texture);
}
