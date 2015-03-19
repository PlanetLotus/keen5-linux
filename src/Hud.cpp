#include "Hud.h"
#include "Texture.h"

Hud::Hud(Texture* texture) {
    this->texture = texture;

    srcClip = { srcClipX, srcClipY, srcClipWidth, srcClipHeight };
}

void Hud::update() {
    // Probably need a reference to the to-be-written StatsManager here and pull values from it
    // Note: I'm pretty sure points ONLY come from collecting items with point
    // values, and not from things like stunning enemies and completing levels.
}

void Hud::draw() {
    texture->render(screenMarginX, screenMarginY, &srcClip);

    // In addition to drawing the HUD, I need a way of determining whether each value has changed and if so update its box accordingly
}
