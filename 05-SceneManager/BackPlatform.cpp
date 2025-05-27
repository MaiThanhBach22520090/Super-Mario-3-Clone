#include "BackPlatform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void BackPlatform::RenderBoundingBox()
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

void BackPlatform::Render()
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
    for (int row = 1; row < this->height-1; row++)
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

    // Bottom row (always 1 row below the last middle row)
    float bottomY = y + this->cellHeight * (this->height-1);
    xx = x;
    s->Get(this->bottomSpriteIdBegin)->Draw(xx, bottomY);
    xx += this->cellWidth;

    for (int i = 1; i < this->length - 1; i++)
    {
        s->Get(this->bottomSpriteIdMiddle)->Draw(xx, bottomY);
        xx += this->cellWidth;
    }

    if (length > 1)
        s->Get(this->bottomSpriteIdEnd)->Draw(xx, bottomY);
}

void BackPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight * 3; // Top, middle, and bottom parts
}

int BackPlatform::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1; // Collidable from above
	else return 0; // Not collidable from other directions
}
