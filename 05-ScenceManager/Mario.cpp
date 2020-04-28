#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Whip.h"

CMario::CMario() : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isAttack = false;
	isSit = false;
	attackTime = 0;
	tag = 2;
	// mainWeap = new CWhip();
}

CMario* CMario::instance = NULL;

CMario * CMario::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CMario();
	}
	return instance;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT> filterCoObjs;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE){
		CalcPotentialCollisions(coObjects, coEvents);
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//Xet va cham giua roi va cay nen
	if(this->isAttack){
		int currentFrame = -1;
		animation_set->at(MARIO_ANI_BIG_ATTACK_STAND_RIGHT)->GetCurrentFrame(currentFrame);
		if(currentFrame == 2){
			// coEvents.clear();//clear
			// coEventsResult.clear();
			if(nx>0){
				CWhip::GetInstance()->SetPosition(this->x+50, this->y+13);	
			}
		}
	}
}

void CMario::Render(){
	int ani = -1;
 
    float simonPosX = 0.0f, simonPosY = 0.0f, xxx = 0.0f, yyy = 0.0f;
	GetPosition(simonPosX, simonPosY);

	if(this->isAttack){
		attackTime += dt;
		if(nx>0){
			ani = MARIO_ANI_BIG_ATTACK_STAND_RIGHT;
		}else{
			ani = MARIO_ANI_BIG_ATTACK_STAND_LEFT;
		}
	}else if(this->isSit){
		if(nx>0){
			ani = MARIO_ANI_BIG_SIT_RIGHT;
		}else{
			ani = MARIO_ANI_BIG_SIT_LEFT;
		}
	}else{
		if(state == MARIO_STATE_DIE){
			ani = MARIO_ANI_DIE;
		}else if(level == MARIO_LEVEL_BIG){
			if (vx == 0){
				// if(state == MARIO_STATE_ATTACK_STAND_RIGHT){
				// 	ani = MARIO_ANI_BIG_ATTACK_STAND_RIGHT;
				// }else if(state == MARIO_STATE_ATTACK_STAND_LEFT){
				// 	ani = MARIO_ANI_BIG_ATTACK_STAND_LEFT;
				// }else{
					if (nx>0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else ani = MARIO_ANI_BIG_IDLE_LEFT;
				// }
			}else if(vx > 0){
				ani = MARIO_ANI_BIG_WALKING_RIGHT; 
			}else{
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		}
	}

	if(attackTime >= 200){
		SetState(MARIO_STATE_IDLE);
		attackTime = 0;
		this->isAttack = false;
		
		if(this->isAttack == false){
			CWhip::GetInstance()->SetState(WHIP_STATE_DISAPPEAR);
		    CWhip::GetInstance()->simonCurrentFrame = -1;
		}
	}
    
	int currentFrame = -1;
	animation_set->at(ani)->GetCurrentFrame(currentFrame);
    CWhip::GetInstance()->simonPosX = this->x;
	CWhip::GetInstance()->simonPosY = this->y;

	if(this->isAttack){
        CWhip::GetInstance()->simonCurrentFrame = currentFrame;
		if(nx>0){
           CWhip::GetInstance()->SetState(WHIP_STATE_RIGHT);
		}else{
           CWhip::GetInstance()->SetState(WHIP_STATE_LEFT);
		}
	}

	// else if (level == MARIO_LEVEL_SMALL)
	// {
	// 	if (vx == 0)
	// 	{
	// 		if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
	// 		else ani = MARIO_ANI_SMALL_IDLE_LEFT;
	// 	}
	// 	else if (vx > 0)
	// 		ani = MARIO_ANI_SMALL_WALKING_RIGHT;
	// 	else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	// }

	int alpha = 255;
	if (untouchable) alpha = 128;
	DebugOut(L"[INFO] The number of Mario animation is %d \n", ani);
	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_ATTACK_STAND_RIGHT: 
		vx = 0;
		nx = 1;
		this->isAttack = true;
		break;
	case MARIO_STATE_ATTACK_STAND_LEFT: 
		vx = 0;
		nx = -1;
		this->isAttack = true;
		break;
	case MARIO_STATE_SIT_RIGHT:
		vy = 0;
		vx = 0;
		this->isSit = true;
		break;
	case MARIO_STATE_SIT_LEFT:
		vy = 0;
		vx = 0;
		this->isSit = true;
		break;
	case MARIO_STATE_JUMP: 
		vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG) {
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	} else {
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

