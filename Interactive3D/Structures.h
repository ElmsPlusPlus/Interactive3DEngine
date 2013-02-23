#ifndef STRUCTURES_H
#define STRUCTURES_H

// Useful structures for the program

// A constand buffer structure based on the elements passed into the shader
struct CBUFFER
 {
     XMMATRIX    Transformation;
	 XMMATRIX	 Rotation;
     XMVECTOR    LightVector;
     XMFLOAT4    LightColor;
     XMFLOAT4    AmbientColor;
	 XMFLOAT4	 DiffuseColor;
 };

// A vertex structure that stores its Position, Normal Vector and UV co-ordinates
struct VERTEX
{
	XMFLOAT3 Position;  // position
    XMFLOAT3 Normal;    // normal
	XMFLOAT2 TexCoord;   // Texture UV Coordinates
};

#endif