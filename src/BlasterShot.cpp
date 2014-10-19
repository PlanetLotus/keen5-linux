#include <algorithm>
#include "BlasterShot.h"
#include "globals.h"
#include "helpers.h"

using namespace std;

BlasterShot::BlasterShot(int startX, int startY, float velocityX, float velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;
    isStunned = false;

    xVel = velocityX * 20;
    yVel = velocityY * 20;

    expireTimer = 0;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    // Animation instantiation
    SDL_Rect move0 = {TILE_WIDTH * 6, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move1 = {TILE_WIDTH * 7, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move2 = {TILE_WIDTH * 8, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move3 = {TILE_WIDTH * 9, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

    SDL_Rect collide0 = {TILE_WIDTH * 10, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect collide1 = {TILE_WIDTH * 11, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

    SDL_Rect move_array[4] = { move0, move1, move2, move3 };
    SDL_Rect collide_array[2] = { collide0, collide1 };

    vector<SDL_Rect> move_anim(move_array, move_array + sizeof(move_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> collide_anim(collide_array, collide_array + sizeof(collide_array) / sizeof(SDL_Rect));

    anims[0] = move_anim;
    anims[1] = collide_anim;

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
    vector<BlasterShot*>::iterator it = find(blasterShotBatchRef.begin(), blasterShotBatchRef.end(), this);

    if (it != blasterShotBatchRef.end())
        blasterShotBatchRef.erase(it);
}
