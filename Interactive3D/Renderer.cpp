#pragma once
#include "Renderer.h"
#include "Framework.h"

// The format of the vertex buffers that are being passed to the shader
D3D11_INPUT_ELEMENT_DESC ied[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

// Constructor
Renderer::Renderer()
{
	// Get the pointer addresses from the Framework
	_device = Framework::This()->Device();
	_deviceContext = Framework::This()->DeviceContext();

	

	// Set default and wireframe rasteriser states
	D3D11_RASTERIZER_DESC rasteriserDesc;
	rasteriserDesc.FillMode = D3D11_FILL_SOLID;
	rasteriserDesc.CullMode = D3D11_CULL_BACK;
	rasteriserDesc.FrontCounterClockwise = false;
	rasteriserDesc.DepthBias = 0;
	rasteriserDesc.SlopeScaledDepthBias = 0.0f;
	rasteriserDesc.DepthBiasClamp = 0.0f;
	rasteriserDesc.DepthClipEnable = true;
	rasteriserDesc.ScissorEnable = false;
	rasteriserDesc.MultisampleEnable = false;
	rasteriserDesc.AntialiasedLineEnable = false;
	_device->CreateRasterizerState(&rasteriserDesc, &_defaultRasteriserState);

	rasteriserDesc.FillMode = D3D11_FILL_WIREFRAME;
	_device->CreateRasterizerState(&rasteriserDesc, &_wireframeRasteriserState);
}

Renderer::~Renderer() 
{
}

void Renderer::SetWorldTransformation(XMMATRIX& worldTransformation)
{
	_worldTransformation = worldTransformation;
}

void Renderer::SetProjectionTransformation(float fieldOfView, float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	_projectionTransformation = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearClippingPlane, farClippingPlane);
}

void Renderer::SetEyePosition(float x, float y, float z)
{
	_eyePosition = XMVectorSet(x, y, z, 0);
}

void Renderer::SetFocalPointPosition(float x, float y, float z)
{
	_focalPointPosition = XMVectorSet(x, y, z, 0);
}

void Renderer::SetUpVector(float x, float y, float z)
{
	_upVector = XMVectorSet(x, y, z, 0);
}

void Renderer::SetViewTransformation()
{
	_viewTransformation =  XMMatrixLookAtLH(_eyePosition, 
		_focalPointPosition, _upVector);
}

void Renderer::SetVertexBuffer(ID3D11Buffer * vertexBuffer)
{
	_vertexBuffer = vertexBuffer;
}

void Renderer::SetIndexBuffer(ID3D11Buffer * indexBuffer)
{
	_indexBuffer = indexBuffer;
}

void Renderer::SetConstantBuffer(ID3D11Buffer * constantBuffer)
{
	_constantBuffer = constantBuffer;
}

void Renderer::SetTexture(ID3D11ShaderResourceView * texture)
{
	_texture = texture;
}

void Renderer::SetAmbientLight(float r, float g, float b, float a)
{
	_ambientLightColour = XMFLOAT4(r, g, b, a);
}

void Renderer::SetDirectionalLightVector(float x, float y, float z)
{
	_directionalLightVector = XMVector4Normalize(XMVectorSet(x, y, z, 0.0f));
}

void Renderer::SetDirectionalLightColour(float r, float g, float b, float a)
{
	_directionalLightColour = XMFLOAT4(r, g, b, a);
}

void Renderer::SetDiffuseColour(XMFLOAT4 diffuseColour)
{
	_diffuseColour = diffuseColour;
}

bool Renderer::Initialise(void)
{
	// Compile the shader file and references its information with the shader interfaces
    if (FAILED(D3DX11CompileFromFile(L"Shader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &_vsBlob, 0, 0)))
	{
		return false;
	}
	if (FAILED(_device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), NULL, &_vertexShader)))
	{
		return false;
	}
    _deviceContext->VSSetShader(_vertexShader, 0, 0);

    if (FAILED(D3DX11CompileFromFile(L"Shader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &_psBlob, 0, 0)))
	{
		return false;
	}
	if (FAILED(_device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), NULL, &_pixelShader)))
	{
		return false;
	}
    _deviceContext->PSSetShader(_pixelShader, 0, 0);

	if (FAILED(_device->CreateInputLayout(ied, ARRAYSIZE(ied), _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), &_layout)))
	{
		return false;
	}
	_deviceContext->IASetInputLayout(_layout);

	return true;
}

void Renderer::Render(unsigned int primitiveCount, bool wireframe)
{
	if (wireframe)
	{
		_deviceContext->RSSetState(_wireframeRasteriserState);
	}
	else
	{
		_deviceContext->RSSetState(_defaultRasteriserState);
	}

	_viewTransformation = Framework::This()->GetCamera()->GetViewMatrix();

	// Set the cBuffer with the values to draw the object
	cBuffer.LightVector = _directionalLightVector;
	cBuffer.LightColor = _directionalLightColour;
	cBuffer.AmbientColor = _ambientLightColour;
	cBuffer.Transformation = _worldTransformation  * _viewTransformation * _projectionTransformation;
	cBuffer.Rotation = _worldTransformation;
	cBuffer.DiffuseColor = _diffuseColour;


	// Update the constant buffer with the complete transformation
	_deviceContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
	_deviceContext->UpdateSubresource(_constantBuffer, 0, 0, &cBuffer, 0, 0);

	// Set the texture to be used by the pixel shader
    _deviceContext->PSSetShaderResources(0, 1, &_texture);

	// Draw the object 
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/*if(wireframe)
	{
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	else
	{
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}*/
	
	_deviceContext->DrawIndexed(primitiveCount, 0, 0);
}

void Renderer::Shutdown(void)
{
	// Release the interfaces accessed by the renderer
	SAFE_RELEASE(_vsBlob);
	SAFE_RELEASE(_psBlob);
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_pixelShader);
	SAFE_RELEASE(_layout);
}
