#include "Knight.h"
#include "Game.h"
#include "Utils.h"

CKnight::CKnight(){
	this->tag = 7;
	SetState(KNIGHT_STATE_WALKING_RIGHT);
}

void CKnight::GetBoundingBox(float &left, float &top, float &right, float &bottom){
	left = x;
	top = y;
	right = x + KNIGHT_BBOX_WIDTH;
	bottom = y + KNIGHT_BBOX_HEIGHT;
}

void CKnight::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if(this->state == KNIGHT_STATE_DISAPPEAR) return;
	if(CGame::GetInstance()->current_scene == 2){
		DebugOut(L"[INFO] Co chay vo ham nay\n");
		if (vx > 0 && x > 254) {
			x = 254; vx = -vx;
        	this->state = KNIGHT_STATE_WALKING_LEFT;
		}
		if (vx < 0 && x < 78) {
			x = 78; vx = -vx;
        	this->state = KNIGHT_STATE_WALKING_RIGHT;
		}
	}else if(CGame::GetInstance()->current_scene == 4){
		if (vx > 0 && x > 670) {
			x = 670; vx = -vx;
        	this->state = KNIGHT_STATE_WALKING_LEFT;
		}
		if (vx < 0 && x < 635) {
			x = 635; vx = -vx;
        	this->state = KNIGHT_STATE_WALKING_RIGHT;
		}
	}
}

void CKnight::Render(){
	if(this->state == KNIGHT_STATE_DISAPPEAR) return;

	int ani = 0, currentFrame = -1;
    if(this->state == KNIGHT_STATE_WALKING_RIGHT){
        ani = KNIGHT_ANI_WALKING_RIGHT;
    }else if(this->state == KNIGHT_STATE_WALKING_LEFT){
        ani = KNIGHT_ANI_WALKING_LEFT;
    }
	if(this->state == KNIGHT_STATE_DEAD){
		ani = KNIGHT_ANI_DEAD;
		animation_set->at(KNIGHT_ANI_DEAD)->GetCurrentFrame(currentFrame);
	}

	if(currentFrame == 1){
		currentFrame = -1;
		this->state = KNIGHT_STATE_DISAPPEAR;
		animation_set->at(KNIGHT_ANI_WALKING_RIGHT)->Render(10000, 10000);
		animation_set->at(KNIGHT_ANI_WALKING_LEFT)->Render(10000, 10000);
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CKnight::SetState(int state){
	CGameObject::SetState(state);
	
    switch(state){
		case KNIGHT_STATE_WALKING_RIGHT: 
			vx = KNIGHT_WALKING_SPEED;
            break;
		case KNIGHT_STATE_WALKING_LEFT: 
			vx = -KNIGHT_WALKING_SPEED;
            break;
	}

}
