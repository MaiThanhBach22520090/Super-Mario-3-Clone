#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CDoor : public CGameObject
{
public:
	CDoor(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {};
};

