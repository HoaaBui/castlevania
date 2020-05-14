#pragma once
#include "GameObject.h"

#define SMALL_CANDLE_BBOX_WIDTH  16
#define SMALL_CANDLE_BBOX_HEIGHT 32

#define SMALL_CANDLE_STATE_ALIVE		100
#define SMALL_CANDLE_STATE_DEAD			200
#define SMALL_CANDLE_STATE_DISAPPEAR	300

#define ANIMATION_SMALL_CANDLE_ALIVE 	0
#define ANIMATION_SMALL_CANDLE_DEAD 	1

class CSmallCandle : public CGameObject{
public:
	DWORD lifeTime;
public:
	CSmallCandle();
	~CSmallCandle();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
};