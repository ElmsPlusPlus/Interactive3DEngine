#include "SkyBox.h"

	VERTEX _skyVertices[] =
	{
		{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 1
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 0.0f, 1.0f),  XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 2
		{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 3
		{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 4
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 5
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f)},    // side 6
		{XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f)},
	};


	DWORD skyIndices[] = 
	{
		0, 1, 2,       // side 1
		2, 1, 3,
		4, 5, 6,       // side 2
		6, 5, 7,
		8, 9, 10,      // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

SkyBox::SkyBox(wstring name) : Cube(name)
{
	x= 0; y=0; z=0;
}


SkyBox::~SkyBox(void)
{
}

bool SkyBox::Initialise(void)
{
	// Set up interfaces to initialise the renderer to render the cube
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_MAPPED_SUBRESOURCE ms;
	// Gets the pointer to the renderer being used by the framework
	_renderer = Framework::This()->GetCurrentRenderer();
	// Get the device and deviceContext from the framework
	_device = Framework::This()->Device();
	_deviceContext = Framework::This()->DeviceContext();
	
	// Create a vertex buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VERTEX) * 24;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_vertexBuffer)))
	{
		return false;
	}
	if (FAILED(_deviceContext->Map(_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms)))
	{
		return false;
	}
	memcpy(ms.pData, _skyVertices, sizeof(_skyVertices));
	_deviceContext->Unmap(_vertexBuffer, NULL);

	// Create an index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(DWORD) * 36;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_indexBuffer)))
	{
		return false;
	}
    _deviceContext->Map(_indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
    memcpy(ms.pData, skyIndices, sizeof(skyIndices));                 
    _deviceContext->Unmap(_indexBuffer, NULL);

	// Create a constant buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CBUFFER);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_constantBuffer)))
	{
		return false;
	}

	// Create a texture interface
	D3DX11CreateShaderResourceViewFromFile(_device, L"Sky.png", NULL, NULL, &_texture, NULL);          

	return true;
}

void SkyBox::Update(XMMATRIX& currentWorldMatrix)
{
	x = Framework::This()->GetCamera()->GetCameraPosition().x;
	y = Framework::This()->GetCamera()->GetCameraPosition().y;
	z = Framework::This()->GetCamera()->GetCameraPosition().z;
	transMat = XMMatrixTranslation(x,y,z);
	XMMATRIX scale = XMMatrixScaling(5000.0f, 5000.0f, 5000.0f);
	_worldTransformation = scale * transMat;
	//_worldTransformation = transMat;
}
