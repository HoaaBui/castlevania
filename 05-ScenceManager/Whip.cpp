#include "Whip.h"
#include "Utils.h"
#include "light.h"
#include "Mario.h"

CWhip::CWhip(){
    this->simonCurrentFrame = -1;
	this->simonPosX = 0.0f;
	this->simonPosY = 0.0f;
}

CWhip::~CWhip(){

}

CWhip * CWhip::instance = NULL;
CWhip * CWhip::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CWhip();
	}
	return instance;
}

void CWhip::SetState(int state){
	
	CGameObject::SetState(state);
	
	switch (state){
	case WHIP_STATE_RIGHT:
		nx = 1;
		break;
	case WHIP_STATE_LEFT:
		nx = -1;
		break;
	default:
		break;
	}
}

void CWhip::Render()
{
	// int currentFrame = -1;
	// animation_set->at(0)->GetCurrentFrame(currentFrame);
	// DebugOut(L"[INFO] This is your current whip frame : %d\n", currentFrame);
	// animation_set->at(1)->Render(x, y);
	// //RenderBoundingBox();

    // CWhip * whip = CWhip::GetInstance();
	// int state = whip->GetState();
    // DebugOut(L"[INFO] This is your current whip STATE : %d\n", state);
	// if(state == WHIP_STATE_RIGHT){
    //    animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->Render(x, y);
	// }else if(state == WHIP_STATE_LEFT){
    //   animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->Render(x, y);
	// }else if(state == WHIP_STATE_DISAPPEAR){
	// 	animation_set->at(0)->SetCurrentFrame(-1);
	// 	animation_set->at(1)->SetCurrentFrame(-1);
	// 	animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->Render(10000, 10000);
	// 	animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->Render(10000, 10000);
	// }

	CWhip * whip = CWhip::GetInstance();
	int state = whip->GetState();
	float simonX = whip->simonPosX;
	float simonY = whip->simonPosY;
	float whipX = 0.0f;
	// float whipY = 0.f;
    // DebugOut(L"[INFO] This is your current whip STATE : %d\n", state);

	if(state == WHIP_STATE_RIGHT){
	      if(whip->simonCurrentFrame == 0){
			// DebugOut(L"[INFO] This is your current whip STATE when Simon 0 : %d\n", state);
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(0);
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX-25, simonY+14);
		  }else if(whip->simonCurrentFrame == 1){
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(1);
            animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX-25, simonY+17);
		  }else if(whip->simonCurrentFrame == 2){
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(2);
            animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->renderOnlyCurrentFrame(simonX+50, simonY+13);
		  }else{
			animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(-1);
		  }
	}else if(state == WHIP_STATE_LEFT){
          if(whip->simonCurrentFrame == 0){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(0);
			whip->x = simonX+45;
			whip->y = simonY+11;
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX+45, simonY+11);
		  }else if(whip->simonCurrentFrame == 1){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(1);
			whip->x = simonX+45;
			whip->y = simonY+9;
            animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX+45, simonY+9);
		  }else if(whip->simonCurrentFrame == 2){
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(2);
			whip->x = simonX-70;
			whip->y = simonY+13;
            animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->renderOnlyCurrentFrame(simonX-70, simonY+13);
		  }else{
			whip->x = 10000;
			whip->y = 10000;
			animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(-1);
		  }
	}else if(state == WHIP_STATE_DISAPPEAR){
		animation_set->at(0)->SetCurrentFrame(-1);
		animation_set->at(1)->SetCurrentFrame(-1);
		animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->Render(10000, 10000);
		animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->Render(10000, 10000);
	}

}

void CWhip::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + WHIP_BBOX_WIDTH;
	b = y + WHIP_BBOX_HEIGHT;
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario *mario = CMario::GetInstance();
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

				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	// if (colliable_objects->size() != 0){
	// 	vector<LPCOLLISIONEVENT> coEvents;
	// 	vector<LPCOLLISIONEVENT> coEventsResult;
	// 	coEvents.clear();
	// 	CalcPotentialCollisions(colliable_objects, coEvents);
	// 	if (coEvents.size() != 0)
	// 	{
	// 		float min_tx, min_ty, nx = 0, ny;
	// 		float rdx = 0; 
	// 		float rdy = 0;
	// 		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	// 		for (UINT i = 0; i < coEventsResult.size(); i++)
	// 		{
	// 			LPCOLLISIONEVENT e = coEventsResult[i];
	// 			if (dynamic_cast<CLight *>(e->obj))
	// 			{
	// 				//				if (timeToBound >= 0.0075)
	// 				//{
	// 					CLight *light = dynamic_cast<CLight *>(e->obj);
	// 					if (e->nx == 0)
	// 					{
	// 						// if (light->GetState() != LIGHT_STATE_DESTROY)
	// 						// {
	// 						// 	light->SetState(LIGHT_STATE_DESTROY);
	// 						// }
	// 					}
	// 				//}
	// 			}
	// 		}
	// 		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	// 	}
	// }
}