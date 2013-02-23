#include "TerrainNode.h"
#include <iostream>
#include <fstream>

TerrainNode::TerrainNode(wstring name, string heightFile, LPCWSTR textureFile, int cellsWidth, int cellsLength, int terrainStretch) :SceneNode(name)
{
	_heightFile = heightFile;
	_textureFile = textureFile;
	_cellsWidth = cellsWidth;
	_cellsLength = cellsLength;
	_halfWidth = (static_cast<float>(_cellsWidth / 2.0f));
	_halfLength = (static_cast<float>(_cellsLength / 2.0f));
	_numberOfVertices = ((_cellsWidth + 1) * (_cellsWidth + 1));
	_numberOfIndices = (_cellsWidth) * (_cellsLength) * 6;
	_vertices = new VERTEX[_numberOfVertices];
	_indices = new int[_numberOfIndices];
	TERRAIN_STRETCH = terrainStretch;
}


TerrainNode::~TerrainNode(void)
{
}

bool TerrainNode::Initialise(void)
{
	#pragma region Load Raw File
	float * _heightValues = new float[_numberOfVertices];
	BYTE * rawFileValues = new BYTE[_numberOfVertices];
	std::ifstream inputHeightMap(_heightFile.c_str(), std::ios_base::binary);
	if (inputHeightMap == NULL )
	{
		return false;
	}
	inputHeightMap.read(reinterpret_cast<char*>(rawFileValues), _numberOfVertices);
	inputHeightMap.close();
	// Normalise BYTE values to the range 0.0f - 1.0f;
	for(unsigned int i = 0; i < _numberOfVertices; i++)
	{
		_heightValues[i] = static_cast<float>(rawFileValues[i]) / (_cellsLength - 1);
	}
	delete[] rawFileValues;
	#pragma endregion

	#pragma region Load Vertices
	int k = 0;
	float u = 0,v = 0;
		//, halfWidth = (static_cast<float>(_cellsWidth / 2.0f)), halfLength = 
		//(static_cast<float>(_cellsLength / 2.0f));

	for (int z = 0; z < _cellsLength + 1; z++)
	{
		for (int x = 0; x < _cellsWidth + 1; x++)
		{
			k = z * _cellsLength + x;

			_vertices[k].Normal = XMFLOAT3(0,0,0);
			_vertices[k].Position = XMFLOAT3(((static_cast<float>(x) - _halfWidth) * TERRAIN_STRETCH), 
				_heightValues[k] * _cellsLength, (-(static_cast<float>(z) - _halfLength) * TERRAIN_STRETCH));
			_vertices[k].TexCoord = XMFLOAT2(static_cast<float>(x) / _cellsWidth,
				static_cast<float>(z) / _cellsLength);
		}
	}
	#pragma endregion

	#pragma region Generate Indices
    int counter = 0;
    for (int z = 0; z < _cellsLength -1; z++)
    {
        for (int x = 0; x < _cellsWidth - 1; x++)
        {
			int topLeft = x + z * (_cellsWidth);
            int topRight = (x + 1) + z * (_cellsWidth);
            int lowerLeft = x + (z + 1) * (_cellsWidth);
            int lowerRight = (x + 1) + (z + 1) * (_cellsWidth);
 
            _indices[counter++] = topLeft;
            _indices[counter++] = topRight;
            _indices[counter++] = lowerLeft;

            _indices[counter++] = lowerLeft;
            _indices[counter++] = topRight;
            _indices[counter++] = lowerRight;
        }
    }
	#pragma endregion

	#pragma region Calculate Normals
	int * _vertContCount = new int[_numberOfVertices];
	for (unsigned int i = 0; i < _numberOfVertices; i++)
	{
		_vertContCount[i] = 0;
	}
	for (int i = 0; i < (2 * ((_cellsWidth - 1) * (_cellsLength - 1))); i++)
	{
		int index0 = _indices[i * 3];
		int index1 = _indices[i * 3 + 1];
		int index2 = _indices[i * 3 + 2];
        XMVECTOR u = XMVectorSet(_vertices[index1].Position.x - _vertices[index0].Position.x,
                                 _vertices[index1].Position.y - _vertices[index0].Position.y,
                                 _vertices[index1].Position.z - _vertices[index0].Position.z,
                                 0.0f);
        XMVECTOR v = XMVectorSet(_vertices[index2].Position.x - _vertices[index0].Position.x,
                                 _vertices[index2].Position.y - _vertices[index0].Position.y,
                                 _vertices[index2].Position.z - _vertices[index0].Position.z,
                                 0.0f);
		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(u, v));
		XMStoreFloat3(&_vertices[index0].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index0].Normal), normal));
		_vertContCount[index0]++;
		XMStoreFloat3(&_vertices[index1].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index1].Normal), normal));
		_vertContCount[index1]++;
		XMStoreFloat3(&_vertices[index2].Normal, XMVectorAdd(XMLoadFloat3(&_vertices[index2].Normal), normal));
		_vertContCount[index2]++;
	}
	// Now divide the vertex normals by the contributing counts and normalise
	for (unsigned int i = 0; i < _numberOfVertices; i++)
	{
		XMVECTOR vertexNormal = XMLoadFloat3(&_vertices[i].Normal);
		XMStoreFloat3(&_vertices[i].Normal, XMVector3Normalize(vertexNormal / static_cast<float>(_vertContCount[i])));
	}
	delete _vertContCount;
	#pragma endregion

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
	bufferDesc.ByteWidth = sizeof(VERTEX) * _numberOfVertices;
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
	memcpy(ms.pData, _vertices, _numberOfVertices * sizeof(VERTEX));
	_deviceContext->Unmap(_vertexBuffer, NULL);

	// Create an index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(int) * _numberOfIndices;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    if (FAILED(_device->CreateBuffer(&bufferDesc, NULL, &_indexBuffer)))
	{
		return false;
	}
    _deviceContext->Map(_indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
    memcpy(ms.pData, _indices, sizeof(int) * _numberOfIndices);                 
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
	D3DX11CreateShaderResourceViewFromFile(_device, _textureFile, NULL, NULL, &_texture, NULL);
	//D3DX11CreateShaderResourceViewFromFile(_device, L"Terrain1.bmp", NULL, NULL, &_texture, NULL);

	return true;
}

void TerrainNode::Update(XMMATRIX& currentWorldMatrix)
{
}

void TerrainNode::Render(void)
{
	// Passes in pointers to the renderer 
	_renderer->SetVertexBuffer(_vertexBuffer);
	_renderer->SetIndexBuffer(_indexBuffer);
	_renderer->SetConstantBuffer(_constantBuffer);
	_renderer->SetTexture(_texture);
	// Sets the world transformation for the renderer to use
	_renderer->SetWorldTransformation(_worldTransformation);
	// Calls the renderer to render
	_renderer->Render(_numberOfIndices, false);
}

void TerrainNode::Shutdown(void)
{
	delete _vertices;
	delete _indices;

	// Releases the COM interfaces used
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_RELEASE(_constantBuffer);
	SAFE_RELEASE(_texture);
}

float TerrainNode::PlaceOnTerrain(float x, float z, float offset)
{
	float y;

	x = ((x / 5) + _halfWidth);
	z = ((-z / 5) + _halfLength);

	int k = static_cast<int>(z * _cellsLength + x);

	y = _vertices[k].Position.y + offset;

	return y;
}