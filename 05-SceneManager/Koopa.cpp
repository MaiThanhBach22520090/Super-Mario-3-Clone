#include "Koopa.h"

CKoopa::CKoopa(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = KOOPA_GRAVITY;
    die_start = -1;
    SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - KOOPA_BBOX_WIDTH / 2;
    top = y - (state == KOOPA_STATE_WALKING ? KOOPA_BBOX_HEIGHT : KOOPA_BBOX_HEIGHT_SHELL) / 2;
    right = left + KOOPA_BBOX_WIDTH;
    bottom = top + (state == KOOPA_STATE_WALKING ? KOOPA_BBOX_HEIGHT : KOOPA_BBOX_HEIGHT_SHELL);
}

void CKoopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopa*>(e->obj)) return;

    if (e->ny != 0)
        vy = 0;
    else if (e->nx != 0)
        vx = -vx;
}

void CKoopa::OnStompedByMario(float marioX)
{
    if (state == KOOPA_STATE_WALKING)
    {
        SetState(KOOPA_STATE_SHELL);
    }
    else if (state == KOOPA_STATE_SHELL)
    {
        if (marioX < x)
            SetState(KOOPA_STATE_SHELL_MOVING_RIGHT);
        else
            SetState(KOOPA_STATE_SHELL_MOVING_LEFT);
    }
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    // If Koopa is in shell and timeout has passed, return to walking
    if ((state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT) &&
        (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
    {
        // If being carried, drop from Mario before switching back to walking
        if (beingCarried)
        {
            beingCarried = false;
            x += (nx >= 0 ? 6 : -6); // offset from Mario

        }

        y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2;
        SetState(KOOPA_STATE_WALKING);
    }

    if (beingCarried)
    {
        vx = 0;
        vy = 0;
        return;
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
    int aniId;

    if (state == KOOPA_STATE_WALKING)
        aniId = vx > 0 ? ID_ANI_KOOPA_WALKING_RIGHT : ID_ANI_KOOPA_WALKING_LEFT;
    else if (state == KOOPA_STATE_SHELL)
        aniId = ID_ANI_KOOPA_SHELL;
    else if (state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT)
        aniId = ID_ANI_KOOPA_SHELL_MOVING;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        vx = -KOOPA_WALKING_SPEED;
        break;

    case KOOPA_STATE_SHELL:
        vx = 0;
        die_start = GetTickCount64();
        break;

    case KOOPA_STATE_SHELL_MOVING_LEFT:
        vx = -KOOPA_SHELL_SPEED;
        break;

    case KOOPA_STATE_SHELL_MOVING_RIGHT:
        vx = KOOPA_SHELL_SPEED;
        break;
    }
}
