#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "light.h"

#define WHIP_BBOX_WIDTH  		50
#define WHIP_BBOX_HEIGHT 		20

#define WHIP_BBOX_WIDTH_1		60
#define WHIP_BBOX_HEIGHT_1		20

#define WHIP_BBOX_WIDTH_2		90
#define WHIP_BBOX_HEIGHT_2		20
// Can than cai vu bounding box nay

#define WHIP_STATE_RIGHT		100
#define WHIP_STATE_LEFT			200
#define WHIP_STATE_DISAPPEAR	300

#define ANIMATION_ATTACK_WHIP_RIGHT_ZERO 	0
#define ANIMATION_ATTACK_WHIP_LEFT_ZERO 	1
#define ANIMATION_ATTACK_WHIP_RIGHT_MOT 	2
#define ANIMATION_ATTACK_WHIP_LEFT_MOT 		3
#define ANIMATION_ATTACK_WHIP_RIGHT_HAI 	4
#define ANIMATION_ATTACK_WHIP_LEFT_HAI 		5

#define WHIP_LEVEL_0				0
#define WHIP_LEVEL_1				1
#define WHIP_LEVEL_2				2

class CWhip : public CGameObject
{
public:
    static CWhip* instance;
	static CWhip* GetInstance();

	// nhung bien nay co gia tri chi khi ma simon attack
	float simonPosX;
	float simonPosY;
	int simonCurrentFrame;

	int level; // cap do cua cay roi
	
public:
    CWhip();
	~CWhip();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetState(int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};