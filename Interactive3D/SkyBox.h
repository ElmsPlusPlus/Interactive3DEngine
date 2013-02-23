#pragma once
#include "cube.h"

class SkyBox : public Cube
{
public:
	SkyBox(wstring name);
	~SkyBox(void);

	bool Initialise(void);
	void Update(XMMATRIX& currentWorldTransformation);

private:
	XMMATRIX					transMat;
	float						x;
	float						y;
	float						z;
};

