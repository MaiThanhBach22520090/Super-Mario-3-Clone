#include "Game.h"
#include "Mario.h"

CMario::CMario(float x, float y, float vx):CGameObject(x, y)
{
	this->speed = vx;
};

void CMario::Update(DWORD dt)
{
	x += vx*dt;

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {

		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}

	HandleInput();
}

void CMario::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	if (vx > 0) ani = CAnimations::GetInstance()->Get(500);
	else if (vx < 0) ani = CAnimations::GetInstance()->Get(501);
	else ani = CAnimations::GetInstance()->Get(502);

	ani->Render(x, y);
}

void CMario::HandleInput()
{
	// Reset velocities
	vx = 0;

	// Handle WASD input and set direction
	if (GetAsyncKeyState('A') & 0x8000) {
		vx = -speed; // Move left
	}
	else if (GetAsyncKeyState('D') & 0x8000) {
		vx = speed; // Move right
	}
}



void CBrick::Render() {

	LPANIMATION ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);

}

