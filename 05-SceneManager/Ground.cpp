#include "Ground.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CGround::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;
	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	float xx = x - this->cellWidth / 2 + rect.right / 2;
	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CGround::Render()
{
	if (this->length <= 0) return;
	CSprites* s = CSprites::GetInstance();
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	// Top row
	float xx = x;
	s->Get(this->topSpriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->topSpriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->topSpriteIdEnd)->Draw(xx, y);
	// Middle rows (i = 1 to height-1)
	for (int row = 1; row < this->height; row++)
	{
		float rowY = y + this->cellHeight * row;
		float xx = x;
		s->Get(this->middleSpriteIdBegin)->Draw(xx, rowY);
		xx += this->cellWidth;
		for (int col = 1; col < this->length - 1; col++)
		{
			s->Get(this->middleSpriteIdMiddle)->Draw(xx, rowY);
			xx += this->cellWidth;
		}

		if (length > 1)
			s->Get(this->middleSpriteIdEnd)->Draw(xx, rowY);
	}

	RenderBoundingBox();
}

void CGround::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->cellWidth / 2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight * this->height;
}
