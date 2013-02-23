#pragma once
#include "core.h"
#include "Structures.h"

// Renderer class. Takes in all the parameters to draw to the screen.

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	// Sets the world transformation of the object being rendered
	void SetWorldTransformation(XMMATRIX& worldTransformation);
	// Sets the projection matrix of the renderer
	void SetProjectionTransformation(float fieldOfView, float aspectRatio, float nearClippingPlane, float farClippingPlane);
	// Sets the eye position of the "camera"
    void SetEyePosition(float x, float y, float z);
	// Sets the focal point of the "camera"
    void SetFocalPointPosition(float x, float y, float z);
	// Sets the up vector of the "camera"
    void SetUpVector(float x, float y, float z);
	// Sets the view matrix based on the eye position, focal point and up vector
	void SetViewTransformation();
	// Sets the pointer to the vertex buffer 
	void SetVertexBuffer(ID3D11Buffer * vertexBuffer);
	// Sets the pointer to the index buffer
	void SetIndexBuffer(ID3D11Buffer * indexBuffer);
	// Sets the pointer to the constant buffer
	void SetConstantBuffer(ID3D11Buffer * constantBuffer);
	// Sets the pointer to the shader
	void SetTexture(ID3D11ShaderResourceView * texture);
	// Sets the ambient light
    void SetAmbientLight(float r, float g, float b, float a);
	// Sets the directional light's vector
    void SetDirectionalLightVector(float x, float y, float z);
	// Sets the directional light's colour
    void SetDirectionalLightColour(float r, float g, float b, float a);
	//
	void SetDiffuseColour(XMFLOAT4);
	// Initialised the shader file in preparation for rendering
	bool Initialise(void);
	// Renders the object 
	void Render(unsigned int primitiveCount, bool wireframe);
	// Shutdown
	void Shutdown(void);



private:

	ID3D11RasterizerState *	    _defaultRasteriserState;
	ID3D11RasterizerState *     _wireframeRasteriserState;

	// Interfaces accesed by the renderer
	ID3DBlob *					_vsBlob;
	ID3DBlob *					_psBlob;
	ID3D11VertexShader *		_vertexShader;
	ID3D11PixelShader *			_pixelShader;
	ID3D11InputLayout *			_layout;

	// Interfaces passed in by the object and should not be released here
	ID3D11Device *				_device;
	ID3D11DeviceContext *		_deviceContext;
	ID3D11Buffer *				_vertexBuffer;
	ID3D11Buffer *				_indexBuffer;
	ID3D11ShaderResourceView *	_texture;  
	ID3D11Buffer *				_constantBuffer;
	
	// Matrices used by the renderer
	XMMATRIX					_worldTransformation;
	XMMATRIX					_projectionTransformation;
	XMMATRIX					_viewTransformation;

	// Variables that make up the "camera"
    XMVECTOR                    _eyePosition;
    XMVECTOR                    _focalPointPosition;
    XMVECTOR                    _upVector;

	// Variables for the lights
    XMFLOAT4                    _ambientLightColour;
    XMFLOAT4                    _directionalLightColour;
    XMVECTOR                    _directionalLightVector;
	XMFLOAT4					_diffuseColour;

	// Constant buffer that will contain information on the lights and matrices for the shader
	CBUFFER cBuffer;
};