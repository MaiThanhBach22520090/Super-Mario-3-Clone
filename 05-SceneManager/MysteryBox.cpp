#include "MysteryBox.h"
#include "Mushroom.h"
#include "Coin.h"
#include "Leaf.h"
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
	if (dynamic_cast<CMario*>(e->obj))
	{
		CMario* mario = dynamic_cast<CMario*>(e->obj);
		if (e->ny < 0)
		{
			SetState(MYSTERY_BOX_STATE_HIT);
		}
	}
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

		if (spawnObjectId == 10020)
		{
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->GetLevel() == MARIO_LEVEL_BIG || mario->GetLevel() == MARIO_LEVEL_RACCOON)
			{
				CLeaf* leaf = new CLeaf(this->x, this->y - MYSTERY_BOX_BBOX_HEIGHT);
				scene->AddObject(leaf);
			}
			else
			{
				
				CMushroom* mushroom = new CMushroom(this->x, this->y - MYSTERY_BOX_BBOX_HEIGHT, spawnObjectId);
				scene->AddObject(mushroom);
			}

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