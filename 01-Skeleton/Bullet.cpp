#include "Bullet.h"
#include "Game.h"

// Constructor
CBullet::CBullet(float x, float y, LPTEXTURE texture, float speed)
    : CGameObject(x, y, texture), speed(speed), active(false) {
    // Initialize velocities to 0
    vx = 0;
    vy = 0;
}

// Update method: Moves the bullet and checks for screen boundaries
void CBullet::Update(DWORD dt) {
    if (!active) return; // Do nothing if the bullet is inactive

    // Move the bullet
    x += vx * dt;
    y += vy * dt;

    // Check if the bullet is out of the screen
    int backBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int backBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    if (x < 0 || x > backBufferWidth || y < 0 || y > backBufferHeight) {
        active = false; // Deactivate the bullet
    }
}

// Render method: Draws the bullet if it's active
void CBullet::Render() {
    if (active) {
        CGameObject::Render();
    }
}

