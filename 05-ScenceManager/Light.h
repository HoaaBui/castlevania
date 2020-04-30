#pragma once
#include "GameObject.h"

#define LIGHT_BBOX_WIDTH  32
#define LIGHT_BBOX_HEIGHT 64

#define LIGHT_STATE_ALIVE		100
#define LIGHT_STATE_DEAD		200
#define LIGHT_STATE_DISAPPEAR	300

#define ANIMATION_LIGHT_ALIVE 	0
#define ANIMATION_LIGHT_DEAD 	1

class CLight : public CGameObject{
public:
	DWORD lifeTime;
public:
	CLight();
	~CLight();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};