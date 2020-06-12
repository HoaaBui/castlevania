#include "Whip.h"
#include "Utils.h"
#include "light.h"
#include "Heart.h"
#include "Knight.h"
#include "SmallCandle.h"
#include "Utils.h"
#include "WhipIcon.h"

CWhip::CWhip(){
    this->simonCurrentFrame = -1;
	this->simonPosX = 0.0f;
	this->simonPosY = 0.0f;
	this->tag = 3;
	this->level = WHIP_LEVEL_0;
}

CWhip::~CWhip(){}

CWhip * CWhip::instance = NULL;
CWhip * CWhip::GetInstance(){
	if (instance == NULL){
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

	int ani= -1; // xet animation cho cay roi
	if(this->level == 0){
		if(state == WHIP_STATE_RIGHT){
			ani = ANIMATION_ATTACK_WHIP_RIGHT_ZERO;
		}else if(state == WHIP_STATE_LEFT){
			ani = ANIMATION_ATTACK_WHIP_LEFT_ZERO;
		}
	}else if(this->level == 1){
		if(state == WHIP_STATE_RIGHT){
			ani = ANIMATION_ATTACK_WHIP_RIGHT_MOT;
		}else if(state == WHIP_STATE_LEFT){
			ani = ANIMATION_ATTACK_WHIP_LEFT_MOT;
		}
	}else if(this->level == 2){
		if(state == WHIP_STATE_RIGHT){
			ani = ANIMATION_ATTACK_WHIP_RIGHT_HAI;
		}else if(state == WHIP_STATE_LEFT){
			ani = ANIMATION_ATTACK_WHIP_LEFT_HAI;
		}
	}

	//xet position cho cay roi
	float whipX = 0.0f;
	float whipY = 0.0f;
	if(this->level == 0){
		if(state == WHIP_STATE_RIGHT){
			if(simonCurrentFrame == 1){
				whipX =  simonPosX-25; whipY =  simonPosY+14;
			}else if(simonCurrentFrame == 2){
				whipX =  simonPosX-22; whipY =  simonPosY+5;
			}else if(simonCurrentFrame == 3){
				whipX =  simonPosX+50; whipY =  simonPosY+13;
			}
		}else if(state == WHIP_STATE_LEFT){
			if(simonCurrentFrame == 1){
				whipX =  simonPosX+45; whipY =  simonPosY+11;
			}else if(simonCurrentFrame == 2){
				whipX =  simonPosX+38; whipY =  simonPosY+5;
			}else if(simonCurrentFrame == 3){
				whipX =  simonPosX-70; whipY =  simonPosY+13;
			}
		}
	}else if(this->level == 1){
		if(state == WHIP_STATE_RIGHT){
			if(simonCurrentFrame == 1){
				whipX =  simonPosX-25; whipY =  simonPosY+14;
			}else if(simonCurrentFrame == 2){
				whipX =  simonPosX-20; whipY =  simonPosY+5;
			}else if(simonCurrentFrame == 3){
				whipX =  simonPosX-30; whipY =  simonPosY+16;
			}
		}else if(state == WHIP_STATE_LEFT){
			if(simonCurrentFrame == 1){
				whipX =  simonPosX-80; whipY =  simonPosY+11;
			}else if(simonCurrentFrame == 2){
				whipX =  simonPosX-80; whipY =  simonPosY+5;
			}else if(simonCurrentFrame == 3){
				whipX =  simonPosX-80; whipY =  simonPosY+13;
			}
		}
	}
	// else if(this->level == 2){
	// 	if(state == WHIP_STATE_RIGHT){
	// 		if(simonCurrentFrame == 0){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}else if(simonCurrentFrame == 1){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}else if(simonCurrentFrame == 2){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}
	// 	}else if(state == WHIP_STATE_LEFT){
	// 		if(simonCurrentFrame == 0){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}else if(simonCurrentFrame == 1){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}else if(simonCurrentFrame == 2){
	// 			whipX =  ;
	// 			whipY =  ;
	// 		}
	// 	}
	// }

	if(state == WHIP_STATE_RIGHT){
	    if(simonCurrentFrame == 1){
			// DebugOut(L"[INFO] This is your whip current frame: %d \n",simonCurrentFrame);
			animation_set->at(ani)->SetCurrentFrame(0);
			animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
		}else if(simonCurrentFrame == 2){
			// DebugOut(L"[INFO] This is your whip current frame: %d \n",simonCurrentFrame);
			animation_set->at(ani)->SetCurrentFrame(1);
            animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
		}else if(simonCurrentFrame == 3){
			// DebugOut(L"[INFO] This is your whip current frame: %d \n",simonCurrentFrame);
			animation_set->at(ani)->SetCurrentFrame(2);
            animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
			// simonCurrentFrame = -1;
		}
	}else if(state == WHIP_STATE_LEFT){
        if(simonCurrentFrame == 1){
			animation_set->at(ani)->SetCurrentFrame(0);
			animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
		}else if(simonCurrentFrame == 2){
			animation_set->at(ani)->SetCurrentFrame(1);
            animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
		}else if(simonCurrentFrame == 3){
			animation_set->at(ani)->SetCurrentFrame(2);
            animation_set->at(ani)->renderOnlyCurrentFrame(whipX, whipY);
			// simonCurrentFrame = -1;
		}
	}

	// RenderBoundingBox();
}

void CWhip::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;

	if(this->level == 0){
		r = x + WHIP_BBOX_WIDTH;
		b = y + WHIP_BBOX_HEIGHT;
	}else if(this->level == 1){
		r = x + WHIP_BBOX_WIDTH_1;
		b = y + WHIP_BBOX_HEIGHT_1;
	}else if(this->level == 2){
		r = x + WHIP_BBOX_WIDTH_2;
		b = y + WHIP_BBOX_HEIGHT_2;
	}
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	if(simonCurrentFrame == 3){
		if(nx>0){
			if(level == 0){
				this->x = simonPosX + 50;
				this->y = simonPosY + 13;
			}else if(level == 1){
				this->x = simonPosX + 80;
				this->y = simonPosY + 13;
			}
		}else{
			if(level == 0){
				this->x = simonPosX - 30;
				this->y = simonPosY + 13;
			}else if(level == 1){
				this->x = simonPosX - 60;
				this->y = simonPosY + 13;
			}
		}
	}

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT> filterCoObjs;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size()!=0){
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
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
			if (dynamic_cast<CWhipIcon *>(e->obj)){
				DebugOut(L"[INFO] Co chay vao cai icon: \n");
				CWhipIcon *wIcon = dynamic_cast<CWhipIcon *>(e->obj);
				wIcon->isCollision = true;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}