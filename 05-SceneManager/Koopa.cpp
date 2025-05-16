#include "Koopa.h"

CKoopa::CKoopa(float x, float y, bool hasWings) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = KOOPA_GRAVITY;
    die_start = -1;

    this->isParatroopa = hasWings;
    this->hasWings = hasWings;
    this->isOnGround = false;
    this->lastJumpTime = GetTickCount64();

    SetState(hasWings ? KOOPA_STATE_PARATROOPA : KOOPA_STATE_WALKING);
}
void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - KOOPA_BBOX_WIDTH / 2;
    top = y - (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_PARATROOPA ? KOOPA_BBOX_HEIGHT : KOOPA_BBOX_HEIGHT_SHELL) / 2;
    right = left + KOOPA_BBOX_WIDTH;
    bottom = top + (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_PARATROOPA ? KOOPA_BBOX_HEIGHT : KOOPA_BBOX_HEIGHT_SHELL);
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
    {
        vy = 0;
        if (e->ny < 0 && isParatroopa && hasWings)
            isOnGround = true;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CKoopa::OnStompedByMario(float marioX)
{
    if (state == KOOPA_STATE_PARATROOPA)
    {
        hasWings = false;
        isParatroopa = false;
        SetState(KOOPA_STATE_WALKING);
    }
    else if (state == KOOPA_STATE_WALKING)
    {
        SetState(KOOPA_STATE_SHELL);
    }
    else if (state == KOOPA_STATE_SHELL)
    {
        SetState(marioX < x ? KOOPA_STATE_SHELL_MOVING_RIGHT : KOOPA_STATE_SHELL_MOVING_LEFT);
    }
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (hasWings)
    {
        ay = PARATROOPA_GRAVITY;
        UpdateJump(dt);
    }
	else
	{
		ay = KOOPA_GRAVITY;
	}

    vy += ay * dt;
    vx += ax * dt;

    if ((state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT) &&
        (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
    {
        if (beingCarried)
        {
            beingCarried = false;
            x += (nx >= 0 ? 6 : -6);
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

void CKoopa::UpdateJump(DWORD dt)
{
    ULONGLONG now = GetTickCount64();
    if (isOnGround && now - lastJumpTime >= PARATROOPA_JUMP_INTERVAL)
    {
        vy = -PARATROOPA_JUMP_SPEED;
        isOnGround = false;
        lastJumpTime = now;
    }
}


void CKoopa::Render()
{
    if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_PARATROOPA)
        aniId = vx > 0 ? ID_ANI_KOOPA_WALKING_RIGHT : ID_ANI_KOOPA_WALKING_LEFT;
    else if (state == KOOPA_STATE_SHELL)
        aniId = ID_ANI_KOOPA_SHELL;
    else if (state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT)
        aniId = ID_ANI_KOOPA_SHELL_MOVING;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);

    if (hasWings)
    {
		int wingOffsetY = 8;

		if (vx < 0)
			CAnimations::GetInstance()->Get(ID_ANI_WING_RIGHT)->Render(x + KOOPA_BBOX_WIDTH / 2, y - wingOffsetY);
		else
			CAnimations::GetInstance()->Get(ID_ANI_WING_LEFT)->Render(x - KOOPA_BBOX_WIDTH / 2, y - wingOffsetY);
    }

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

    case KOOPA_STATE_PARATROOPA:
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