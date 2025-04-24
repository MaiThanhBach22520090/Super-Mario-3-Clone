#pragma once
#include "GameObject.h"


class cSolidBlock : public CGameObject
{
protected:
	float width;
	float height;
	int animationId;

public:
	cSolidBlock(float x, float y, int animationId, float width, float height) : CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		this->animationId = animationId;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

