#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define KOOPA_WALK_SPEED 0.05f
#define KOOPA_SHELL_SPEED 0.2f

#define KOOPA_GRAVITY 0.002f

#define KOOPA_WIDTH 16
#define KOOPA_HEIGHT 26
#define KOOPA_SHELL_HEIGHT 16

#define KOOPA_STATE_WALKING 0
#define KOOPA_STATE_SHELL 1
#define KOOPA_STATE_SLIDING 2

#define ID_ANI_KOOPA_WALKING 20001
#define ID_ANI_KOOPA_SHELL 20002
#define ID_ANI_KOOPA_SLIDING 20003

class CKoopa : public CGameObject
{
protected:
    float ax, ay;
    int state;
    DWORD shellStartTime;
    bool isOnGround;

public:
    CKoopa(float x, float y);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

    void SetState(int state);
};
