#include "Coin.h"
#include "PlayScene.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt)
{
	if (!isMysteryCoin) return;

	y += vy * dt;

	if (goingUp)
	{
		if (y <= startY - COIN_FLOAT_HEIGHT)
		{
			y = startY - COIN_FLOAT_HEIGHT;
			vy = COIN_FALL_DOWN_SPEED;
			goingUp = false;
			spawnTime = GetTickCount64(); // start countdown to delete
		}
	}
	else // falling
	{
		DWORD now = GetTickCount64();
		if (now - spawnTime >= COIN_LIFETIME_AFTER_FALL)
		{
			Delete();
		}
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}