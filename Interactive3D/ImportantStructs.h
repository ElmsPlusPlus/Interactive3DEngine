#pragma once

struct CBUFFER
 {
     XMMATRIX    Transformation;
	 XMMATRIX	 Rotation;
     XMVECTOR    LightVector;
     XMFLOAT4    LightColor;
     XMFLOAT4    AmbientColor;
 };

struct VERTEX
{
	XMFLOAT3 Position;  // position
    XMFLOAT3 Normal;    // normal
	XMFLOAT2 TexCoord;   // Texture UV Coordinates
};