#include "WhipIcon.h"

CWhipIcon::CWhipIcon(){
	this->tag = 9;
	this->isCollision = false;
	this->state = WHIP_ICON_STATE_ALIVE;
}

CWhipIcon::~CWhipIcon(){}

void CWhipIcon::Render(){
	if(this->state == WHIP_ICON_STATE_ALIVE){
		animation_set->at(0)->Render(x, y);
	}
}

void CWhipIcon::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + WHIP_ICON_BBOX_WIDTH;
	b = y + WHIP_ICON_BBOX_HEIGHT;
}