#pragma once
#include "GameObject.h"


class BackPlatform : public CGameObject
{
protected:
	int length;	
	int height;
	
	// Unit: cell 
	float cellWidth;
	float cellHeight;
	int topSpriteIdBegin, topSpriteIdMiddle, topSpriteIdEnd;
	int middleSpriteIdBegin, middleSpriteIdMiddle, middleSpriteIdEnd;
	int bottomSpriteIdBegin, bottomSpriteIdMiddle, bottomSpriteIdEnd;
public:
	BackPlatform(float x, float y,
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
		this->bottomSpriteIdBegin = sprite_base_id + 7;
		this->bottomSpriteIdMiddle = sprite_base_id + 8;
		this->bottomSpriteIdEnd = sprite_base_id + 9;
	
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);
};

