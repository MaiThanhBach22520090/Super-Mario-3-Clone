#include "SolidBlock.h"

void cSolidBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(animationId)->Render(x, y);
}

void cSolidBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}