#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "Koopa.h"
#include "Tail.h"

#include "debug.h"

#pragma region Stats

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.00008f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_AIRJUMP_RUN_SPEED_Y	0.25f

#define MARIO_FLY_FLAP_SPEED 0.18f
#define MARIO_FLAP_COOLDOWN 120
#define MARIO_AIR_DRAG 0.0004f

#define MARIO_GRAVITY			0.002f

#define MARIO_FLYING_GRAVITY           0.0001f   
#define MARIO_GLIDE_GRAVITY            0.0006f   
#define MARIO_FLY_ASCEND_BOOST         0.0002f   
#define MARIO_MAX_FLYING_ASCENT_SPEED  0.1f      
#define MARIO_FLYING_DECELERATION      0.0002f
#define MARIO_MAX_FLYING_TIME          2500 


#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#pragma endregion


#pragma region STATE

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLYING 700
#define MARIO_STATE_GLIDING 701
#define MARIO_STATE_FLAPING	702

#define MARIO_STATE_TAIL_ATTACK 800

#pragma endregion


#pragma region ANIMATION_ID

// BIG MARIO
#pragma region Big

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999


#pragma endregion

// SMALL MARIO
#pragma region Small
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#pragma endregion

// RACCOON MARIO
#pragma region Raccoon

#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 2101

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 2201

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 2301

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2400
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2401

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2501

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2601

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2700
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2701

#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT	2800
#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT	2801

#define ID_ANI_MARIO_RACCOON_FLYING_RIGHT	2810
#define ID_ANI_MARIO_RACCOON_FLYING_LEFT	2811

#pragma endregion

#pragma endregion


#pragma region OTHER STUFF

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON	3

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_BIG_SITTING_BBOX_WIDTH  15
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_RACOON_TAIL_WIDTH_DIFF 8

#define MARIO_UNTOUCHABLE_TIME 2500

#define MARIO_FLAPING_TIME_PER_FRAME 75

#define MARIO_TAIL_ATTACK_TIME_PER_FRAME 100

#pragma endregion


class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	bool isCarrying = false;
	CKoopa* carriedKoopa = nullptr;

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 
	int points;

	// Raccoon
	bool isFlying = false;
	bool canFly = false;
	float currentFlyingTime = 0;
	ULONGLONG lastFlapTime;
	bool isGliding = false;
	float currentGlidingTime = 0;
	bool isAttacking = false;
	DWORD currentTailAttackTime = 0;
	CTail* tail = nullptr;

	// Teleport
	bool isTeleporting = false;
	int teleportPhase = 0; // 0: not teleporting, 1: sinking, 2: teleporting, 3: done
	ULONGLONG teleportStartTime = 0;
	float teleportDestinationX, teleportDestinationY;
	float teleportSpeedY = 0;
	float teleportStartY = 0;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithMysteryBox(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithEnemy();

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_RACCOON;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;

		tail = new CTail(x, y);
	}

	~CMario()
	{
		if (tail != nullptr)
			delete tail;
		if (carriedKoopa != nullptr)
			carriedKoopa = nullptr;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		if (isTeleporting) return 0;
		return (state != MARIO_STATE_DIE) ; 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0) && isTeleporting ? 0 : 1; ; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	float GetX() { return x; }
	float GetY() { return y; }

	void SetTeleporting(bool teleporting) { isTeleporting = teleporting; }
	bool IsTeleporting() { return isTeleporting; }
	bool IsSitting() { return isSitting; }

	float GetRenderX();
	int GetLevel() { return level; }

	void ReleaseCarriedKoopa();
	void AddPoint(int point) { this->points += point; }
	int GetPointCount() { return points; }

	void HandleFlying(DWORD dt);
	void HandleGliding(DWORD dt);

	void ResetTailAttackAnimation();
	void HandleTailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void StartTeleport(float destX, float destY, bool goingDown);
	void HandleTeleporting(DWORD dt);
};