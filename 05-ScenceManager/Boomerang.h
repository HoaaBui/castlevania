#pragma once
#include "GameObject.h"
#include "Light.h"
// #include "Mario.h"

using namespace std;
#define BOOMERANG_BBOX_WIDTH  28
#define BOOMERANG_BBOX_HEIGHT 28

#define BOOMERANG_STATE_RIGHT 		100
#define BOOMERANG_STATE_LEFT 		200
#define BOOMERANG_STATE_DISAPPEAR	300

#define BOOMERANG_ANI_ATK 0

#define BOOMERANG_ATTACK_RANGE 200

class CBoomerang : public CGameObject{
public:
	// static CBoomerang* instance;
	// static CBoomerang* GetInstance();

	// nhung bien nay co gia tri chi khi ma simon attack
	// float simonPosX;
	// float simonPosY;
	int simonCurrentFrame;
	bool isComingBack;

public:
    CBoomerang();
	~CBoomerang();

	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);

    void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};