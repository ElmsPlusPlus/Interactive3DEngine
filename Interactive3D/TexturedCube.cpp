#include "TexturedCube.h"

#pragma region Verts and Indices Values

	//VERTEX _cubeVertices[] =
	//{
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 1
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 2
	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 3
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 4
	//	{XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 5
	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),    XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 6
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},
	//};

	//VERTEX _cubeVertices[] =
	//{
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 1
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 2
	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 3
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 4
	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 5
	//	{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

	//	{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 6
	//	{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
	//	{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},
	//};


	//DWORD cubeIndices[] = 
	//{
	//	0, 1, 2,       // side 1
	//	2, 1, 3,
	//	4, 5, 6,       // side 2
	//	6, 5, 7,
	//	8, 9, 10,      // side 3
	//	10, 9, 11,
	//	12, 13, 14,    // side 4
	//	14, 13, 15,
	//	16, 17, 18,    // side 5
	//	18, 17, 19,
	//	20, 21, 22,    // side 6
	//	22, 21, 23,
	//};

#pragma endregion 


TexturedCube::TexturedCube(wstring name, float xPos, float yPos, float zPos, bool terrainPlaced) : SceneNode(name)
{
	degrees = 1.0f;
	x = xPos;
	y = yPos;
	z = zPos;
	offset = 1.5f;
	onTerrain = terrainPlaced;
}


TexturedCube::~TexturedCube(void)
{
}

bool TexturedCube::Initialise(void)
{
	if (onTerrain)
	{
		//TerrainNode* terrain;
		GraphNode* terrain;
		//terrain = dynamic_cast<TerrainNode*>(Framework::This()->GetSceneGraph()->Find(L"Terrain"));
		terrain = Framework::This()->GetSceneGraph()->Find(L"Terrain");
		//y = ((TerrainNode*)terrain)->PlaceOnTerrain(x,z);
		y = dynamic_cast<TerrainNode*>(terrain)->PlaceOnTerrain(x,z, offset);
	}

	//// Set up interfaces to initialise the renderer to render the cube
	//D3D11_BUFFER_DESC bufferDesc;
	//D3D11_MAPPED_SUBRESOURCE ms;
	//// Gets the pointer to the renderer being used by the framework
	//_renderer = Framework::This()->GetCurrentRenderer();
	//// Get the device and deviceContext from the framework
	//_device = Framework::This()->Device();
	//_deviceContext = Framework::This()->DeviceContext();
	//
	//// Create a vertex buffer
	//ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	//bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//bufferDesc.ByteWidth = sizeof(VERTEX) * 24;
	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_vertexBuffer)))
	//{
	//	return false;
	//}
	//if (FAILED(_deviceContext->Map(_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms)))
	//{
	//	return false;
	//}
	//memcpy(ms.pData, _cubeVertices, sizeof(_cubeVertices));
	//_deviceContext->Unmap(_vertexBuffer, NULL);

	//// Create an index buffer
	//ZeroMemory(&bufferDesc, sizeof(bufferDesc));
 //   bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
 //   bufferDesc.ByteWidth = sizeof(DWORD) * 36;
 //   bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
 //   bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 //   bufferDesc.MiscFlags = 0;
 //   if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_indexBuffer)))
	//{
	//	return false;
	//}
 //   _deviceContext->Map(_indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
 //   memcpy(ms.pData, cubeIndices, sizeof(cubeIndices));                 
 //   _deviceContext->Unmap(_indexBuffer, NULL);

	//// Create a constant buffer
	//ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(CBUFFER);
	//bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_constantBuffer)))
	//{
	//	return false;
	//}

	// Create a texture interface
	D3DX11CreateShaderResourceViewFromFile(_device, L"Wood.png", NULL, NULL, &_texture, NULL);          

	return true;
}

void TexturedCube::Update(XMMATRIX& currentWorldTransformation)
{

	transMat = XMMatrixTranslation(x,y,z);
	XMMATRIX scale = XMMatrixScaling(100.0f, 100.0f, 100.0f);
	_worldTransformation = scale * transMat;
	//_worldTransformation = transMat;
	//degrees += 1.0f;
	//yRotMat = XMMatrixRotationX(XMConvertToRadians(degrees));
	//_worldTransformation = yRotMat;
}

void TexturedCube::Render(void)
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

void TexturedCube::Shutdown(void)
{
	// Releases the COM interfaces used
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_constantBuffer);
	SAFE_RELEASE(_texture);
}
