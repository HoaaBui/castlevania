
#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "Knife.h"
#include "Boomerang.h"

#define SIMON_WALKING_SPEED					0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y					0.65f
#define SIMON_JUMP_DEFLECT_SPEED 			0.2f
#define SIMON_GRAVITY						0.002f
#define SIMON_DIE_DEFLECT_SPEED	 			0.5f
#define SIMON_HITED_DEFLECT_SPEED			0.5f


#define SIMON_STATE_IDLE					0
#define SIMON_STATE_WALKING_RIGHT			100
#define SIMON_STATE_WALKING_LEFT			200
#define SIMON_STATE_JUMP					300
#define SIMON_STATE_DIE						400
#define SIMON_STATE_ATTACK_STAND_RIGHT		500
#define SIMON_STATE_ATTACK_STAND_LEFT		600
#define SIMON_STATE_SIT_RIGHT				700
#define SIMON_STATE_SIT_LEFT				800
#define SIMON_STATE_ATTACK_SIT_RIGHT		900
#define SIMON_STATE_ATTACK_SIT_LEFT			1000
#define SIMON_STATE_TAKE_WEAPON				1100
#define SIMON_STATE_IS_HIT_LEFT				1200
#define SIMON_STATE_IS_HIT_RIGHT			1300

#define SIMON_ANI_IDLE_RIGHT			0
#define SIMON_ANI_IDLE_LEFT				1

#define SIMON_ANI_WALKING_RIGHT			4
#define SIMON_ANI_WALKING_LEFT			5

#define SIMON_ANI_ATTACK_STAND_RIGHT	9
#define SIMON_ANI_ATTACK_STAND_LEFT		10

#define SIMON_ANI_SIT_RIGHT				11
#define SIMON_ANI_SIT_LEFT				12

#define SIMON_ANI_ATTACK_SIT_RIGHT		13
#define SIMON_ANI_ATTACK_SIT_LEFT		14

#define SIMON_ANI_JUMP_RIGHT			15
#define SIMON_ANI_JUMP_LEFT				16

#define SIMON_ANI_TAKE_WEAPON_RIGHT		17
#define SIMON_ANI_TAKE_WEAPON_LEFT		18

#define SIMON_ANI_DIE					8

#define SIMON_BBOX_WIDTH  				40
#define SIMON_BBOX_HEIGHT 				63

#define SIMON_UNTOUCHABLE_TIME 			5000

#define SIMON_HP						160 



class CSimon : public CGameObject
{
public:
	int level;
	int healthPoint;
	int untouchable;
	DWORD untouchable_start;

	bool isAttack;
	bool isSit;
	bool isHitted;
	bool isUsedSubWeaponKnife;
	bool isUsedWhip;
	bool isUsedSubWeaponBoomerang;
	bool isJumped;
	bool isDead; 
	DWORD attackTime;
	// CWhip* mainWeap;
	CKnife *knife;
	CBoomerang *boomerang;
	CWhip *whip;

	bool isTouchingBrickStair;
	bool isGoingOnStair;
	float brickStairNx;
	float brickStairNy;

	static CSimon* instance;
	static CSimon* GetInstance();

	int simonCurrentFrame;
	
	// Comfirm wepon Siom can use
	bool canUseKnife;
	bool canUseBoomerang;
	bool canUseBottle; 
	bool canStopTime;


	bool isTakeWeapon;
	DWORD takeWeaponTime;

	bool isWalking;

public:
	CSimon();
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	// void AddWeapon(CWhip* weapon);
	// void SetWhip(CWhip *whip) { mainWeap = whip; };
	// CWhip *GetWhip() { return this->mainWeap; };
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};