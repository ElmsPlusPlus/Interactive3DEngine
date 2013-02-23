#pragma once
#include "cube.h"
class WoodenCube :public Cube
{
public:
	WoodenCube(wstring name, float xPos, float yPos, float zPos, bool terrainPlace);
	~WoodenCube(void);

	bool Initialise(void);
	void Update(XMMATRIX& currentWorldTransformation);

private:
	XMMATRIX					transMat;
	float						x;
	float						y;
	float						z;
	bool						onTerrain;
	float						offset;
};

