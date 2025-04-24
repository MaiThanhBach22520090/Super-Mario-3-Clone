#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class Door : public CGameObject
{
public:
	Door(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {};
};

