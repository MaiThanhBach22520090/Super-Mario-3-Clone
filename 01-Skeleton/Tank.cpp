#include "Tank.h"
#include "Game.h"
#include <Windows.h>

// Constructor
CTank::CTank(float x, float y, LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight, float speed, LPTEXTURE bulletTexture)
    : CGameObject(x, y, texUp), speed(speed), direction(0),
    texUp(texUp), texDown(texDown), texLeft(texLeft), texRight(texRight) {
    vx = 0;
    vy = 0;


    // Initialize the bullet
    bullet = new CBullet(x, y, bulletTexture, 0.05f); // Use the ship's texture for the bullet
    bullet->SetActive(false);
}

// Destructor
CTank::~CTank() {
    if (bullet) delete bullet;
}

// Update method: Updates the ship's position based on velocity
void CTank::Update(DWORD dt) {
    // Update the ship's position based on its velocity
    x += vx * dt;
    y += vy * dt;

    // Clamp the ship's position to the screen boundaries
    int backBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int backBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    if (x < 0) x = 0;
    if (x > backBufferWidth - texture->getWidth()) x = backBufferWidth - texture->getWidth();
    if (y < 0) y = 0;
    if (y > backBufferHeight - texture->getHeight()) y = backBufferHeight - texture->getHeight();

    // Update the bullet
	if (bullet->IsActive()) {
		bullet->Update(dt);
	}
}

// HandleInput method: Processes WASD input to control the ship
void CTank::HandleInput() {
    // Reset velocities
    vx = 0;
    vy = 0;

    // Handle WASD input and set direction
    if (GetAsyncKeyState('W') & 0x8000) {
        vy = -speed; // Move up
        SetDirection(0);
    }
    else if (GetAsyncKeyState('S') & 0x8000) {
        vy = speed; // Move down
        SetDirection(1);
    }
    else if (GetAsyncKeyState('A') & 0x8000) {
        vx = -speed; // Move left
        SetDirection(2);
    }
    else if (GetAsyncKeyState('D') & 0x8000) {
        vx = speed; // Move right
        SetDirection(3);
    }

	// Handle shooting
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		Shoot();
	}
}

// SetDirection method: Updates the ship's direction and texture
void CTank::SetDirection(int dir) {
    if (dir == direction) return; // No change needed

    direction = dir;
    switch (dir) {
    case 0: // Up
        texture = texUp;
        break;
    case 1: // Down
        texture = texDown;
        break;
    case 2: // Left
        texture = texLeft;
        break;
    case 3: // Right
        texture = texRight;
        break;
    }
}

/// Shoot method: Fires a bullet
void CTank::Shoot() {
    if (bullet->IsActive()) return; // Only one bullet allowed

    // Set the bullet's position and velocity based on the ship's direction
    bullet->SetPosition(x + texture->getWidth() / 2, y + texture->getHeight() / 2);
	float bulletSpeed = 0.3f;
    switch (direction) {
    case 0: // Up
        bullet->SetVelocity(0, -bulletSpeed);
        break;
    case 1: // Down
        bullet->SetVelocity(0, bulletSpeed);
        break;
    case 2: // Left
        bullet->SetVelocity(-bulletSpeed, 0);
        break;
    case 3: // Right
        bullet->SetVelocity(bulletSpeed, 0);
        break;
    }
    bullet->SetActive(true);
}