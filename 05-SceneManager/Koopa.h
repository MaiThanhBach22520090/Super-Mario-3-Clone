#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_SHELL_SPEED 0.2f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 24
#define KOOPA_BBOX_HEIGHT_SHELL 16

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SHELL 200
#define KOOPA_STATE_SHELL_MOVING 300
#define KOOPA_STATE_SHELL_MOVING_LEFT  301
#define KOOPA_STATE_SHELL_MOVING_RIGHT 302

#define KOOPA_DIE_TIMEOUT 5000

#define ID_ANI_KOOPA_WALKING_LEFT   10040
#define ID_ANI_KOOPA_WALKING_RIGHT  10041
#define ID_ANI_KOOPA_SHELL 10042
#define ID_ANI_KOOPA_SHELL_MOVING 10043

class CKoopa : public CGameObject
{
protected:
    float ax;
    float ay;
    ULONGLONG die_start;
    bool isFacingRight = false;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CKoopa(float x, float y);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void SetState(int state);


    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; };

    void OnStompedByMario(float marioX);

	int GetX() { return x; }
};
