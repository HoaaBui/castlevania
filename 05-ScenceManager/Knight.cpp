#include "Knight.h"

CKnight::CKnight(){
	SetState(KNIGHT_STATE_WALKING_RIGHT);
}

void CKnight::GetBoundingBox(float &left, float &top, float &right, float &bottom){
	left = x;
	top = y;
	right = x + KNIGHT_BBOX_WIDTH;
}

void CKnight::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx > 0 && x > 254) {
		x = 254; vx = -vx;
        this->state = KNIGHT_STATE_WALKING_LEFT;
	}

	if (vx < 0 && x < 78) {
		x = 78; vx = -vx;
        this->state = KNIGHT_STATE_WALKING_RIGHT;
	}
}

void CKnight::Render()
{
	int ani = 0;
    if(this->state == KNIGHT_STATE_WALKING_RIGHT){
        ani = 0;
    }else{
        ani = 1;
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
