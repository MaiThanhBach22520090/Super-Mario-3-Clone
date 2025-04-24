#include "Koopa.h"
#include "Game.h"

CKoopa::CKoopa(float x, float y, float vx) : CGameObject(x, y)
{
	this->vx = vx;
}

void CKoopa::Update(DWORD dt)
{
	x += vx * dt;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - KOOPA_WIDTH) {
		vx = -vx;
		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - KOOPA_WIDTH)
		{
			x = (float)(BackBufferWidth - KOOPA_WIDTH);
		}
	}
}

void CKoopa::Render()
{
	LPANIMATION ani;
	if (vx < 0) ani = CAnimations::GetInstance()->Get(600);
	else ani = CAnimations::GetInstance()->Get(601);

	ani->Render(x, y);

}
