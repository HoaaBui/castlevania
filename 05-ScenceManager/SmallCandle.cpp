#include "SmallCandle.h"

CSmallCandle::CSmallCandle(){
	this->tag = 8;
	this->isDead = false;
	this->lifeTime = 0;
	SetState(SMALL_CANDLE_STATE_ALIVE);
}

CSmallCandle::~CSmallCandle(){

}

void CSmallCandle::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
		case SMALL_CANDLE_STATE_ALIVE:
			break;
		case SMALL_CANDLE_STATE_DEAD:
			break;
		default:
			break;
	}
}

void CSmallCandle::Render(){
	int ani = ANIMATION_SMALL_CANDLE_ALIVE;
	int currentFrame = -1;

	if(this->state == SMALL_CANDLE_STATE_DEAD){
		ani = ANIMATION_SMALL_CANDLE_DEAD;
		animation_set->at(1)->Render(x, y);
		animation_set->at(1)->GetCurrentFrame(currentFrame);
	}else if(this->state == SMALL_CANDLE_STATE_ALIVE){
		animation_set->at(0)->Render(x, y);
	}

	if(currentFrame == 1){
		currentFrame = -1;
		this->state = SMALL_CANDLE_STATE_DISAPPEAR;
		animation_set->at(1)->Render(10000, 10000);
		animation_set->at(0)->Render(10000, 10000);
	}
}

void CSmallCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	if(this->state == SMALL_CANDLE_STATE_DISAPPEAR){
		x = 10000;
		y = 10000;
	}
}

void CSmallCandle::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + SMALL_CANDLE_BBOX_WIDTH;
	b = y + SMALL_CANDLE_BBOX_HEIGHT;
}