#pragma once

#include "GameObject.h"

#define TAIL_BBOX_WIDTH  16
#define TAIL_BBOX_HEIGHT 8

class CTail : public CGameObject
{
	bool isActive;

public:
	CTail(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldenBrick(LPCOLLISIONEVENT e);

	bool IsActive() { return isActive; }
	void SetActive(bool active) { isActive = active; }
	virtual int IsCollidable() override { return isActive ? 1 : 0; }
};
