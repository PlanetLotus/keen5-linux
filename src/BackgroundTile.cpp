#include "BackgroundTile.h"
#include "helpers.h"
#include "Texture.h"
#include "Tile.h"

BackgroundTile::BackgroundTile(int srcX, int srcY, int destX, int destY) {
    srcBox.x = srcX;
    srcBox.y = srcY;

    srcBox.w = TILE_WIDTH;
    srcBox.h = TILE_HEIGHT;

    destBox.x = destX;
    destBox.y = destY;

    destBox.w = TILE_WIDTH;
    destBox.h = TILE_HEIGHT;
}

void BackgroundTile::draw(Texture* texture, SDL_Rect cameraBox) {
    if (isTileLocationOnScreen(&destBox, &cameraBox))
        texture->render(destBox.x - cameraBox.x, destBox.y - cameraBox.y, &srcBox);
}
