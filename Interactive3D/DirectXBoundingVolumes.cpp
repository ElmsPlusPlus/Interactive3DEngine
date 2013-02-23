#include "DirectXBoundingVolumes.h"
#if !defined(WINDOWS8_SDK)
#pragma warning( disable : 4800 ) 

// These routines are copied directly from the routines in DirectXCollision.h, supplied as part of the
// Windows 8 SDK. They have only been modified to account for differences in types/methods exposed by
// the DirectX 11.1 include files. No other changes have been made.

XMGLOBALCONST XMVECTORF32 g_BoxOffset[8] =
{
    { -1.0f, -1.0f,  1.0f, 0.0f },
    {  1.0f, -1.0f,  1.0f, 0.0f },
    {  1.0f,  1.0f,  1.0f, 0.0f },
    { -1.0f,  1.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, -1.0f, 0.0f },
    {  1.0f,  1.0f, -1.0f, 0.0f },
    { -1.0f,  1.0f, -1.0f, 0.0f },
};

void BoundingSphere::CreateFromPoints(BoundingSphere& Out, size_t Count, const XMFLOAT3* pPoints, size_t Stride )
{
    // Find the points with minimum and maximum x, y, and z
    XMVECTOR MinX, MaxX, MinY, MaxY, MinZ, MaxZ;

    MinX = MaxX = MinY = MaxY = MinZ = MaxZ = XMLoadFloat3( pPoints );

    for( size_t i = 1; i < Count; ++i )
    {
        XMVECTOR Point = XMLoadFloat3( reinterpret_cast<const XMFLOAT3*>( reinterpret_cast<const BYTE*>(pPoints) + i * Stride ) );

        float px = XMVectorGetX( Point );
        float py = XMVectorGetY( Point );
        float pz = XMVectorGetZ( Point );

        if( px < XMVectorGetX( MinX ) )
            MinX = Point;

        if( px > XMVectorGetX( MaxX ) )
            MaxX = Point;

        if( py < XMVectorGetY( MinY ) )
            MinY = Point;

        if( py > XMVectorGetY( MaxY ) )
            MaxY = Point;

        if( pz < XMVectorGetZ( MinZ ) )
            MinZ = Point;

        if( pz > XMVectorGetZ( MaxZ ) )
            MaxZ = Point;
    }

    // Use the min/max pair that are farthest apart to form the initial sphere.
    XMVECTOR DeltaX = MaxX - MinX;
    XMVECTOR DistX = XMVector3Length( DeltaX );

    XMVECTOR DeltaY = MaxY - MinY;
    XMVECTOR DistY = XMVector3Length( DeltaY );

    XMVECTOR DeltaZ = MaxZ - MinZ;
    XMVECTOR DistZ = XMVector3Length( DeltaZ );

    XMVECTOR vCenter;
    XMVECTOR vRadius;

    if( XMVector3Greater( DistX, DistY ) )
    {
        if( XMVector3Greater( DistX, DistZ ) )
        {
            // Use min/max x.
            vCenter = XMVectorLerp(MaxX,MinX,0.5f);
            vRadius = DistX * 0.5f;
        }
        else
        {
            // Use min/max z.
            vCenter = XMVectorLerp(MaxZ,MinZ,0.5f);
            vRadius = DistZ * 0.5f;
        }
    }
    else // Y >= X
    {
        if( XMVector3Greater( DistY, DistZ ) )
        {
            // Use min/max y.
            vCenter = XMVectorLerp(MaxY,MinY,0.5f);
            vRadius = DistY * 0.5f;
        }
        else
        {
            // Use min/max z.
            vCenter = XMVectorLerp(MaxZ,MinZ,0.5f);
            vRadius = DistZ * 0.5f;
        }
    }

    // Add any points not inside the sphere.
    for( size_t i = 0; i < Count; ++i )
    {
        XMVECTOR Point = XMLoadFloat3( reinterpret_cast<const XMFLOAT3*>( reinterpret_cast<const BYTE*>(pPoints) + i * Stride ) );

        XMVECTOR Delta = Point - vCenter;

        XMVECTOR Dist = XMVector3Length( Delta );

        if( XMVector3Greater( Dist, vRadius ) )
        {
            // Adjust sphere to include the new point.
            vRadius = ( vRadius + Dist ) * 0.5f;
            vCenter += ( XMVectorReplicate( 1.0f ) - XMVectorDivide(vRadius,Dist) ) * Delta;
        }
    }

    XMStoreFloat3( &Out.Center, vCenter );
    XMStoreFloat( &Out.Radius, vRadius );

}

void BoundingSphere::CreateMerged(BoundingSphere& Out, const BoundingSphere& S1, const BoundingSphere& S2)
{
    XMVECTOR Center1 = XMLoadFloat3( &S1.Center );
    float r1 = S1.Radius;

    XMVECTOR Center2 = XMLoadFloat3( &S2.Center );
    float r2 = S2.Radius;

    XMVECTOR V = XMVectorSubtract( Center2, Center1 );

    XMVECTOR Dist = XMVector3Length( V );

    float d = XMVectorGetX(Dist);

    if ( r1 + r2 >= d )
    {
        if ( r1 - r2 >= d )
        {
            Out = S1;
            return;
        }
        else if ( r2 - r1 >= d )
        {
            Out = S2;
            return;
        }
    }

    XMVECTOR N = XMVectorDivide( V, Dist );

    float t1 = XMMin( -r1, d-r2 );
    float t2 = XMMax( r1, d+r2 );
    float t_5 = (t2 - t1) * 0.5f;
    
    XMVECTOR NCenter = XMVectorAdd( Center1, XMVectorMultiply( N, XMVectorReplicate( t_5 + t1 ) ) );

    XMStoreFloat3( &Out.Center, NCenter );
    Out.Radius = t_5;
}

void BoundingSphere::Transform(BoundingSphere& Out, CXMMATRIX M ) const
{
    // Load the center of the sphere.
    XMVECTOR vCenter = XMLoadFloat3( &Center );

    // Transform the center of the sphere.
    XMVECTOR C = XMVector3Transform( vCenter, M );
    
    XMVECTOR dX = XMVector3Dot( M.r[0], M.r[0] );
    XMVECTOR dY = XMVector3Dot( M.r[1], M.r[1] );
    XMVECTOR dZ = XMVector3Dot( M.r[2], M.r[2] );

    XMVECTOR d = XMVectorMax( dX, XMVectorMax( dY, dZ ) );

    // Store the center sphere.
    XMStoreFloat3( &Out.Center, C );

    // Scale the radius of the pshere.
    float Scale = sqrtf( XMVectorGetX(d) );
    Out.Radius = Radius * Scale;
}

void BoundingSphere::Transform(BoundingSphere& Out, float Scale, FXMVECTOR Rotation, FXMVECTOR Translation ) const
{
    // Load the center of the sphere.
    XMVECTOR vCenter = XMLoadFloat3( &Center );

    // Transform the center of the sphere.
    vCenter = XMVector3Rotate( vCenter * XMVectorReplicate( Scale ), Rotation ) + Translation;

    // Store the center sphere.
    XMStoreFloat3( &Out.Center, vCenter );

    // Scale the radius of the pshere.
    Out.Radius = Radius * Scale;
}

bool BoundingSphere::Intersects(const BoundingSphere& sh ) const
{
    // Load A.
    XMVECTOR vCenterA = XMLoadFloat3( &Center );
    XMVECTOR vRadiusA = XMVectorReplicatePtr( &Radius );

    // Load B.
    XMVECTOR vCenterB = XMLoadFloat3( &sh.Center );
    XMVECTOR vRadiusB = XMVectorReplicatePtr( &sh.Radius );

    // Distance squared between centers.    
    XMVECTOR Delta = vCenterB - vCenterA;
    XMVECTOR DistanceSquared = XMVector3LengthSq( Delta );

    // Sum of the radii squared.
    XMVECTOR RadiusSquared = XMVectorAdd( vRadiusA, vRadiusB );
    RadiusSquared = XMVectorMultiply( RadiusSquared, RadiusSquared );

    return XMVector3LessOrEqual( DistanceSquared, RadiusSquared );
}

void BoundingBox::Transform(BoundingBox& Out, CXMMATRIX M ) const
{
    // Load center and extents.
    XMVECTOR vCenter = XMLoadFloat3( &Center );
    XMVECTOR vExtents = XMLoadFloat3( &Extents );

    // Compute and transform the corners and find new min/max bounds.
    XMVECTOR Corner = XMVectorMultiplyAdd( vExtents, g_BoxOffset[0], vCenter );
    Corner = XMVector3Transform( Corner, M );

    XMVECTOR Min, Max;
    Min = Max = Corner;

    for( size_t i = 1; i < CORNER_COUNT; ++i )
    {
        Corner = XMVectorMultiplyAdd( vExtents, g_BoxOffset[i], vCenter );
        Corner = XMVector3Transform( Corner, M );

        Min = XMVectorMin( Min, Corner );
        Max = XMVectorMax( Max, Corner );
    }

    // Store center and extents.
    XMStoreFloat3( &Out.Center, ( Min + Max ) * 0.5f );
    XMStoreFloat3( &Out.Extents, ( Max - Min ) * 0.5f );
}

void BoundingBox::Transform(BoundingBox& Out, float Scale, FXMVECTOR Rotation, FXMVECTOR Translation ) const
{
    // Load center and extents.
    XMVECTOR vCenter = XMLoadFloat3( &Center );
    XMVECTOR vExtents = XMLoadFloat3( &Extents );

    XMVECTOR VectorScale = XMVectorReplicate( Scale );

    // Compute and transform the corners and find new min/max bounds.
    XMVECTOR Corner = XMVectorMultiplyAdd( vExtents, g_BoxOffset[0], vCenter );
    Corner = XMVector3Rotate( Corner * VectorScale, Rotation ) + Translation;

    XMVECTOR Min, Max;
    Min = Max = Corner;

    for( size_t i = 1; i < CORNER_COUNT; ++i )
    {
        Corner = XMVectorMultiplyAdd( vExtents, g_BoxOffset[i], vCenter );
        Corner = XMVector3Rotate( Corner * VectorScale, Rotation ) + Translation;

        Min = XMVectorMin( Min, Corner );
        Max = XMVectorMax( Max, Corner );
    }

    // Store center and extents.
    XMStoreFloat3( &Out.Center, ( Min + Max ) * 0.5f );
    XMStoreFloat3( &Out.Extents, ( Max - Min ) * 0.5f );
}

void BoundingBox::GetCorners(XMFLOAT3* Corners ) const
{
    // Load the box
    XMVECTOR vCenter = XMLoadFloat3( &Center );
    XMVECTOR vExtents = XMLoadFloat3( &Extents );

    for( size_t i = 0; i < CORNER_COUNT; ++i )
    {
        XMVECTOR C = XMVectorMultiplyAdd( vExtents, g_BoxOffset[i], vCenter );
        XMStoreFloat3( &Corners[i], C );
    }
}

bool BoundingBox::Intersects(const BoundingSphere& sh ) const
{
    XMVECTOR SphereCenter = XMLoadFloat3( &sh.Center );
    XMVECTOR SphereRadius = XMVectorReplicatePtr( &sh.Radius );

    XMVECTOR BoxCenter = XMLoadFloat3( &Center );
    XMVECTOR BoxExtents = XMLoadFloat3( &Extents );

    XMVECTOR BoxMin = BoxCenter - BoxExtents;
    XMVECTOR BoxMax = BoxCenter + BoxExtents;

    // Find the distance to the nearest point on the box.
    // for each i in (x, y, z)
    // if (SphereCenter(i) < BoxMin(i)) d2 += (SphereCenter(i) - BoxMin(i)) ^ 2
    // else if (SphereCenter(i) > BoxMax(i)) d2 += (SphereCenter(i) - BoxMax(i)) ^ 2

    XMVECTOR d = XMVectorZero();

    // Compute d for each dimension.
    XMVECTOR LessThanMin = XMVectorLess( SphereCenter, BoxMin );
    XMVECTOR GreaterThanMax = XMVectorGreater( SphereCenter, BoxMax );

    XMVECTOR MinDelta = SphereCenter - BoxMin;
    XMVECTOR MaxDelta = SphereCenter - BoxMax;

    // Choose value for each dimension based on the comparison.
    d = XMVectorSelect( d, MinDelta, LessThanMin );
    d = XMVectorSelect( d, MaxDelta, GreaterThanMax );

    // Use a dot-product to square them and sum them together.
    XMVECTOR d2 = XMVector3Dot( d, d );

    return XMVector3LessOrEqual( d2, XMVectorMultiply( SphereRadius, SphereRadius ) );
}
 
bool BoundingBox::Intersects(const BoundingBox& box ) const
{
    XMVECTOR CenterA = XMLoadFloat3( &Center );
    XMVECTOR ExtentsA = XMLoadFloat3( &Extents );

    XMVECTOR CenterB = XMLoadFloat3( &box.Center );
    XMVECTOR ExtentsB = XMLoadFloat3( &box.Extents );

    XMVECTOR MinA = CenterA - ExtentsA;
    XMVECTOR MaxA = CenterA + ExtentsA;

    XMVECTOR MinB = CenterB - ExtentsB;
    XMVECTOR MaxB = CenterB + ExtentsB;

    // for each i in (x, y, z) if a_min(i) > b_max(i) or b_min(i) > a_max(i) then return false
    XMVECTOR Disjoint = XMVectorOrInt( XMVectorGreater( MinA, MaxB ), XMVectorGreater( MinB, MaxA ) );

    // Duplicate the fourth element from the first element.
    XMVECTOR C = XMVectorSwizzle(Disjoint, 0, 1, 2, 0);
    return XMComparisonAnyTrue( XMVector4EqualIntR( C, XMVectorTrueInt() ) );
}

void BoundingBox::CreateMerged(BoundingBox& Out, const BoundingBox& b1, const BoundingBox& b2 )
{
    XMVECTOR b1Center = XMLoadFloat3( &b1.Center );
    XMVECTOR b1Extents = XMLoadFloat3( &b1.Extents );

    XMVECTOR b2Center = XMLoadFloat3( &b2.Center );
    XMVECTOR b2Extents = XMLoadFloat3( &b2.Extents );

    XMVECTOR Min = XMVectorSubtract( b1Center, b1Extents );
    Min = XMVectorMin( Min, XMVectorSubtract( b2Center, b2Extents ) );

    XMVECTOR Max = XMVectorAdd( b1Center, b1Extents );
    Max = XMVectorMax( Max, XMVectorAdd( b2Center, b2Extents ) );

    XMStoreFloat3( &Out.Center, ( Min + Max ) * 0.5f );
    XMStoreFloat3( &Out.Extents, ( Max - Min ) * 0.5f );
}

void BoundingBox::CreateFromPoints(BoundingBox& Out, size_t Count, const XMFLOAT3* pPoints, size_t Stride )
{
    // Find the minimum and maximum x, y, and z
    XMVECTOR vMin, vMax;

    vMin = vMax = XMLoadFloat3( pPoints );

    for( size_t i = 1; i < Count; ++i )
    {
        XMVECTOR Point = XMLoadFloat3( reinterpret_cast<const XMFLOAT3*>( reinterpret_cast<const BYTE*>(pPoints) + i * Stride ) );

        vMin = XMVectorMin( vMin, Point );
        vMax = XMVectorMax( vMax, Point );
    }

    // Store center and extents.
    XMStoreFloat3( &Out.Center, ( vMin + vMax ) * 0.5f );
    XMStoreFloat3( &Out.Extents, ( vMax - vMin ) * 0.5f );
}

#pragma warning( default : 4800 )
#endif
