#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define KOOPA_WIDTH 14

class CKoopa : public CGameObject
{
protected:
	float vx;

public:
	CKoopa(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};

