#include "Koopa.h"
#include "Platform.h"
#include "MysteryBox.h"
#include "Ground.h"
#include "BackPlatform.h"
#include "Goomba.h"
#include "PiranhaPlant.h"
#include "Mario.h"
#include "PlayScene.h"



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



#pragma region Collisions

void CKoopa::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    OnCollisionWithMysteryBox(e);

    if (dynamic_cast<CGoomba*>(e->obj))
    {
        OnCollisionWithGoomba(e);
    }
    else if (dynamic_cast<CKoopa*>(e->obj))
    {
        OnCollisionWithKoopa(e);
    }
    else if (dynamic_cast<CPiranhaPlant*>(e->obj))
    {
        OnCollisionWithPiranhaPlant(e);
    }

    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CKoopa*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
        if (e->ny < 0 && isParatroopa && hasWings)
            isOnGround = true;

		if (vy == 0)
			isOnGround = true;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CKoopa::OnCollisionWithMysteryBox(LPCOLLISIONEVENT e)
{
    CMysteryBox* box = dynamic_cast<CMysteryBox*>(e->obj);
    if (box != nullptr &&
        (state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT) &&
        e->nx != 0) // Side hit only
    {
        box->SetState(MYSTERY_BOX_STATE_HIT);
    }
}

void CKoopa::OnStompedByMario(float marioX)
{
    if (state == KOOPA_STATE_PARATROOPA)
    {

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

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba != nullptr && (state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT))
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE_ON_HIT)
		{
			goomba->SetState(GOOMBA_STATE_DIE_ON_HIT);
            CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
            mario->AddPoint(100); // Add points for hitting Goomba with shell

		}
	}
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
    CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
    if (koopa != nullptr && (state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT))
    {
        CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

        if (koopa->GetState() == KOOPA_STATE_PARATROOPA)
        {
            mario->AddPoint(100);
            koopa->SetState(KOOPA_STATE_WALKING);
        }
        else 
        {    
            koopa->SetState(KOOPA_STATE_DIE_ON_HIT);
			mario->AddPoint(100);
        }
	}
}

void CKoopa::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* piranhaPlant = dynamic_cast<CPiranhaPlant*>(e->obj);
	if (piranhaPlant != nullptr)
	{
        CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
        mario->AddPoint(100); // Add points for hitting Goomba with shell
		piranhaPlant->Delete();
	}
}

#pragma endregion

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

    if ((state == GOOMBA_STATE_DIE_ON_HIT) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
    {
        isDeleted = true;
        return;
    }

    if ((state == KOOPA_STATE_SHELL || state == KOOPA_STATE_SHELL_MOVING_LEFT || state == KOOPA_STATE_SHELL_MOVING_RIGHT) &&
        (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
    {
        if (beingCarried || (!beingCarried && vx == 0)) // Only recover if held
        {
            y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SHELL) / 2;
            SetState(KOOPA_STATE_WALKING);
        }
    }


    if (beingCarried)
    {
        vx = 0;
        vy = 0;
        return;
    }

    if (!hasWings && state == KOOPA_STATE_WALKING && isOnGround)
        CheckForCliff(coObjects);

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
	else if (state == KOOPA_STATE_DIE_ON_HIT)
		aniId = ID_ANI_KOOPA_SHELL;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);

    if (hasWings)
    {
		int wingOffsetY = 8;

		if (vx < 0)
			CAnimations::GetInstance()->Get(ID_ANI_WING_RIGHT)->Render(x + KOOPA_BBOX_WIDTH / 2, y - wingOffsetY);
		else
			CAnimations::GetInstance()->Get(ID_ANI_WING_LEFT)->Render(x - KOOPA_BBOX_WIDTH / 2, y - wingOffsetY);
    }

    //RenderBoundingBox();
}
void CKoopa::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case KOOPA_STATE_WALKING:
        hasWings = false;
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
	case KOOPA_STATE_DIE_ON_HIT:
		vx = 0;
		vy = 0.5f;
    }
}

void CKoopa::CheckForCliff(const vector<LPGAMEOBJECT>* coObjects)
{
    float probeX = (vx > 0)
        ? x + KOOPA_BBOX_WIDTH / 2 + PROBE_DISTANCE
        : x - KOOPA_BBOX_WIDTH / 2 - PROBE_DISTANCE;
       float probeY = y + KOOPA_BBOX_HEIGHT + PROBE_DEPTH;

    bool hasGround = false;

    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (dynamic_cast<CGround*>(obj) || dynamic_cast<BackPlatform*>(obj))
        {
            float l, t, r, b;
            obj->GetBoundingBox(l, t, r, b);

            if (probeX >= l && probeX <= r && probeY >= t && probeY <= b)
            {
                hasGround = true;
                break;
            }
        }
    }


    if (!hasGround)
    {
        vx = -vx;
        nx = -nx;
    }
}