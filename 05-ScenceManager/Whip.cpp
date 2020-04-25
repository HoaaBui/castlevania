#include "Whip.h"
#include "Utils.h"

CWhip::CWhip(){
    
}

CWhip::~CWhip(){

}

CWhip * CWhip::instance = NULL;
CWhip * CWhip::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CWhip();
	}
	return instance;
}

void CWhip::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
	case WHIP_STATE_RIGHT:
		nx = 1;
		break;
	case WHIP_STATE_LEFT:
		nx = -1;
		break;
	default:
		break;
	}
}

void CWhip::Render()
{
	// int currentFrame = -1;
	// animation_set->at(0)->GetCurrentFrame(currentFrame);
	// DebugOut(L"[INFO] This is your current whip frame : %d\n", currentFrame);
	// animation_set->at(1)->Render(x, y);
	// //RenderBoundingBox();

    CWhip * whip = CWhip::GetInstance();
	int state = whip->GetState();
    DebugOut(L"[INFO] This is your current whip STATE : %d\n", state);
	if(state == WHIP_STATE_RIGHT){
       animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->Render(x, y);
	}else if(state == WHIP_STATE_LEFT){
       animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->Render(x, y);
	}else if(state == WHIP_STATE_DISAPPEAR){
		animation_set->at(0)->SetCurrentFrame(-1);
		animation_set->at(1)->SetCurrentFrame(-1);
	}

}

void CWhip::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + WHIP_BBOX_WIDTH;
	b = y + WHIP_BBOX_HEIGHT;
}