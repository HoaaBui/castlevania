#include "Whip.h"
#include "Utils.h"

void CWhip::Render()
{
	int currentFrame = -1;
	animation_set->at(0)->GetCurrentFrame(currentFrame);
	DebugOut(L"[INFO] This is your current whip frame : %d\n", currentFrame);
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