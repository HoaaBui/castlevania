#pragma once
#include "GameObject.h"

#define WHIP_BBOX_WIDTH  16
#define WHIP_BBOX_HEIGHT 16

class CWhip : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};