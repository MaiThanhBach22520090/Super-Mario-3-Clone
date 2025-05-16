#include "Tail.h"
#include "Goomba.h"
#include "Koopa.h"
#include "PlayScene.h"


CTail::CTail(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isActive = false;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render()
{
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TAIL_BBOX_WIDTH;
	b = y + TAIL_BBOX_HEIGHT;
}

void CTail::OnNoCollision(DWORD dt)
{
}

void CTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (isActive == false) return;

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba != NULL)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa != NULL)
	{
		koopa->SetState(KOOPA_STATE_SHELL);
		koopa->SetWings(false);
	}
}