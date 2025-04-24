#include "MysteryBox.h"
#include "Mushroom.h"
#include "Animations.h"
#include "PlayScene.h"

CMysteryBox::CMysteryBox(float x, float y, int animationIdActive, int animationIdUsed) : CGameObject(x, y)
{
	this->animationIdActive = animationIdActive;
	this->animationIdUsed = animationIdUsed;
	this->animationId = animationIdActive;
	this->startY = y;
	this->hasSpawned = false;
	this->bouncing = false;

	SetState(MYSTERY_BOX_STATE_IDLE);
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
		scene->AddObject(new CMushroom(x, y - MYSTERY_BOX_BBOX_HEIGHT, 10020));

		hasSpawned = true;
	}
}
