#include "Light.h"

CLight::CLight(){
	this->tag = 1;
}

CLight::~CLight(){

}

void CLight::Render(){
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CLight::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + LIGHT_BBOX_WIDTH;
	b = y + LIGHT_BBOX_HEIGHT;
}