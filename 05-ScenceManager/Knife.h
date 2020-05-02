#pragma once
#include "GameObject.h"
#include "Light.h"
#include "Mario.h"

#define KNIFE_BBOX_WIDTH  32
#define KNIFE_BBOX_HEIGHT 16

#define KNIFE_STATE_RIGHT 		100
#define KNIFE_STATE_LEFT 		200
#define KNIFE_STATE_DISAPPEAR	300

#define KNIFE_ANI_ATK_RIGHT 0
#define KNIFE_ANI_ATK_LEFT  1

#define KNIFE_ATTACK_RANGE 50

class CKnife : public CGameObject{
public:
	static CKnife* instance;
	static CKnife* GetInstance();

	// nhung bien nay co gia tri chi khi ma simon attack
	float simonPosX;
	float simonPosY;
	int simonCurrentFrame;

public:
    CKnife();
	~CKnife();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

    void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};