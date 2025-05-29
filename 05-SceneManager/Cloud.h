#pragma once
#include "GameObject.h"

class CCloud : public CGameObject
{
protected:
	int length;
	int cellWidth;
	int cellHeight;

	int topSpriteIdBegin, topSpriteIdMiddle, topSpriteIdEnd;
	int bottomSpriteIdBegin, bottomSpriteIdMiddle, bottomSpriteIdEnd;

public:
	CCloud(float x, float y,int length,	int sprite_base_id)
		: CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = 16;
		this->cellHeight = 16;
		this->topSpriteIdBegin = sprite_base_id + 1;
		this->topSpriteIdMiddle = sprite_base_id + 2;
		this->topSpriteIdEnd = sprite_base_id + 3;
		this->bottomSpriteIdBegin = sprite_base_id + 4;
		this->bottomSpriteIdMiddle = sprite_base_id + 5;
		this->bottomSpriteIdEnd = sprite_base_id + 6;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 0; }
};

