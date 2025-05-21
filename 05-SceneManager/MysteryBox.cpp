#include "MysteryBox.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Animations.h"
#include "PlayScene.h"

void CMysteryBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MYSTERY_BOX_BBOX_WIDTH / 2;
	top = y - MYSTERY_BOX_BBOX_HEIGHT / 2;
	right = left + MYSTERY_BOX_BBOX_WIDTH;
	bottom = top + MYSTERY_BOX_BBOX_HEIGHT;
}

void CMysteryBox::OnNoCollision(DWORD dt)
{

}

void CMysteryBox::OnCollisionWith(LPCOLLISIONEVENT e)
{

}

void CMysteryBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void CMysteryBox::Render()
{
	animationId = hasSpawned ? animationIdUsed : animationIdActive;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(animationId)->Render(x, y);
}

void CMysteryBox::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == MYSTERY_BOX_STATE_HIT && !hasSpawned)
	{
		bouncing = true;
		vy = MYSTERY_BOX_BOUNCE_SPEED;

		// Spawn mushroom slightly above the box
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

		if (spawnObjectId == 10020) // Mushroom
		{
			// Spawn a mushroom
			float x = this->x;
			float y = this->y - MYSTERY_BOX_BBOX_HEIGHT;
			// Create a new mushroom object
			CMushroom* mushroom = new CMushroom(x, y, spawnObjectId);
			scene->AddObject(mushroom);
		}
		else if (spawnObjectId == 11000) // Coin
		{
			// Spawn a coin
			float x = this->x;
			float y = this->y - MYSTERY_BOX_BBOX_HEIGHT - 48;
			// Create a new coin object
			CCoin* coin = new CCoin(x, y, true);
			scene->AddObject(coin);
		}

		hasSpawned = true;
	}
}