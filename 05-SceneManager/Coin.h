#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_FLOAT_UP_SPEED -0.05f
#define COIN_FALL_DOWN_SPEED 0.05f
#define COIN_FLOAT_HEIGHT 16
#define COIN_LIFETIME_AFTER_FALL 200


class CCoin : public CGameObject {

public:

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
			spawnTime = 0;
		}
	}

	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	
};