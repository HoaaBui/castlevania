#pragma once
#include "GameObject.h"

#define WHIP_ICON_BBOX_WIDTH  32
#define WHIP_ICON_BBOX_HEIGHT 32

#define WHIP_ICON_STATE_ALIVE		100
#define WHIP_ICON_STATE_DEAD	    200

#define WHIP_ICON_GRAVITY			0.002f

class CWhipIcon : public CGameObject{

public:
	CWhipIcon();
	~CWhipIcon();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};