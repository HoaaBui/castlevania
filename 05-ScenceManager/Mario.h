#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "knife.h"

#define MARIO_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE					0
#define MARIO_STATE_WALKING_RIGHT			100
#define MARIO_STATE_WALKING_LEFT			200
#define MARIO_STATE_JUMP					300
#define MARIO_STATE_DIE						400
#define MARIO_STATE_ATTACK_STAND_RIGHT		500
#define MARIO_STATE_ATTACK_STAND_LEFT		600
#define MARIO_STATE_SIT_RIGHT				700
#define MARIO_STATE_SIT_LEFT				800
#define MARIO_STATE_ATTACK_SIT_RIGHT		900
#define MARIO_STATE_ATTACK_SIT_LEFT			1000

#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_SMALL_IDLE_RIGHT			2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_BIG_ATTACK_STAND_RIGHT	9
#define MARIO_ANI_BIG_ATTACK_STAND_LEFT		10

#define MARIO_ANI_BIG_SIT_RIGHT				11
#define MARIO_ANI_BIG_SIT_LEFT				12

#define MARIO_ANI_BIG_ATTACK_SIT_RIGHT		13
#define MARIO_ANI_BIG_ATTACK_SIT_LEFT		14

#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  40
#define MARIO_BIG_BBOX_HEIGHT 63

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
public:
	int level;
	int untouchable;
	DWORD untouchable_start;

    bool isAttack;
	bool isSit;
	bool isUsedSubWeapon;
	DWORD attackTime;
	// CWhip* mainWeap;

	// static CMario* instance;
	// static CMario* GetInstance();
public: 
	CMario();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	// void AddWeapon(CWhip* weapon);
	// void SetWhip(CWhip *whip) { mainWeap = whip; };
	// CWhip *GetWhip() { return this->mainWeap; };

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};