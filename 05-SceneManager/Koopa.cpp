#include "Koopa.h"
#include "Game.h"

CKoopa::CKoopa(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = KOOPA_GRAVITY;
    this->vx = -KOOPA_WALK_SPEED;
    this->state = KOOPA_STATE_WALKING;
    this->shellStartTime = 0;
    this->isOnGround = false;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    x += vx * dt;
    y += vy * dt;

    // TODO: Add proper collision detection here (OnCollisionWith)

    // Auto exit shell after some time
    if (state == KOOPA_STATE_SHELL && GetTickCount64() - shellStartTime >= 4000)
    {
        SetState(KOOPA_STATE_WALKING);
    }

    CGameObject::Update(dt, coObjects);
}

void CKoopa::Render()
{
    int ani = -1;

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        ani = ID_ANI_KOOPA_WALKING;
        break;
    case KOOPA_STATE_SHELL:
        ani = ID_ANI_KOOPA_SHELL;
        break;
    case KOOPA_STATE_SLIDING:
        ani = ID_ANI_KOOPA_SLIDING;
        break;
    }

    CAnimations::GetInstance()->Get(ani)->Render(x, y);
    RenderBoundingBox();
}

void CKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - KOOPA_WIDTH / 2;
    r = l + KOOPA_WIDTH;

    if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SLIDING)
    {
        t = y - KOOPA_SHELL_HEIGHT;
        b = y;
    }
    else
    {
        t = y - KOOPA_HEIGHT;
        b = y;
    }
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0) vx = -vx;
    if (e->ny != 0) vy = 0;
}

void CKoopa::SetState(int newState)
{
    state = newState;

    switch (newState)
    {
    case KOOPA_STATE_WALKING:
        vy = 0;
        vx = -KOOPA_WALK_SPEED;
        break;
    case KOOPA_STATE_SHELL:
        vx = 0;
        shellStartTime = GetTickCount64();
        break;
    case KOOPA_STATE_SLIDING:
        vx = (vx >= 0 ? KOOPA_SHELL_SPEED : -KOOPA_SHELL_SPEED);
        break;
    }
}
