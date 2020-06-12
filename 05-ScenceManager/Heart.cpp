#include "Heart.h"

CHeart::CHeart(){
	this->tag = 6;
	this->isCollision = false;
	this->state = HEART_STATE_ALIVE;
}

CHeart::~CHeart(){}

void CHeart::Render(){
	if(this->state == HEART_STATE_ALIVE && this->isCollision == true){
		animation_set->at(0)->Render(x, y);
	}
}

void CHeart::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + HEART_BBOX_WIDTH;
	b = y + HEART_BBOX_HEIGHT;
}