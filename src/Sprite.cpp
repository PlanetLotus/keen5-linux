#include "Sprite.h"

void Sprite::update() {}

/*
void Sprite::animate(int nextState) {
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
*/

void Sprite::draw() {}
