#include "Ampton.h"
#include "globals.h"

using namespace std;

Ampton::Ampton(Player* player) {
    hitbox.x = TILE_WIDTH * 14;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    facing = LEFT;
    patrolSpeed = 4;
    xVel = patrolSpeed * facing;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;

    state = PATROL;
    frame = 0;
    animState = 0;
    srcClip = NULL;

    keen = player;
    isStunned = false;

    SDL_Rect walkL0 = { TILE_WIDTH * 12, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 14, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 16, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 18, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkR0 = { TILE_WIDTH * 12, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR1 = { TILE_WIDTH * 14, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR2 = { TILE_WIDTH * 16, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR3 = { TILE_WIDTH * 18, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkRightArray[4] = { walkR0, walkR1, walkR2, walkR3 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkRightAnim(walkRightArray, walkRightArray + sizeof(walkRightArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
    anims[1] = walkRightAnim;
}

void Ampton::animate(int nextState, int frametime) {
    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * frametime)
        frame = 0;

    srcClip = &anims[animState][frame / frametime];
}

void Ampton::update() {
    animate(0);

    xVel += xVelRem;
    yVel += abs(yVelRem);

    hitbox.x += xVel;
    hitbox.y += yVel;

    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);
}

void Ampton::draw(SDL_Rect cameraBox) {
    // Bottom-align the hitbox for tall frames
    int offsetY = srcClip->h - TILE_HEIGHT * 2;
    int destY = hitbox.y - offsetY;

    gKeenTexture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, srcClip);
}
