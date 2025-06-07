#include "Tail.h"
#include "Goomba.h"
#include "Koopa.h"
#include "GoldenBrick.h"
#include "Collision.h"

CTail::CTail(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isActive = false;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActive) return;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTail::Render()
{
	RenderBoundingBox();
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isActive)
	{
		l = t = r = b = 0;
		return;
	}

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
	if (!isActive) return;

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CGoldenBrick*>(e->obj))
		OnCollisionWithGoldenBrick(e);
}

void CTail::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba)
		goomba->SetState(GOOMBA_STATE_DIE);
}

void CTail::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa)
	{
		koopa->SetState(KOOPA_STATE_SHELL);
		koopa->SetWings(false);
	}
}

void CTail::OnCollisionWithGoldenBrick(LPCOLLISIONEVENT e)
{
	CGoldenBrick* brick = dynamic_cast<CGoldenBrick*>(e->obj);
	if (brick)
		brick->OnCollisionWithTail(e);
}
