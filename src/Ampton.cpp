#include "Ampton.h"
#include "globals.h"

Ampton::Ampton(Player* player) {

}

void Ampton::update() {
}

void Ampton::draw(SDL_Rect cameraBox) {
    gKeenTexture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}
