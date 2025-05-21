#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_FLOAT_UP_SPEED -0.2f            // faster rise
#define COIN_FALL_DOWN_SPEED 0.2f            // faster fall
#define COIN_LIFETIME_AFTER_FALL 300         // in milliseconds (e.g., 300 ms)

#define COIN_FLOAT_HEIGHT 16


#define COIN_GRAVITY 0.001f


class CCoin : public CGameObject {

public:

	float ay;

	DWORD spawnTime;
	int animationId = ID_ANI_COIN;
	bool isMysteryCoin = false;
	bool goingUp = true;
	float startY;

public:
	CCoin(float x, float y, bool isMysteryCoin) : CGameObject(x, y)
	{
		this->isMysteryCoin = isMysteryCoin;
		if (isMysteryCoin)
		{
			startY = y;
			this->vy = COIN_FLOAT_UP_SPEED;
			this->ay = COIN_GRAVITY; 
			spawnTime = 0;
		}
		else
		{
			startY = y;
			this->ay = 0;
			this->vy = 0;
			spawnTime = 0;
		}
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithMysteryBox(LPCOLLISIONEVENT e);

	void HandleFloating(DWORD dt);
	
};