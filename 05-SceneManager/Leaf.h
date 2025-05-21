#pragma once
#include "GameObject.h"



#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_FLOAT_SPEED -0.05f
#define LEAF_GRAVITY 0.0002f
#define LEAF_FALL_SPEED 0.05f
#define LEAF_SWAY_SPEED 0.02f
#define LEAF_SWAY_DISTANCE 32f

#define LEAF_LIFE_TIME 7000

#define ID_ANI_LEAF 10060


class CLeaf : public CGameObject
{
public:
	CLeaf(float x, float y) : CGameObject(x, y)
	{
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e) {};

};

