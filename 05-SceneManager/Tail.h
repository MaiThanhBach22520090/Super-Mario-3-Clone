#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	8

#define TAIL_EXIST_TIME		4000 // milliseconds

class CTail : public CGameObject
{
	bool isActive;

public:
	CTail(float x, float y);

	void SetActive(bool active) { isActive = active; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	int IsCollidable() override { return isActive; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
};
