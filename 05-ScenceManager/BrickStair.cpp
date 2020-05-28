#include "BrickStair.h"

void CBrickStair::Render(){
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrickStair::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + BRICK_STAIR_BBOX_WIDTH;
	b = y + BRICK_STAIR_BBOX_HEIGHT;
}