#include "Light.h"

CLight::CLight(){
	this->tag = 1;
	this->isDead = false;
	this->lifeTime = 0;
	SetState(LIGHT_STATE_ALIVE);
}

CLight::~CLight(){

}

void CLight::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
		case LIGHT_STATE_ALIVE:
			break;
		case LIGHT_STATE_DEAD:
			// this->isGoingThrough = true;
			break;
		default:
			break;
	}
}

void CLight::Render(){
	int ani = ANIMATION_LIGHT_ALIVE;
	int currentFrame = -1;

	if(this->state == LIGHT_STATE_DEAD){
		ani = ANIMATION_LIGHT_DEAD;
		animation_set->at(1)->Render(x, y);
		// this->lifeTime += dt;
		animation_set->at(1)->GetCurrentFrame(currentFrame);
	}else if(this->state == LIGHT_STATE_ALIVE){
		animation_set->at(0)->Render(x, y);
	}

	// if(this->lifeTime > 200){
	if(currentFrame == 1){
		// lifeTime = 0;
		// isDead = true;
		currentFrame = -1;
		this->state = LIGHT_STATE_DISAPPEAR;
		animation_set->at(1)->Render(10000, 10000);
		animation_set->at(0)->Render(10000, 10000);
	}
	//RenderBoundingBox();
}

void CLight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	if(this->state == LIGHT_STATE_DISAPPEAR){
		x = 10000;
		y = 10000;
	}
}

void CLight::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + LIGHT_BBOX_WIDTH;
	b = y + LIGHT_BBOX_HEIGHT;
}