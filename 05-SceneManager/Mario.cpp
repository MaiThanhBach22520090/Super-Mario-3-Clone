#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "Mushroom.h"
#include "MysteryBox.h"
#include "PiranhaPlant.h"
#include "Koopa.h"
#include "Leaf.h"


#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	#pragma region Handle Koopa

	if (isCarrying && carriedKoopa != nullptr)
	{
		float offsetX = nx > 0 ? 12.0f : -12.0f; // adjust for your sprite
		float offsetY = -6.0f; // height adjustment
		carriedKoopa->SetPosition(x + offsetX, y + offsetY);
	}

	if (isCarrying && carriedKoopa)
	{
		// Drop it if it's no longer in shell state
		int koopaState = carriedKoopa->GetState();
		if (koopaState == KOOPA_STATE_WALKING)
		{
			isCarrying = false;
			carriedKoopa->SetBeingCarried(false);
			carriedKoopa = nullptr;
		}
		else
		{
			// Carry position sync
			carriedKoopa->SetPosition(x + nx * 10, y);
		}
	}


	#pragma endregion

	if (isOnPlatform)
	{
		ay = MARIO_GRAVITY;
	}

	HandleFlying(dt);
	HandleGliding(dt);
	HandleTailAttack(dt, coObjects);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

#pragma region Collision
void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CMysteryBox*>(e->obj))
		OnCollisionWithMysteryBox(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->HasWings())
		{
			goomba->SetWings(false);
			goomba->SetGravity(GOOMBA_GRAVITY);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	if (mushroom != NULL)
	{
		mushroom->Delete();
		if (level == MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_BIG;

			// Move up a bit to avoid being stuck in the ground
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		}

		else if (level == MARIO_LEVEL_BIG)
			coin += 100;
	}
}

void CMario::OnCollisionWithMysteryBox(LPCOLLISIONEVENT e)
{
	CMysteryBox* box = dynamic_cast<CMysteryBox*>(e->obj);
	if (box != nullptr && e->ny > 0)
	{
		box->SetState(MYSTERY_BOX_STATE_HIT);
	}
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
	if (plant != NULL)
	{
		if (untouchable == 0)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (!koopa) return;

	int koopaState = koopa->GetState();

	// If Mario is carrying the Koopa but it turns back to walking, drop it
	if (isCarrying && carriedKoopa == koopa && koopaState == KOOPA_STATE_WALKING)
	{
		isCarrying = false;
		carriedKoopa->SetBeingCarried(false);
		carriedKoopa = nullptr;
	}

	if (e->ny < 0)
	{
		// Mario stomped The Paratroopa
		if (koopaState == KOOPA_STATE_PARATROOPA)
		{
			koopa->SetState(KOOPA_STATE_WALKING);
			koopa->SetWings(false);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else

			// Mario stomped the Koopa
			if (koopaState == KOOPA_STATE_WALKING ||
				koopaState == KOOPA_STATE_SHELL_MOVING_LEFT ||
				koopaState == KOOPA_STATE_SHELL_MOVING_RIGHT)
			{
				koopa->SetState(KOOPA_STATE_SHELL);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else if (koopaState == KOOPA_STATE_SHELL)
			{
				if ((state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT) && !isCarrying)
				{
					isCarrying = true;
					carriedKoopa = koopa;
					koopa->SetBeingCarried(true);
				}
				else
				{
					// Kick if not carrying
					koopa->OnStompedByMario(x);
				}
			}
	}
	else
	{
		// Side collision
		if (isCarrying && carriedKoopa == koopa)
		{
			// Could add logic to drop or throw here
		}
		else if (koopaState == KOOPA_STATE_SHELL &&
			(state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_RUNNING_LEFT) &&
			!isCarrying)
		{
			isCarrying = true;
			carriedKoopa = koopa;
			koopa->SetBeingCarried(true);
		}
		else if (koopaState == KOOPA_STATE_WALKING ||
			koopaState == KOOPA_STATE_SHELL_MOVING_LEFT ||
			koopaState == KOOPA_STATE_SHELL_MOVING_RIGHT)
		{
			if (untouchable == 0)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	if (leaf != NULL)
	{
		leaf->Delete();
		if (level == MARIO_LEVEL_BIG)
		{
			SetLevel(MARIO_LEVEL_RACCOON);
		}
	}
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}


#pragma endregion

void CMario::ReleaseCarriedKoopa()
{
	if (isCarrying && carriedKoopa != nullptr)
	{
		carriedKoopa->SetBeingCarried(false);
		carriedKoopa->SetState(nx > 0 ? KOOPA_STATE_SHELL_MOVING_RIGHT : KOOPA_STATE_SHELL_MOVING_LEFT);
		carriedKoopa->SetPosition(x + (nx > 0 ? 6 : -6), y - 6);
		isCarrying = false;
		carriedKoopa = nullptr;
	}
}

#pragma region Animation

// Get animation ID for small Mario
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

// Get animdation ID for big Mario
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

// Get animation ID for raccoon Mario
int CMario::GetAniIdRaccoon()
{
	int aniId = -1;
	if (isAttacking)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT;
		return aniId;
	}

	if (isFlying)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_FLYING_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_FLYING_LEFT;
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else
	{
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;
}

#pragma endregion



void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon();

	animations->Get(aniId)->Render(GetRenderX(), y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

float CMario::GetRenderX()
{
	if (level == MARIO_LEVEL_RACCOON && !isAttacking)
	{
		if (nx > 0) return x - MARIO_RACOON_TAIL_WIDTH_DIFF/2;                     
		else return x + MARIO_RACOON_TAIL_WIDTH_DIFF/2;
	}
	return x;
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;

	case MARIO_STATE_FLYING:
		if (!isOnPlatform && canFly && level == MARIO_LEVEL_RACCOON)
		{
			vy = -MARIO_AIRJUMP_RUN_SPEED_Y;
			ay = MARIO_FLYING_GRAVITY;

			isFlying = true;
		}
		break;
	case MARIO_STATE_GLIDING:
		if (!isOnPlatform && level == MARIO_LEVEL_RACCOON)
		{
			isGliding = true;
			vy = 0.0f;
			ay = MARIO_GLIDING_GRAVITY;
		}
		break;
	case MARIO_STATE_TAIL_ATTACK:
		if (level == MARIO_LEVEL_RACCOON)
		{
			isAttacking = true;
		}
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::HandleFlying(DWORD dt)
{
	if (isFlying)
	{
		ay = MARIO_FLYING_GRAVITY;

		currentFlyingTime += dt;
		if (currentFlyingTime > MARIO_MAX_FLYING_TIME)
		{
			isFlying = false;
			currentFlyingTime = 0;
			vy = 0.0f;
			ay = MARIO_GRAVITY;
			canFly = false;

			// Slowly reduce speed
			if (abs(vx) > MARIO_RUNNING_SPEED)
			{
				if (vx > 0)
				{
					vx -= MARIO_FLYING_DECELERATION * dt;
				}
				else
				{
					vx += MARIO_FLYING_DECELERATION * dt;
				}
			}
		}
	}

	if (isOnPlatform)
	{
		if (abs(this->vx) == MARIO_RUNNING_SPEED)
		{
			canFly = true;
		}
		else
		{
			canFly = false;
		}
	}
}

void CMario::HandleGliding(DWORD dt)
{
	if (isOnPlatform)
	{
		isGliding = false;
		currentGlidingTime = 0;
		ay = MARIO_GRAVITY;
	}

	if (isGliding)
	{
		currentGlidingTime += dt;
		if (currentGlidingTime >= MARIO_FLAPING_TIME_PER_FRAME * 3)
		{
			isGliding = false;
			currentGlidingTime = 0;
			ay = MARIO_GRAVITY;
		}
	}
}

void CMario::ResetTailAttackAnimation()
{
	currentTailAttackTime = 0;
}

void CMario::HandleTailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isAttacking)
	{
		currentTailAttackTime += dt;

		if (currentTailAttackTime >= MARIO_TAIL_ATTACK_TIME_PER_FRAME * 5)
		{
			isAttacking = false;
			currentTailAttackTime = 0;
			tail->SetActive(false);
			return;
		}

		float tailX, tailY;

		if (currentTailAttackTime < MARIO_TAIL_ATTACK_TIME_PER_FRAME ||
			(currentTailAttackTime >= MARIO_TAIL_ATTACK_TIME_PER_FRAME * 4 &&
				currentTailAttackTime < MARIO_TAIL_ATTACK_TIME_PER_FRAME * 5))
		{
			tailX = (nx > 0) ? x + MARIO_BIG_BBOX_WIDTH / 2 : x - TAIL_BBOX_WIDTH;
		}
		else if (currentTailAttackTime >= MARIO_TAIL_ATTACK_TIME_PER_FRAME * 2 &&
			currentTailAttackTime < MARIO_TAIL_ATTACK_TIME_PER_FRAME * 3)
		{
			tailX = (nx > 0) ? x - MARIO_BIG_BBOX_WIDTH / 2 : x + TAIL_BBOX_WIDTH;
		}
		else
		{
			tailX = x;
		}

		tailY = y + MARIO_BIG_BBOX_HEIGHT / 4;

		tail->SetPosition(tailX, tailY);
		tail->SetActive(true);
		tail->Update(dt, coObjects);
	}
}
