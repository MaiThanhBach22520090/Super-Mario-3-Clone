#include "Star.h"

CStar::CStar(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = STAR_GRAVITY;
}

void CStar::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_STAR)->Render(x, y);
	RenderBoundingBox();
}

void CStar::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - STAR_BBOX_WIDTH / 2;
	t = y - STAR_BBOX_HEIGHT / 2;
	r = l + STAR_BBOX_WIDTH;
	b = t + STAR_BBOX_HEIGHT;
}

void CStar::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CStar*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}