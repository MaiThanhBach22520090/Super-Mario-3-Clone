#include "Cloud.h"
#include "Game.h"

void CCloud::Render()
{
	if (this->length <= 2) return;

	CSprites* s = CSprites::GetInstance();
	float cx, cy;

	CGame::GetInstance()->GetCamPos(cx, cy);

	// Top row
	float xx = x;
	s->Get(this->topSpriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	if (length > 2)
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->topSpriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
	s->Get(this->topSpriteIdEnd)->Draw(xx, y);

	// Bottom row
	xx = x;
	s->Get(this->bottomSpriteIdBegin)->Draw(xx, y + this->cellHeight);
	xx += this->cellWidth;
	if (length > 2)
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->bottomSpriteIdMiddle)->Draw(xx, y + this->cellHeight);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->bottomSpriteIdEnd)->Draw(xx, y + this->cellHeight);
}

void CCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->cellWidth / 2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}
