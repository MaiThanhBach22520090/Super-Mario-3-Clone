// MysteryBox.cpp
#include "MysteryBox.h"
#include "Mushroom.h"
#include "Animations.h"
#include "PlayScene.h"

CMysteryBox::CMysteryBox(float x, float y) : CGameObject(x, y)
{
	this->startY = y;
	this->state = MYSTERY_BOX_STATE_IDLE;
}

void CMysteryBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MYSTERY_BOX_BBOX_WIDTH / 2;
	top = y - MYSTERY_BOX_BBOX_HEIGHT / 2;
	right = left + MYSTERY_BOX_BBOX_WIDTH;
	bottom = top + MYSTERY_BOX_BBOX_HEIGHT;
}

void CMysteryBox::OnNoCollision(DWORD dt)
{
	if (bouncing)
	{
		y += vy * dt;
		if (y <= startY - MYSTERY_BOX_BOUNCE_HEIGHT)
		{
			y = startY - MYSTERY_BOX_BOUNCE_HEIGHT;
			vy = -vy;
		}
		else if (y >= startY)
		{
			y = startY;
			vy = 0;
			bouncing = false;
		}
	}
}

void CMysteryBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	// nothing
}

void CMysteryBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (bouncing)
		vy += 0.0005f * dt;

	CGameObject::Update(dt, coObjects);
}

void CMysteryBox::Render()
{
	int ani = (state == MYSTERY_BOX_STATE_HIT) ? ID_ANI_MYSTERY_BOX_USED : ID_ANI_MYSTERY_BOX_ACTIVE;
	CAnimations::GetInstance()->Get(ani)->Render(x, y);

	// RenderBoundingBox();
}

void CMysteryBox::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MYSTERY_BOX_STATE_HIT:
		if (!hasSpawned)
		{
			bouncing = true;
			vy = MYSTERY_BOX_BOUNCE_SPEED;

			// Spawn mushroom
			LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
			scene->AddObject(new CMushroom(x, y - 16, 10020)); // adjust animation ID
			hasSpawned = true;
		}
		break;
	}
}
