#pragma once
#include "core.h"
#if !defined(WINDOWS8_SDK)

// This structure is supplied to use with DirectX 11 if the Windows 8 SDK is not installed.  Only a subset
// of the routines supplied in DirectXCollision.h have been implemented.  All routines have been copied
// directly from DirectXCollision.h and DirectXCollision.inl, suitably modified to enable them to be compiled
// using Visual Studio 2010 with the June 2010 DirectX SDK.  
//
// If the Windows 8 SDK is installed, DirectXCollision.h should be used instead.

struct BoundingSphere
{
	XMFLOAT3	Center;
	float		Radius;

    BoundingSphere& operator=(const BoundingSphere& sp ) { Center = sp.Center; Radius = sp.Radius; return *this; }

    void Transform(BoundingSphere& Out, CXMMATRIX M ) const;
    void Transform(BoundingSphere& Out, float Scale, FXMVECTOR Rotation, FXMVECTOR Translation ) const;

    bool Intersects(const BoundingSphere& sh ) const;

    static void CreateMerged(BoundingSphere& Out, const BoundingSphere& S1, const BoundingSphere& S2);
    static void CreateFromPoints(BoundingSphere& Out, size_t Count, const XMFLOAT3* pPoints, size_t Stride );

};

struct BoundingBox
{
    static const size_t CORNER_COUNT = 8;

    XMFLOAT3 Center;            // Center of the box.
    XMFLOAT3 Extents;           // Distance from the center to each side.

    BoundingBox& operator=(const BoundingBox& box) { Center = box.Center; Extents = box.Extents; return *this; }

    void Transform(BoundingBox& Out, CXMMATRIX M ) const;
    void Transform(BoundingBox& Out, float Scale, FXMVECTOR Rotation, FXMVECTOR Translation ) const;

    void GetCorners(XMFLOAT3* Corners ) const;

    bool Intersects(const BoundingSphere& sh ) const;
    bool Intersects(const BoundingBox& box ) const;

    static void CreateMerged(BoundingBox& Out, const BoundingBox& b1, const BoundingBox& b2 );
    static void CreateFromPoints(BoundingBox& Out, size_t Count, const XMFLOAT3* pPoints, size_t Stride );
};

#endif
