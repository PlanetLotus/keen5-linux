#include <algorithm>
#include "globals.h"
#include "Laser.h"
#include "Tile.h"

using namespace std;

Laser::Laser(int startX, int startY, float velocityX, float velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;
    isStunned = false;

    xVel = velocityX * speed;
    yVel = velocityY * speed;
    xVelRem = 0;
    yVelRem = 0;

    expireTimer = 0;

    frame = 0;
    animState = 0;

    srcClip = nullptr;

    // Add self to sprite batch
    laserBatchRef.push_back(this);
}

void Laser::animate(int nextState) {
    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * FRAMETIME)
        frame = 0;

    srcClip = &anims[animState][frame / FRAMETIME];
}

void Laser::expire() {
    xVel = 0;
    yVel = 0;

    if (expireTimer / FRAMETIME >= 2) {
        die();
        return;
    }

    expireTimer++;
}

void Laser::die() {
    // Erase this Laser from laserBatch
    // This calls the destructor internally
    auto it = find(laserBatchRef.begin(), laserBatchRef.end(), this);

    if (it != laserBatchRef.end())
        laserBatchRef.erase(it);
}
