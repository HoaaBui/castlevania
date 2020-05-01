#include "Knife.h"
#include "Light.h"

CKnife::~CKnife(){
	this->simonCurrentFrame = -1;
	this->simonPosX = 0.0f;
	this->simonPosY = 0.0f;
	this->tag = 4;
}

CKnife::CKnife(){

}

CKnife * CKnife::instance = NULL;
CKnife * CKnife::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CKnife();
	}
	return instance;
}

void CKnife::Render(){
	CKnife * knife = CKnife::GetInstance();
	int state = knife->GetState();
	float simonX = knife->simonPosX;
	float simonY = knife->simonPosY;

	if(state == KNIFE_STATE_RIGHT){
		animation_set->at(KNIFE_ANI_ATK_RIGHT)->Render(simonX+50, simonY+13);
	}else if(state == KNIFE_STATE_LEFT){
		animation_set->at(KNIFE_ANI_ATK_LEFT)->Render(simonX-40, simonY+13);
	}else if(state == KNIFE_STATE_DISAPPEAR){
		animation_set->at(KNIFE_ANI_ATK_RIGHT)->Render(10000, 10000);
		animation_set->at(KNIFE_ANI_ATK_LEFT)->Render(10000, 10000);
	}
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
	
	switch (state){
		case KNIFE_STATE_RIGHT:
			nx = 1;
			break;
		case KNIFE_STATE_LEFT:
			nx = -1;
			break;
		default:
			break;
	}
}

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CKnife * knife = CKnife::GetInstance();
	if(knife->simonCurrentFrame == 2){
		if(knife->nx>0){
			this->x = knife->simonPosX + 50;
			this->y = knife->simonPosY + 13;
		}else{
			this->x = knife->simonPosX - 40;
			this->y = knife->simonPosY + 13;
		}
		x += dx;
		y += dy;
	}
	// else{
	// 	this->x = 10000;
	// 	this->y = 10000;
	// 	// Khong xet va cham cho cai roi nua
	// }

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
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}