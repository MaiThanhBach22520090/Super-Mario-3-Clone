#pragma once
#include "GameObject.h"

class CGround : public CGameObject
{
protected:
	int length;
	int height;

	float cellWidth;
	float cellHeight;
	int topSpriteIdBegin, topSpriteIdMiddle, topSpriteIdEnd;
	int middleSpriteIdBegin, middleSpriteIdMiddle, middleSpriteIdEnd;

public:
	CGround(float x, float y,
		float cell_width, float cell_height, int length, int height,
		int sprite_base_id)
		: CGameObject(x, y)
	{
		this->length = length;
		this->height = height;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->topSpriteIdBegin = sprite_base_id + 1;
		this->topSpriteIdMiddle = sprite_base_id + 2;
		this->topSpriteIdEnd = sprite_base_id + 3;
		this->middleSpriteIdBegin = sprite_base_id + 4;
		this->middleSpriteIdMiddle = sprite_base_id + 5;
		this->middleSpriteIdEnd = sprite_base_id + 6;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsBlocking() { return 1; }
	int IsCollidable() { return 1; }
};

