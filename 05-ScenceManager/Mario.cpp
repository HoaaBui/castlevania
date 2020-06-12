#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Whip.h"
#include "Knife.h"
#include "Heart.h"
#include "BrickStair.h"
#include "Brick.h"
#include "WhipIcon.h"

CMario::CMario() : CGameObject(){
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isAttack = false;
	isSit = false;
	isUsedSubWeapon = false;
	isUsedSubWeaponBoomerang = false;
	attackTime = 0;
	tag = 2;
	isJumped = false;
	knife = new CKnife();
	whip = new CWhip();
	boomerang = new CBoomerang();

	isTouchingBrickStair = false;
	isGoingOnStair = false;
}

CMario* CMario::instance = NULL;

CMario * CMario::GetInstance(){
	if (instance == NULL){
		instance = new CMario();
	}
	return instance;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects){
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPGAMEOBJECT> filterCoObjs;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	filterUnwantedColliableObject(coObjects,filterCoObjs);

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE){
		CalcPotentialCollisions(&filterCoObjs, coEvents);
	}

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME){
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if(coEvents.size()==0){
		x += dx; 
		y += dy;
	}else{
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

			if (dynamic_cast<CGoomba *>(e->obj)){ // if e->obj is Goomba 
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if(e->ny < 0){
					if(goomba->GetState()!= GOOMBA_STATE_DIE){
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}else if(e->nx != 0){
					if(untouchable==0){
						if (goomba->GetState()!=GOOMBA_STATE_DIE){
							if (level > MARIO_LEVEL_SMALL){
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}else SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if(dynamic_cast<CPortal *>(e->obj)){
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				DebugOut(L"[INFO] Switching to scene %d", p->GetSceneId());
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			if (dynamic_cast<CHeart *>(e->obj)){
				CHeart *heart = dynamic_cast<CHeart *>(e->obj);
				heart->state = HEART_STATE_DEAD;
				heart->isCollision = false;
			}else if (dynamic_cast<CBrickStair *>(e->obj)){
				CBrickStair *bStair = dynamic_cast<CBrickStair *>(e->obj);
				// this->isTouchingBrickStair = true;
				DebugOut(L"[INFO] Co chay vo ham nay abcd\n");
				// if (e->nx != 0 || e->ny != 0){
				// 	isTouchingBrickStair = true;
				// 	brickStairNy = bStair->ny2;
				// 	brickStairNx = bStair->nx;

				// }
			}else if(dynamic_cast<CBrick *>(e->obj)){
				this->isJumped = false;
			}

			if (dynamic_cast<CWhipIcon *>(e->obj)){
				// DebugOut(L"[INFO] Co chay vao cai icon: \n");
				CWhipIcon *wIcon = dynamic_cast<CWhipIcon *>(e->obj);
				wIcon->state = WHIP_ICON_STATE_DEAD;
				wIcon->isCollision = false;

				// Xet level cho cay roi
				if(whip->level == 0){
					whip->level += 1;
				}else if(whip->level == 1){
					whip->level += 1;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
	if(this->isUsedWhip && whip != NULL){
		whip->Update(dt,coObjects);
	}
	if(this->isUsedSubWeapon && knife != NULL){
		knife->Update(dt,coObjects);
	}
	if(this->isUsedSubWeaponBoomerang){
		boomerang->Update(dt,coObjects);
	}
}

void CMario::Render(){
	int ani = -1;
 
    float simonPosX = 0.0f, simonPosY = 0.0f;
	GetPosition(simonPosX, simonPosY);

	if(this->isAttack){
		attackTime += dt;
		if(nx>0){
			if(this->isSit == true){
				ani = MARIO_ANI_BIG_ATTACK_SIT_RIGHT;
			}else if(this->isSit == false){
				ani = MARIO_ANI_BIG_ATTACK_STAND_RIGHT;
			}
		}else if(nx<0){
			if(this->isSit == true){
				ani = MARIO_ANI_BIG_ATTACK_SIT_LEFT;
			}else if(this->isSit == false){
				ani = MARIO_ANI_BIG_ATTACK_STAND_LEFT;
			}
		}
	}else if(this->isSit && this->isAttack == false){
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
				if (nx>0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}else if(vx > 0){
				ani = MARIO_ANI_BIG_WALKING_RIGHT; 
			}else{
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		}
	}

	int currentFrame = -1;
	animation_set->at(ani)->GetCurrentFrame(currentFrame);

	// con so 220 la do mo dai
	if(attackTime >= 300){
		animation_set->at(MARIO_ANI_BIG_ATTACK_STAND_RIGHT)->SetCurrentFrame(-1);
		animation_set->at(MARIO_ANI_BIG_ATTACK_STAND_LEFT)->SetCurrentFrame(-1);
		whip->animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(-1);
		whip->animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(-1);

		attackTime = 0.0f;
		this->isUsedWhip = false;
		this->isAttack = false;
		this->isSit = false;
		SetState(MARIO_STATE_IDLE);
		animation_set->at(ani)->SetCurrentFrame(-1);
		this->state = MARIO_STATE_IDLE;
	}

	boomerang->simonCurrentFrame = currentFrame;
	boomerang->nx = nx;

	if(this->isUsedSubWeapon){
		if(nx>0){
           	knife->SetState(KNIFE_STATE_RIGHT);
		}else{
           	knife->SetState(KNIFE_STATE_LEFT);
		}	
		knife->Render();
	}

	if(this->isUsedSubWeaponBoomerang){
		if(nx>0){
           	boomerang->SetState(BOOMERANG_STATE_RIGHT);
		}else{
           	boomerang->SetState(BOOMERANG_STATE_LEFT);
		}	
		boomerang->Render();
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	int curFrame = -1;
	animation_set->at(ani)->GetCurrentFrame(curFrame);

	animation_set->at(ani)->Render(x, y, alpha);	

	whip->simonCurrentFrame = currentFrame;
	whip->simonPosX = x;
	whip->simonPosY = y;
	whip->isSimonSit = this->isSit;

	if(this->isAttack && this->isUsedWhip){
		if(nx>0){
           	whip->SetState(WHIP_STATE_RIGHT);
		}else{
           	whip->SetState(WHIP_STATE_LEFT);
		}	

		// // whip->Render();
		// DebugOut(L"[INFO] This is your Simon Attack current frame: %d \n",currentFrame);
		// whip->simonCurrentFrame = currentFrame;
		// whip->simonPosX = x;
		// whip->simonPosY = y;
		// whip->isSimonSit = this->isSit;
		whip->Render();
	}
}

void CMario::SetState(int state){
	CGameObject::SetState(state);

	switch (state){
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

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom){
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

