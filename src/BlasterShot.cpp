#include "BlasterShot.h"

SDL_Rect MOVE0 = {TILE_WIDTH * 6, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
SDL_Rect MOVE1 = {TILE_WIDTH * 7, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
SDL_Rect MOVE2 = {TILE_WIDTH * 8, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
SDL_Rect MOVE3 = {TILE_WIDTH * 9, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

SDL_Rect COLLIDE0 = {TILE_WIDTH * 10, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
SDL_Rect COLLIDE1 = {TILE_WIDTH * 11, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

SDL_Rect MOVE_ARRAY[4] = { MOVE0, MOVE1, MOVE2, MOVE3 };
vector<SDL_Rect> MOVE_ANIM(MOVE_ARRAY, MOVE_ARRAY + sizeof(MOVE_ARRAY) / sizeof(SDL_Rect));

SDL_Rect COLLIDE_ARRAY[2] = { COLLIDE0, COLLIDE1 };
vector<SDL_Rect> COLLIDE_ANIM(COLLIDE_ARRAY, COLLIDE_ARRAY + sizeof(COLLIDE_ARRAY) / sizeof(SDL_Rect));

static vector<SDL_Rect> ANIMS[2] = { MOVE_ANIM, COLLIDE_ANIM };

BlasterShot::BlasterShot(int startX, int startY, int velocityX, int velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;

    xVel = velocityX;
    yVel = velocityY;

    frame = 0;
    animState = 0;

    srcClip = NULL;
}

void BlasterShot::update() {
    // Check collision
    animate(0);
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

    if (frame == ANIMS[animState].size() * FRAMETIME)
        frame = 0;

    srcClip = &ANIMS[animState][frame / FRAMETIME];
}

void BlasterShot::draw() {
    hitbox.x += xVel;
    hitbox.y += yVel;

    gKeenTexture->render(hitbox.x, hitbox.y, srcClip);
}
