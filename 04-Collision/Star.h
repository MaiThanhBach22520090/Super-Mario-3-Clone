#pragma once
#include "GameObject.h"

#include "Animation.h"

#define ID_ANI_STAR 11001

#define STAR_WIDTH 16
#define STAR_BBOX_WIDTH 16
#define STAR_BBOX_HEIGHT 16
#define STAR_BOUNCE_SPEED 0.1f
#define STAR_GRAVITY 0.001f

class CStar : public CGameObject
{
	float ax;
	float ay;


public:
	CStar(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void OnCollisionWith(LPCOLLISIONEVENT e);
};

