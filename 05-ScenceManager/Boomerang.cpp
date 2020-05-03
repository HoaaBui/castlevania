#include "Boomerang.h"
#include "Light.h"
#include "Mario.h"

CBoomerang::CBoomerang(){
	this->simonCurrentFrame = -1;
	// this->simonPosX = 0.0f;
	// this->simonPosY = 0.0f;
	this->tag = 5;
}

CBoomerang::~CBoomerang(){

}

// CBoomerang * CBoomerang::instance = NULL;
// CBoomerang * CBoomerang::GetInstance()
// {
// 	if (instance == NULL)
// 	{
// 		instance = new CBoomerang();
// 	}
// 	return instance;
// }

void CBoomerang::Render(){
	// CBoomerang * Boomerang = CBoomerang::GetInstance();
	// int state = Boomerang->GetState();
	// float simonX = Boomerang->simonPosX;
	// float simonY = Boomerang->simonPosY;

	// animation_set->at(BOOMERANG_ANI_ATK)->Render(x, y);
	if(state == BOOMERANG_STATE_RIGHT){
		animation_set->at(BOOMERANG_ANI_ATK)->Render(x, y);
	}else if(state == BOOMERANG_STATE_LEFT){
		animation_set->at(BOOMERANG_ANI_ATK)->Render(x, y);
	}else if(state == BOOMERANG_STATE_DISAPPEAR){
		animation_set->at(BOOMERANG_ANI_ATK)->Render(10000, 10000);
	}
}

void CBoomerang::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
		case BOOMERANG_STATE_RIGHT:
			nx = 1;
			vx = 0.3f;
			break;
		case BOOMERANG_STATE_LEFT:
			nx = -1;
			vx = -0.3f;
			break;
		default:
			break;
	}
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CMario *mario = CMario::GetInstance();
	if(simonCurrentFrame == 2){
		if(nx>0){
			this->x = mario->x + 10;
			this->y = mario->y;
		}else{
			this->x = mario->x - 10;
			this->y = mario->y;
		}
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
				light->SetState(LIGHT_STATE_DEAD);
				this->state = BOOMERANG_STATE_DISAPPEAR;
				CMario *mario = CMario::GetInstance();
				this->x = 10000;
				this->y = 10000;
				mario->isUsedSubWeaponBoomerang = false;
			}
		}
	}else{
		CMario *mario = CMario::GetInstance();
		if (abs(mario->x - this->x) > BOOMERANG_ATTACK_RANGE 
		 	&& mario->isUsedSubWeaponBoomerang){
		 	mario->isUsedSubWeaponBoomerang = false;
		 	this->state = BOOMERANG_STATE_DISAPPEAR;
		 	this->x = 10000;
		 	this->y = 10000;
		}
		x += dx;
		y += dy;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}