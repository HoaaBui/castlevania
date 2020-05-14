#include "Whip.h"
#include "Utils.h"
#include "light.h"
#include "Mario.h"
#include "Heart.h"
#include "Knight.h"
#include "SmallCandle.h"

CWhip::CWhip(){
    this->simonCurrentFrame = -1;
	this->simonPosX = 0.0f;
	this->simonPosY = 0.0f;
	this->tag = 3;
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

void CWhip::Render(){
	CWhip * whip = CWhip::GetInstance();
	int state = whip->GetState();
	float simonX = whip->simonPosX;
	float simonY = whip->simonPosY;

	if(state == WHIP_STATE_RIGHT){
	      if(whip->simonCurrentFrame == 0){
			// DebugOut(L"[INFO] This is your current whip STATE when Simon 0 : %d\n", state);
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(0);
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX-25, simonY+14);
		  }else if(whip->simonCurrentFrame == 1){
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(1);
            animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX-25, simonY+14);
		  }else if(whip->simonCurrentFrame == 2){
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(2);
            animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX+50, simonY+13);
		  }else{
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(-1);
		  }
	}else if(state == WHIP_STATE_LEFT){
          if(whip->simonCurrentFrame == 0){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(0);
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX+45, simonY+11);
		  }else if(whip->simonCurrentFrame == 1){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(1);
            animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX+45, simonY+6);
		  }else if(whip->simonCurrentFrame == 2){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(2);
            animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX-70, simonY+13);
		  }else{
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(-1);
		  }
	}else if(state == WHIP_STATE_DISAPPEAR){
		animation_set->at(0)->SetCurrentFrame(-1);
		animation_set->at(1)->SetCurrentFrame(-1);
		animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->Render(10000, 10000);
		animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->Render(10000, 10000);
	}

}

void CWhip::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + WHIP_BBOX_WIDTH;
	b = y + WHIP_BBOX_HEIGHT;
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CWhip * whip = CWhip::GetInstance();
	if(whip->simonCurrentFrame == 2){
		if(whip->nx>0){
			this->x = whip->simonPosX + 50;
			this->y = whip->simonPosY + 13;
		}else{
			this->x = whip->simonPosX - 70;
			this->y = whip->simonPosY + 13;
		}
	}else{
		this->x = 10000;
		this->y = 10000;
		// Khong xet va cham cho cai roi nua
	}

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT> filterCoObjs;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size()!=0){
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;
		
		for (UINT i = 0; i < coEventsResult.size(); i++){
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CLight *>(e->obj)){
				CLight *light = dynamic_cast<CLight *>(e->obj);
				if (e->nx == 0){
					light->SetState(LIGHT_STATE_DEAD);
				}
			}
			if (dynamic_cast<CHeart *>(e->obj)){
				CHeart *heart = dynamic_cast<CHeart *>(e->obj);
				heart->isCollision = true;
			}
			if (dynamic_cast<CKnight *>(e->obj)){
				CKnight *knight = dynamic_cast<CKnight *>(e->obj);
				knight->state = KNIGHT_STATE_DEAD;
			}
			if (dynamic_cast<CSmallCandle *>(e->obj)){
				CSmallCandle *smallCandle = dynamic_cast<CSmallCandle *>(e->obj);
				smallCandle->state = SMALL_CANDLE_STATE_DEAD;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}