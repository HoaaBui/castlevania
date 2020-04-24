#include "Whip.h"

void CWhip::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CWhip::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + WHIP_BBOX_WIDTH;
	b = y + WHIP_BBOX_HEIGHT;
}