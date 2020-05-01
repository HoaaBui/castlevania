#include "Knife.h"

CKnife::~CKnife(){

}

CKnife::CKnife(){

}


void CKnife::Render(){
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CKnife::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + KNIFE_BBOX_WIDTH;
	b = y + KNIFE_BBOX_HEIGHT;
}

void CKnife::SetState(int state){
	
	CGameObject::SetState(state);
	
	// switch (state){
	// 	case WHIP_STATE_RIGHT:
	// 		nx = 1;
	// 		break;
	// 	case WHIP_STATE_LEFT:
	// 		nx = -1;
	// 		break;
	// 	default:
	// 		break;
	// }
}
