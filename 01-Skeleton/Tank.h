#pragma once
#include "GameObject.h"
#include "Bullet.h"

class CTank : public CGameObject {
public:
    float speed; // Movement speed of the ship
    int direction; // Current direction (0: up, 1: down, 2: left, 3: right)

    LPTEXTURE texUp;    // Texture for facing up
    LPTEXTURE texDown;  // Texture for facing down
    LPTEXTURE texLeft;  // Texture for facing left
    LPTEXTURE texRight; // Texture for facing right
	LPTEXTURE bulletTexture; // Texture for the bullet

    CBullet* bullet; // The ship's bullet

public:
    CTank(float x, float y, LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight, float speed, LPTEXTURE bulletTexture);
    ~CTank();

    void Update(DWORD dt) override;
    void HandleInput();
    void SetDirection(int dir); // Set the ship's direction and update its texture
    void Shoot(); // Shoot a bullet
    CBullet* GetBullet() const { return bullet; }
};