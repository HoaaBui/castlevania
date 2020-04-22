#pragma once
#include "GameObject.h"

#define WHIP_BBOX_WIDTH			60
#define WHIP_BBOX_HEIGHT		20

#define WHIP_BBOX_WIDTH_1		80
#define WHIP_BBOX_HEIGHT_1		20

#define WICON_BBOX_WIDTH		32
#define WICON_BBOX_HEIGHT		32

#define WHIP_STATE_RIGHT		100
#define WHIP_STATE_LEFT			200

#define WHIP_ANI_ATK_RIGHT		0
#define WHIP_ANI_ATK_LEFT		1
#define WHIP_ANI_ATK_RIGHT_1	2
#define WHIP_ANI_ATK_LEFT_1		3		


#define WHIP_LVL_0				0
#define WHIP_LVL_1				1

#define WHIP_GRAVITY			0.002f
class Whip :public CGameObject
{
	int level;
	static Whip* instance;
public:
	Whip();
	static Whip* GetInstance();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void SetLevel(int l) { level = l; };
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	~Whip();
};

class WhipIcon: public CGameObject
{
public:
	WhipIcon();
	void SetPosition(float x, float y) { this->x = x; this->y = y; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObject = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Render();
	~WhipIcon();
	
};

