#include <algorithm>
#include "BlasterShot.h"
#include "Enemy.h"
#include "globals.h"
#include "helpers.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

BlasterShot::BlasterShot(int startX, int startY, float velocityX, float velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;
    isStunned = false;

    xVel = velocityX * 20;
    yVel = velocityY * 20;
    xVelRem = 0;
    yVelRem = 0;

    expireTimer = 0;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    // Animation instantiation
    vector<SDL_Rect> moveAnim = {
        { TILE_WIDTH * 6, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 7, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 8, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 9, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT }
    };

    vector<SDL_Rect> collideAnim = {
        { TILE_WIDTH * 10, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 11, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT }
    };

    anims = {
        moveAnim,
        collideAnim
    };

    // Add self to sprite batch
    blasterShotBatchRef.push_back(this);

    // Set animation default
    animate(0);
}

void BlasterShot::update() {
    // Check left/right collision
    TileCollisionInfo tciLR;
    if (xVel != 0) {
        tciLR = checkTileCollisionLR();

        // Set properties based on x-collision
        if (tciLR.isLeftColliding()) {
            xVel = (tciLR.tileCollidingWithLeft->getBox().x + tciLR.tileCollidingWithLeft->getBox().w) - hitbox.x;
        } else if (tciLR.isRightColliding()) {
            xVel = tciLR.tileCollidingWithRight->getBox().x - (hitbox.x + hitbox.w);
        }
    }

    // Check top/bottom collision
    TileCollisionInfo tciTB;
    if (yVel != 0) {
        tciTB = checkTileCollisionTB();

        // Set properties based on y-collision
        if (tciTB.isTopColliding()) {
            yVel = (tciTB.tileCollidingWithTop->getBox().y + tciTB.tileCollidingWithTop->getBox().h) - hitbox.y;
        } else if (tciTB.isBottomColliding()) {
            yVel = tciTB.tileCollidingWithBottom->getBox().y - (hitbox.y + hitbox.h);
        }
    }

    if (xVel == 0 && yVel == 0) {
        expire();
        animate(1);
        return;
    } else {
        animate(0);
    }

    // Update hitbox
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Reset velocity if collision
    if (tciTB.isTopColliding() || tciTB.isBottomColliding()) yVel = 0;
    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) xVel = 0;

    // Check unit hit
    for (unsigned int i = 0; i < enemyBatchRef.size(); i++) {
        Enemy* unit = enemyBatchRef[i];
        if (unit != NULL && !unit->getIsStunned() && isUnitColliding(unit->getBox())) {
            expire();
            unit->takeShotByPlayer();
            break;
        }
    }
}

void BlasterShot::animate(int nextState) {
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

void BlasterShot::draw(Texture* texture, SDL_Rect cameraBox) {
    texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}

void BlasterShot::expire() {
    xVel = 0;
    yVel = 0;

    if (expireTimer / FRAMETIME >= 2) {
        die();
        return;
    }

    expireTimer++;
}

void BlasterShot::die() {
    // Erase this BlasterShot from blasterShotBatch
    // This calls the destructor internally
    auto it = find(blasterShotBatchRef.begin(), blasterShotBatchRef.end(), this);

    if (it != blasterShotBatchRef.end())
        blasterShotBatchRef.erase(it);
}
