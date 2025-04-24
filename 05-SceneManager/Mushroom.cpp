#include "Mushroom.h"

CMushroom::CMushroom(float x, float y, int animationID) : CGameObject(x, y)
{
	this->animationId = animationID;
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	this->vx = MUSHROOM_SPEED; // Start moving to the right
	this->vx = (rand() % 2 == 0 ? 1 : -1) * MUSHROOM_SPEED;
}


void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(animationId)->Render(x, y);
}

void CMushroom::SetState(int state)
{
}

void CMushroom::SetAnimationId(int aniId)
{
	this->animationId = aniId;
}