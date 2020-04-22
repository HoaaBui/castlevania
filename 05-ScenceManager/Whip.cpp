#include "Whip.h"

Whip::Whip()
{
	tag = 1;
	level = WHIP_LVL_0;
}
Whip* Whip::instance = NULL;
Whip * Whip::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Whip();
	}
	return instance;
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CMario *mario = CMario::GetInstance();
	if (colliable_objects->size() != 0 && mario->isAttacking == true)
	{
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		CalcPotentialCollisions(colliable_objects, coEvents);
		if (coEvents.size() != 0)
		{
			float min_tx, min_ty, nx = 0, ny;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
			CMario *mario = CMario::GetInstance();
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CLight *>(e->obj))
				{
					//				if (timeToBound >= 0.0075)
					{
						CLight *light = dynamic_cast<CLight *>(e->obj);
						if (e->nx == 0)
						{
							if (light->GetState() != LIGHT_STATE_DESTROY)
							{
								light->SetState(LIGHT_STATE_DESTROY);
								mario->point += light->point;
							}
						}
					}
				}
				if (dynamic_cast<CHeart *>(e->obj))
				{
					CHeart *heart = dynamic_cast<CHeart *>(e->obj);
					if (e->nx == 0)
					{
						if (heart->GetState() == HEART_STATE_LIVE)
						{
							heart->isHit = true;
						}
					}
				}
				if (dynamic_cast<CKnifeIcon *>(e->obj))
				{
					CKnifeIcon *knife = dynamic_cast<CKnifeIcon *>(e->obj);
					if (e->nx == 0)
					{
						if (knife->collision == true)
							knife->isHit = true;
					}
				}
				if (dynamic_cast<Dog *>(e->obj))
				{
					Dog *dog = dynamic_cast<Dog *>(e->obj);
					if (dog->GetState() != DOG_STATE_DIE)
					{
						dog->SetState(DOG_STATE_DIE);
						mario->point += dog->point;
					}
				}
				if (dynamic_cast<CBoss *>(e->obj))
				{
					CBoss *boss = dynamic_cast<CBoss *>(e->obj);
					if (e->nx == 0)
					{
						if (boss->state == BOSS_STATE_ACT)
						{
							boss->isHit = true;
							boss->curHealth -= 1;
							if (boss->curHealth - 1 <= 0) boss->SetState(BOSS_STATE_DIE);
						}

					}
				}
				if (dynamic_cast<WhipIcon *>(e->obj))
				{
					WhipIcon *wicon = dynamic_cast<WhipIcon *>(e->obj);
					if (e->nx == 0)
					{
						if (wicon->collision == true)
							wicon->isHit = true;
					}
				}
				if (dynamic_cast<CCross *>(e->obj))
				{
					CCross *cross = dynamic_cast<CCross *>(e->obj);
					if (e->nx == 0)
					{
						if (cross->GetState() == CROSS_STATE_LIVE)
						{
							cross->isHit = true;
						}
					}
				}
				if (dynamic_cast<CGoomba *>(e->obj))
				{
					CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
					if (e->nx == 0)
					{
						if (goomba->state != GOOMBA_STATE_BURN)
						{
							goomba->SetState(GOOMBA_STATE_BURN);
							mario->point += goomba->point;
						}
					}
				}
				if (dynamic_cast<CFish *>(e->obj))
				{
					CFish *fish = dynamic_cast<CFish *>(e->obj);
					if (e->nx == 0)
					{
						if (fish->state != FISH_STATE_BURN)
						{
							fish->SetState(FISH_STATE_BURN);
							mario->point += fish->point;
						}
					}
				}
				if (dynamic_cast<FishBullet *>(e->obj))
				{
					FishBullet *fish = dynamic_cast<FishBullet *>(e->obj);
					if (e->nx == 0)
					{
						fish->isHit = true;
					}
				}
				if (dynamic_cast<CBat *>(e->obj))
				{
					CBat *bat = dynamic_cast<CBat *>(e->obj);
					if (e->nx == 0)
					{
						if (bat->state != BAT_STATE_DESTROYED)
						{
							bat->SetState(BAT_STATE_DESTROYED);
							mario->point += bat->point;
						}
					}
				}
				if (dynamic_cast<CCandle *>(e->obj))
				{
					CCandle *candle = dynamic_cast<CCandle *>(e->obj);
					if (candle->GetState() != CANDLE_STATE_DESTROYED)
					{
						candle->SetState(CANDLE_STATE_DESTROYED);
						mario->point += candle->point;
					}
				}
				if (dynamic_cast<CAxeIcon *>(e->obj))
				{
					CAxeIcon *axe = dynamic_cast<CAxeIcon *>(e->obj);
					if (e->nx == 0)
					{
						if (axe->collision == true)
							axe->isHit = true;
					}
				}
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}

void Whip::Render()
{
	int alpha = 255,ani;
	if (level == WHIP_LVL_0)
	{
		if (GetState() == WHIP_STATE_RIGHT)
			ani = WHIP_ANI_ATK_RIGHT;
		else ani = WHIP_ANI_ATK_LEFT;
	}
	else if (level == WHIP_LVL_1)
	{
		if (GetState() == WHIP_STATE_RIGHT)
			ani = WHIP_ANI_ATK_RIGHT_1;
		else ani = WHIP_ANI_ATK_LEFT_1;
	}
	animations[ani]->Render(x, y, alpha);
	RenderBoundingBox();
}

void Whip::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WHIP_STATE_LEFT:
		nx = -1;
		break;
	case WHIP_STATE_RIGHT:
		nx = 1;
		break;
	default:
		break;
	}
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (level == WHIP_LVL_0)
	{
		if (animations[WHIP_ANI_ATK_LEFT]->GetCurrentFrame() == 2 && nx < 0)
		{
			left = x;
			top = y;
			right = left + WHIP_BBOX_WIDTH;
			bottom = y + WHIP_BBOX_HEIGHT;
		}
		else if (animations[WHIP_ANI_ATK_RIGHT]->GetCurrentFrame() == 2 && nx > 0)
		{
			x += 40;
			left = x;
			top = y;
			right = left + WHIP_BBOX_WIDTH;
			bottom = y + WHIP_BBOX_HEIGHT;
		}
	}
	else 
	{
		if (animations[WHIP_ANI_ATK_LEFT_1]->GetCurrentFrame() == 2 && nx < 0)
		{
			left = x;
			top = y;
			right = left + WHIP_BBOX_WIDTH_1;
			bottom = y + WHIP_BBOX_HEIGHT_1;
		}
		else if (animations[WHIP_ANI_ATK_RIGHT_1]->GetCurrentFrame() == 2 && nx > 0)
		{
			x += 37;
			left = x;
			top = y;
			right = left + WHIP_BBOX_WIDTH_1;
			bottom = y + WHIP_BBOX_HEIGHT_1;
		}
	}
}


Whip::~Whip()
{
}

WhipIcon::WhipIcon()
{
	tag = 5;
	isHit = false;
}

void WhipIcon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt, coObject);
	if (isHit == true)
		vy = WHIP_GRAVITY*dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObject, coEvents);
	if (coEvents.size() != 0)
	{
	}
	else
	{
		x += dx;
		y += dy;
	}
}

void WhipIcon::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + WICON_BBOX_WIDTH;
	b = y + WICON_BBOX_HEIGHT;
}

void WhipIcon::Render()
{
	if (isHit == true)
	{
		animations[0]->Render(x, y);
	}
}

WhipIcon::~WhipIcon()
{
}
