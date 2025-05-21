#include "Coin.h"
#include "PlayScene.h"
#include "MysteryBox.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isMysteryCoin)
	{
		HandleFloating(dt);
	}

	CGameObject::Update(dt);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCoin::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CCoin::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
	if (e->ny != 0)
	{
		vy = 0;


	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

	if (dynamic_cast<CMysteryBox*>(e->obj))
	{
		OnCollisionWithMysteryBox(e);
	}
}

void CCoin::OnCollisionWithMysteryBox(LPCOLLISIONEVENT e)
{

	CMysteryBox* box = dynamic_cast<CMysteryBox*>(e->obj);

	if (box != nullptr && e->ny < 0)
	{
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->AddPoint(100);
		Delete();
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::HandleFloating(DWORD dt)
{
    vy += ay * dt;

    if (goingUp)
    {
        if (y <= startY - COIN_FLOAT_HEIGHT)
        {
            y = startY - COIN_FLOAT_HEIGHT;
            vy = COIN_FALL_DOWN_SPEED;
        }
    }
}
