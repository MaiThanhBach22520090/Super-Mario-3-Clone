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
#define KOOPA_STATE_PARATROOPA 400

#define PARATROOPA_JUMP_SPEED        0.25f
#define PARATROOPA_GRAVITY           0.0005f
#define PARATROOPA_JUMP_INTERVAL     2000

#define ID_ANI_WING_RIGHT     10050
#define ID_ANI_WING_LEFT     10051

#define KOOPA_DIE_TIMEOUT 5000

#define ID_ANI_KOOPA_WALKING_LEFT   10040
#define ID_ANI_KOOPA_WALKING_RIGHT  10041
#define ID_ANI_KOOPA_SHELL 10042
#define ID_ANI_KOOPA_SHELL_MOVING 10043

class CKoopa : public CGameObject
{
protected:
    float ax, ay;
    int aniId;

    bool beingCarried = false;

    // Paratroopa jump
    bool isParatroopa = false;
    bool hasWings = false;
    bool isOnGround = false;
    ULONGLONG lastJumpTime = 0;

    ULONGLONG die_start;

    void UpdateJump(DWORD dt);

    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }

public:
    CKoopa(float x, float y, bool isParatroopa = false);

    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void OnStompedByMario(float marioX);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
    void Render() override;
    void SetState(int state) override;
    void SetBeingCarried(bool carried) { beingCarried = carried; }
    void SetWings(bool wings) { hasWings = wings; isParatroopa = wings; }
};