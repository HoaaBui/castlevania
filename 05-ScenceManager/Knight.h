#pragma once
#include "GameObject.h"

#define KNIGHT_WALKING_SPEED 0.03f;

#define KNIGHT_BBOX_WIDTH 32
#define KNIGHT_BBOX_HEIGHT 64

#define KNIGHT_STATE_WALKING_RIGHT 100
#define KNIGHT_STATE_WALKING_LEFT 200
#define KNIGHT_STATE_DISAPPEAR 300
#define KNIGHT_STATE_DEAD 400

#define KNIGHT_ANI_WALKING_RIGHT 0
#define KNIGHT_ANI_WALKING_LEFT  1
#define KNIGHT_ANI_DEAD 		 2

class CKnight : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	CKnight();
	virtual void SetState(int state);
};