#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "light.h"
#include "Mario.h"

#define WHIP_BBOX_WIDTH  60
#define WHIP_BBOX_HEIGHT 20


#define WHIP_STATE_RIGHT		100
#define WHIP_STATE_LEFT			200
#define WHIP_STATE_DISAPPEAR	300

#define ANIMATION_ATTACK_WHIP_RIGHT_ZERO 0
#define ANIMATION_ATTACK_WHIP_LEFT_ZERO 1

class CWhip : public CGameObject
{
public:
    static CWhip* instance;
	static CWhip* GetInstance();
	float simonPosX;
	float simonPosY;
	int simonCurrentFrame;
public:
    CWhip();
	~CWhip();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetState(int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};