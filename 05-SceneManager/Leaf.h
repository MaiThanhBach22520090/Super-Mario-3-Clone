#pragma once
#include "GameObject.h"



#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_FLOAT_SPEED -0.1f
#define LEAF_GRAVITY 0.0001f
#define LEAF_FALL_SPEED 0.01f
#define LEAF_SWAY_SPEED 0.04f
#define LEAF_SWAY_DISTANCE 20.0f

#define LEAF_LIFE_TIME 7000
#define LEAF_SWAY_PERIOD 2.0f

#define ID_ANI_LEAF 10060


class CLeaf : public CGameObject
{


protected:
	float ax, ay;
	float originX;
	bool goingUp = true;
	bool movingRight = true;
	DWORD spawnTime;

	DWORD swayStartTime = 0;

public:
	CLeaf(float x, float y);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
};

