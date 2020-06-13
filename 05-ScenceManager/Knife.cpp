#include "Knife.h"
#include "Light.h"
#include "Mario.h"
#include "Heart.h"
#include "WhipIcon.h"

CKnife::CKnife(){
	this->simonCurrentFrame = -1;
	this->simonPosX = 0.0f;
	this->simonPosY = 0.0f;
	this->tag = 4;
	this->count = 0;
}

CKnife::~CKnife(){}

CKnife * CKnife::instance = NULL;
CKnife * CKnife::GetInstance(){
	if (instance == NULL){
		instance = new CKnife();
	}
	return instance;
}

void CKnife::filterUnwantedColliableObjectForKnife(
	vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> &result){
	vector<LPGAMEOBJECT> filter;

	// for (UINT i = 0; i < coObjects->size(); i++){
	// 	if (coObjects->at(i)->isCollision == true) {
	// 			filter.push_back(coObjects->at(i));
	// 	}
	// }

	for (UINT i = 0; i < coObjects->size(); i++){
		if(coObjects->at(i)->tag != 6){
			if (coObjects->at(i)->isCollision == true){
				filter.push_back(coObjects->at(i));
			}
		}
	}

	result = filter;
}

void CKnife::Render(){
	if(state == KNIFE_STATE_RIGHT && this->count == 1){
		animation_set->at(KNIFE_ANI_ATTACK_RIGHT)->Render(x+30, y+17);
	}else if(state == KNIFE_STATE_LEFT && this->count == 1){
		animation_set->at(KNIFE_ANI_ATTACK_LEFT)->Render(x+10, y+17);
	}
}

void CKnife::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + KNIFE_BBOX_WIDTH;
	b = y + KNIFE_BBOX_HEIGHT;
}

void CKnife::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
		case KNIFE_STATE_RIGHT:
			nx = 1;
			vx = 0.3f;
			break;
		case KNIFE_STATE_LEFT:
			nx = -1;
			vx = -0.3f;
			break;
		default:
			break;
	}
}

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CMario *mario2 = CMario::GetInstance();

	if(mario2->simonCurrentFrame == 3 && this->count == 0){
		this->count = 1;
		DebugOut(L"[INFO] Code co chay vao lay lai vi tri: \n");
		if(nx>0){
			this->x = mario2->x;
			this->y = mario2->y;
		}else{
			this->x = mario2->x;
			this->y = mario2->y;
		}
	}

	if(mario2->isUsedSubWeapon == false){
		this->x = mario2->x;
		this->y = mario2->y;
	}

	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT> filterCoObjs;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	
	filterUnwantedColliableObjectForKnife(coObjects,filterCoObjs);
	CalcPotentialCollisions(&filterCoObjs, coEvents);

	CMario *mario = CMario::GetInstance();
	if (coEvents.size()!=0){
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

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
				// if (e->nx == 0){
					light->SetState(LIGHT_STATE_DEAD);
					this->state = KNIFE_STATE_DISAPPEAR;
					mario->isUsedSubWeapon = false;
					mario->simonCurrentFrame = -1;
					this->x = mario->x;
					this->y = mario->y;
					this->count = 0;
				// }
			}

			if (dynamic_cast<CHeart *>(e->obj)){
				CHeart *heart = dynamic_cast<CHeart *>(e->obj);
				heart->isCollision = false;
				mario->isUsedSubWeapon = false;
				mario->simonCurrentFrame = -1;
				this->x = mario->x;
				this->y = mario->y;
				this->count = 0;
			}

			if (dynamic_cast<CWhipIcon *>(e->obj)){
				// DebugOut(L"[INFO] Co chay vao cai icon: \n");
				CWhipIcon *wIcon = dynamic_cast<CWhipIcon *>(e->obj);
				wIcon->isCollision = false;
				mario->isUsedSubWeapon = false;
				mario->simonCurrentFrame = -1;
				this->x = mario->x;
				this->y = mario->y;
				this->count = 0;
			}
		}
	}else if(this->count == 1){
		CMario *mario5 = CMario::GetInstance();
		if (abs(mario5->x - this->x) > KNIFE_ATTACK_RANGE 
		 	&& mario5->isUsedSubWeapon == true){
		 	mario5->isUsedSubWeapon = false;
		 	this->state = KNIFE_STATE_DISAPPEAR;
		 	this->x = mario5->x;
		 	this->y = mario5->y;
			this->count = 0;
		}
		x += dx;
		y += dy;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}