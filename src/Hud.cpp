#include "Hud.h"
#include "StatsManager.h"
#include "Texture.h"

Hud::Hud(Texture* texture, StatsManager* statsManager) {
    this->texture = texture;
    this->statsManager = statsManager;

    srcClip = { srcClipX, srcClipY, srcClipWidth, srcClipHeight };

    valueSrcClips = {
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip,
        eightClip
    };
}

void Hud::update() {
    // Probably need a reference to the to-be-written StatsManager here and pull values from it
    // Note: I'm pretty sure points ONLY come from collecting items with point
    // values, and not from things like stunning enemies and completing levels.
}

void Hud::draw() {
    texture->render(screenMarginX, screenMarginY, &srcClip);

    // In addition to drawing the HUD, I need a way of determining whether each value has changed and if so update its box accordingly
    for (int i = 0; i < 9; i++)
        texture->render(screenMarginX + score1OffsetX + valueWidth * i + spaceBetweenValuesX * i, screenMarginY + row1OffsetY, &valueSrcClips[i]);

    for (int i = 9; i < 11; i++)
        texture->render(screenMarginX + lives1OffsetX + valueWidth * (i - 9) + spaceBetweenValuesX * (i - 9), screenMarginY + row2OffsetY, &valueSrcClips[i]);

    for (int i = 11; i < 13; i++)
        texture->render(screenMarginX + ammo1OffsetX + valueWidth * (i - 11) + spaceBetweenValuesX * (i - 11), screenMarginY + row2OffsetY, &valueSrcClips[i]);
}
