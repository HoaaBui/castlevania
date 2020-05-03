#pragma once
#include "GameObject.h"

#define HEART_BBOX_WIDTH  32
#define HEART_BBOX_HEIGHT 42

#define HEART_STATE_ALIVE		100
#define HEART_STATE_DEAD	    200

class CHeart : public CGameObject{

public:
	CHeart();
	~CHeart();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};