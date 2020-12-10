#pragma once
#include "GameObject.h"

#define KNIFE_ICON_BBOX_WIDTH  32
#define KNIFE_ICON_BBOX_HEIGHT 32

#define KNIFE_ICON_STATE_ALIVE		100
#define KNIFE_ICON_STATE_DEAD	    200

#define KNIFE_ICON_GRAVITY			0.002f

class CKnifeIcon : public CGameObject{

public:
	CKnifeIcon();
	~CKnifeIcon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};