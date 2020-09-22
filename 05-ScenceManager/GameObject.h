#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Sprites.h"
#include "Animations.h"



using namespace std; 
#define ID_TEX_BBOX -100		// special texture to draw object bounding box


class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;
struct CCollisionEvent;
typedef CCollisionEvent *LPCOLLISIONEVENT;
 struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float ny2;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &ObjectA , LPCOLLISIONEVENT &ObjectB )
	{
		return ObjectA->t < ObjectB->t;
	}
};



// Init class CGameObject 

class CGameObject
{
public:

	float x; 
	float y;
	
	// Quảng đường di chuyển điểm X, Y
	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	// Vận tốc di chuyển điểm X, Y
	float vx;
	float vy;

	int nx;	 

	float ny2;

	int state;
	 
	// thời gian di chuyển = tốc độ di chuyển của object
	DWORD dt; 

	LPANIMATION_SET animation_set;

	int tag; // dán nhãn cho object de biet thang nao di qua dk hay ko?
	bool isDead; // Xét điểu điện hiện diện của vật thể 
	bool isCollision; // Xét điều kiện vật thể có va chạm hay không
	bool isHitGround; // Xét vật thể có đang trên mặt đất hay không 

// menthod for game object 
public: 
	
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { this->animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);
	CGameObject();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	void filterUnwantedColliableObject(vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> &result);

	~CGameObject();
};

