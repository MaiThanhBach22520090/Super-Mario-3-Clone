#include "EnemyTank.h"
#include "Game.h"
#include <cstdlib> // For rand()
#include <ctime>   // For time()

CEnemyTank::CEnemyTank(float x, float y, LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight, float speed, LPTEXTURE bulletTexture)
    : CTank(x, y, texUp, texDown, texLeft, texRight, speed, bulletTexture), movedTimer(0) {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator
    HandleMove(); // Initialize the first movement
}

CEnemyTank::~CEnemyTank() {
}

void CEnemyTank::Update(DWORD dt) {
    // Update the tank's position based on its velocity
    x += vx * dt;
    y += vy * dt;

    // Clamp the tank's position to the screen boundaries
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

    // Handle movement
	movedTimer -= dt;
    if (movedTimer <= 0) {
        // Set a random movement duration
        HandleMove();
    }

    Shoot();
}

void CEnemyTank::HandleMove() {
    // Choose a random direction to move
    int dir = rand() % 4;

    // Set the direction and update the texture
    SetDirection(dir);

    // Move in the chosen direction
    switch (dir) {
    case 0: // Up
        vx = 0;
        vy = -speed;
        break;
    case 1: // Down
        vx = 0;
        vy = speed;
        break;
    case 2: // Left
        vx = -speed;
        vy = 0;
        break;
    case 3: // Right
        vx = speed;
        vy = 0;
        break;
    }

    movedTimer = minMoveTime + rand() % (maxMoveTime - minMoveTime);
}

void CEnemyTank::SetDirection(int dir) {
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

void CEnemyTank::Shoot() {
    if (!bullet->IsActive()) {
        bullet->SetActive(true);
        bullet->SetPosition(x + texture->getWidth() / 2, y + texture->getHeight() / 2);
        float BulletSpeed = 0.05f;
        switch (direction) {
        case 0: // Up
            bullet->SetVelocity(0, -BulletSpeed);
            break;
        case 1: // Down
            bullet->SetVelocity(0, BulletSpeed);
            break;
        case 2: // Left
            bullet->SetVelocity(-BulletSpeed, 0);
            break;
        case 3: // Right
            bullet->SetVelocity(BulletSpeed, 0);
            break;
        }
    }
}