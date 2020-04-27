#pragma once
#include "GameObject.h"

// #define LIGHT_BBOX_WIDTH  32
// #define LIGHT_BBOX_HEIGHT 64
#define LIGHT_BBOX_WIDTH  0
#define LIGHT_BBOX_HEIGHT 0

class CLight : public CGameObject{
	
public:
	CLight();
	~CLight();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};