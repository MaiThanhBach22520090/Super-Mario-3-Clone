#include "Door.h"

void Door::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(700);
	ani->Render(x, y);
}

void Door::Update(DWORD dt)
{
}
