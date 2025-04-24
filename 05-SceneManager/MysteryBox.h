// MysteryBox.h
#pragma once

#include "GameObject.h"

#define MYSTERY_BOX_BBOX_WIDTH 16
#define MYSTERY_BOX_BBOX_HEIGHT 16

#define MYSTERY_BOX_STATE_IDLE 0
#define MYSTERY_BOX_STATE_HIT 1

#define ID_ANI_MYSTERY_BOX_ACTIVE 20000
#define ID_ANI_MYSTERY_BOX_USED 20001

#define MYSTERY_BOX_BOUNCE_SPEED -0.05f
#define MYSTERY_BOX_BOUNCE_HEIGHT 4.0f

class CMysteryBox : public CGameObject
{
protected:
	float startY = 0;
	bool hasSpawned = false;
	bool bouncing = false;
public:
	CMysteryBox(float x, float y);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void SetState(int state);
};
