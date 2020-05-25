#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	tag = -1;
	isDead = false;
	isCollision = true;

	ny2 = 0;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects){
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

void CGameObject::filterUnwantedColliableObject(
	vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> &result){
	vector<LPGAMEOBJECT> filter;

	for (UINT i = 0; i < coObjects->size(); i++){
		if(coObjects->at(i)->tag != 1 
		  && coObjects->at(i)->tag != 8){
			if (coObjects->at(i)->isCollision == true) {
				filter.push_back(coObjects->at(i));
			}
		}
	}

	// can xem lai cach lam cho nay do bao loi tren error list.
	result = filter;
}

/*
	Calculate potential collisions with the list of colliable objects 
	
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects, 
	vector<LPCOLLISIONEVENT> &coEvents){

	float l, t, r, b, l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);

	for (UINT i = 0; i < coObjects->size(); i++){
		
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		coObjects->at(i)->GetBoundingBox(l1, t1, r1, b1);

		if(e->t > 0 && e->t <= 1.0f){
			coEvents.push_back(e);
		}else if(this->tag == 3 // Xet truong hop cho cay roi
		&&( (abs(l - l1) < abs(l - r)) && ( abs(t-b1) < (abs(t1-b1) + abs(t-b)) ) )){
			// DebugOut(L"[INFO] Ham co chay dieu kien cay roi vao");
			e->t = 0.3;
			coEvents.push_back(e);
		}else{delete e;}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++){
		LPCOLLISIONEVENT c = coEvents[i];
		if(c->t < min_tx && c->nx != 0){
			min_tx = c->t; nx = c->nx; 
			min_ix = i; rdx = c->dx;
		}
		if (c->t < min_ty  && c->ny != 0){
			min_ty = c->t; ny = c->ny; 
			min_iy = i; rdy = c->dy;
		}
		else if (c->t < min_tx && c->nx == 0){
			min_ix = i;
			// DebugOut(L"[INFO] Ham co chay dieu kien cay roi vao");
		}// xet va cham tu ben trong

		if (min_ix>=0){
			coEventsResult.push_back(coEvents[min_ix]);
		}
		if (min_iy>=0){
			coEventsResult.push_back(coEvents[min_iy]);
		} 	
	}
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}


CGameObject::~CGameObject()
{

}