#pragma once
#include "GameObject.h"

#define MYSTERY_BOX_BBOX_WIDTH 16
#define MYSTERY_BOX_BBOX_HEIGHT 16

#define MYSTERY_BOX_STATE_IDLE 0
#define MYSTERY_BOX_STATE_HIT 1

#define MYSTERY_BOX_BOUNCE_HEIGHT 8.0f
#define MYSTERY_BOX_BOUNCE_SPEED -0.15f
#define MYSTERY_BOX_GRAVITY 0.0005f

class CMysteryBox : public CGameObject
{
protected:
	float startY = 0;
	bool hasSpawned = false;
	bool bouncing = false;
	int animationIdActive = 0;
	int animationIdUsed = 0;
	int animationId = 0;
	int spawnObjectId = 0;

public:

	CMysteryBox(float x, float y, int animationIdActive, int animationIdUsed, int spawnObjectId) : CGameObject(x, y)
	{
		this->animationIdActive = animationIdActive;
		this->animationIdUsed = animationIdUsed;
		this->animationId = animationIdActive;
		this->spawnObjectId = spawnObjectId;
		this->startY = y;
		this->hasSpawned = false;
		this->bouncing = false;
		SetState(MYSTERY_BOX_STATE_IDLE);
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int IsBlocking() { return 1; }
	void SetState(int state);
};
