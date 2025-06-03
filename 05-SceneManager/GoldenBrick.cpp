#include "GoldenBrick.h"
#include "Button.h"
#include "PlayScene.h"
#include "Game.h"
#include "Tail.h"
#include "Koopa.h"
#include "Coin.h"

CGoldenBrick::CGoldenBrick(float x, float y, bool hasButton)
    : CGameObject(x, y)
{
    containsButton = hasButton;
    destroyed = false;
    buttonRevealed = false;
}

void CGoldenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - GOLDEN_BRICK_BBOX_WIDTH / 2;
    t = y - GOLDEN_BRICK_BBOX_HEIGHT / 2;
    r = l + GOLDEN_BRICK_BBOX_WIDTH;
    b = t + GOLDEN_BRICK_BBOX_HEIGHT;
}

void CGoldenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoldenBrick::Render()
{
    int aniId = buttonRevealed ? ID_ANI_GOLDEN_BRICK_EMPTY : ID_ANI_GOLDEN_BRICK;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CGoldenBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CTail*>(e->obj))
	{
		OnCollisionWithTail(e);
	}
	else if (dynamic_cast<CKoopa*>(e->obj))
	{
		OnCollisionWithShell(e);
	}
}

void CGoldenBrick::OnCollisionWithTail(LPCOLLISIONEVENT e)
{
	if (isCoin) return; // Already transformed to coin
	if (containsButton && !buttonRevealed)
	{
		buttonRevealed = true;
		LPGAMEOBJECT button = new CButton(x, y - 16);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(button);
	}

}       

void CGoldenBrick::OnCollisionWithShell(LPCOLLISIONEVENT e)
{
	if (isCoin) return; // Already transformed to coin
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	if (koopa != NULL)
	{
		if (containsButton && !buttonRevealed &&
			(koopa->GetState() == KOOPA_STATE_SHELL_MOVING_LEFT || koopa->GetState() == KOOPA_STATE_SHELL_MOVING_RIGHT) &&
			// Can only reveal button if the Koopa hit the brick from side
			e->nx != 0)
		{
			buttonRevealed = true;
			LPGAMEOBJECT button = new CButton(x, y - 16);
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			scene->AddObject(button);
		}
		else
		{
			destroyed = true;
		}
	}


}

void CGoldenBrick::OnButtonActivated()
{
	if (!containsButton)
		// Delete This Brick And Spawn Coin.
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		if (!isCoin)
		{
			isCoin = true;
			LPGAMEOBJECT coin = new CCoin(x, y, 0);
			scene->AddObject(coin);
		}

		destroyed = true;
		this->Delete();
	}
}
