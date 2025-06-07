#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 1000

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_JUMPING 101

#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_ON_HIT 201

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

#define ID_ANI_WING_RIGHT     10050
#define ID_ANI_WING_LEFT     10051

#define PARAGOOMBA_JUMP_SPEED 0.2f
#define PARAGOOMBA_GRAVITY 0.0005f
#define PARAGOOMBA_JUMP_INTERVAL 2000

class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	bool hasWings = false;
	bool isOnGround = false;
	ULONGLONG lastJumpTime = 0;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, bool hasWings);
	virtual void SetState(int state);

	void HandleJump(DWORD dt);
	bool HasWings() { return hasWings; }
	void SetWings(bool hasWings) { this->hasWings = hasWings; }
	void SetGravity(float gravity) { this->ay = gravity; }
};