#pragma once
#include "GameObject.h"

#define LIGHT_BBOX_WIDTH  32
#define LIGHT_BBOX_HEIGHT 32

class CLight : public CGameObject{
	
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};