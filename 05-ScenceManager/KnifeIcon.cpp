#include "KnifeIcon.h"
#include "Brick.h"

CKnifeIcon::CKnifeIcon(){
	this->tag = 10;
	this->isCollision = false;
	this->state = KNIFE_ICON_STATE_ALIVE;
	this->isHitGround = false;
}

CKnifeIcon::~CKnifeIcon(){}

void CKnifeIcon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){
	CGameObject::Update(dt, coObjects);
	if(this->isCollision == true
	&& this->isHitGround == false){
		vy = KNIFE_ICON_GRAVITY*dt;
	}
	
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

			if(dynamic_cast<CBrick *>(e->obj)){
				this->isHitGround = true;
			}
		}
	}else{
		x += dx;
		y += dy;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKnifeIcon::Render(){
	if(this->state == KNIFE_ICON_STATE_ALIVE && this->isCollision == true){
		animation_set->at(0)->Render(x, y);
	}
}

void CKnifeIcon::GetBoundingBox(float &l, float &t, float &r, float &b){
	l = x;
	t = y;
	r = x + KNIFE_ICON_BBOX_WIDTH;
	b = y + KNIFE_ICON_BBOX_HEIGHT;
}