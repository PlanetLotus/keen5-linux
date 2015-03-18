#include "EnemyLaser.h"
#include "Player.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

EnemyLaser::EnemyLaser(int startX, int startY, float velocityX, float velocityY, Player* player, Texture* texture)
    : Laser(startX, startY, velocityX, velocityY) {
    keen = player;
    this->texture = texture;

    // Animation instantiation
    vector<SDL_Rect> moveAnim = {
        { TILE_WIDTH * 7, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 8, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 9, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 10, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT }
    };

    vector<SDL_Rect> collideAnim = {
        { TILE_WIDTH * 11, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 12, TILE_HEIGHT * 50, TILE_WIDTH, TILE_HEIGHT }
    };

    anims = {
        moveAnim,
        collideAnim
    };

    // Set animation default
    animate(0);
}

bool EnemyLaser::isCollidingWithPlayer() {
    return isUnitColliding(keen->getBox());
}

void EnemyLaser::update() {
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
    hitbox.x += xVel * timeDelta;
    hitbox.y += yVel * timeDelta;

    // Reset velocity if collision
    if (tciTB.isTopColliding() || tciTB.isBottomColliding()) yVel = 0;
    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) xVel = 0;

    // Check unit hit
    if (isCollidingWithPlayer()) {
        expire();
        keen->die(hitbox.x);
    }
}

void EnemyLaser::draw(Texture* texture, SDL_Rect cameraBox) {
    this->texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}
