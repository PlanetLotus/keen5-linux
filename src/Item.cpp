#include <algorithm>
#include <vector>
#include "helpers.h"
#include "Item.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

Item::Item(int spawnX, int spawnY, int type) {
    ItemType itemType = (ItemType)type;
    this->type = itemType;

    if (itemType == ItemType::AMMO) {
        value = 5;
        animState = 0;
    } else if (itemType == ItemType::GUM) {
        value = 100;
        animState = 4;
    } else if (itemType == ItemType::MARSHMELLOW) {
        value = 200;
        animState = 5;
    } else if (itemType == ItemType::VITALIN) {
        value = 1;
        animState = 2;
    } else {
        printf("Item type not recognized: %d\n", (int)type);
    }

    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;

    srcClip = nullptr;
    frame = 0;
    expireTimer = 0;

    vector<SDL_Rect> vitalinAnim = {
        { TILE_WIDTH * 28, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 29, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 30, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 31, 0, TILE_WIDTH, TILE_HEIGHT }
    };
    vector<SDL_Rect> vitalinDieAnim = {
        { TILE_WIDTH * 32, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 33, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 34, 0, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 35, 0, TILE_WIDTH, TILE_HEIGHT }
    };

    vector<SDL_Rect> ammoAnim = {
        { TILE_WIDTH * 28, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 29, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT }
    };
    vector<SDL_Rect> ammoDieAnim = { { TILE_WIDTH * 30, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT } };

    vector<SDL_Rect> gumAnim = {
        { TILE_WIDTH * 33, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 34, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT }
    };
    vector<SDL_Rect> marshmellowAnim = {
        { TILE_WIDTH * 31, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 32, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT }
    };

    anims[0] = ammoAnim; anims[1] = ammoDieAnim;
    anims[2] = vitalinAnim; anims[3] = vitalinDieAnim;
    anims[4] = gumAnim; anims[5] = marshmellowAnim;

    animate(animState, 11);
}

void Item::animate(int nextState, int frametime) {
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

void Item::beginExpire() {
    // If already called for this instance, return
    if (animState == 1 || animState == 3)
        return;

    if (type == ItemType::AMMO) {
        animState = 1;
        frame = 0;
    } else if (type == ItemType::VITALIN) {
        animState = 3;
        frame = 0;
    } else {
        die();
    }
}

void Item::expire() {
    const int frameCount = 5;

    hitbox.x++;
    hitbox.y--;

    if (expireTimer / FRAMETIME >= frameCount)
        die();

    expireTimer++;
}

void Item::die() {
    auto it = find(itemBatchRef.begin(), itemBatchRef.end(), this);

    if (it != itemBatchRef.end())
        itemBatchRef.erase(it);
}

void Item::update() {
    if (animState == 1 || animState == 3) {
        animate(animState, 3);
        expire();
    } else {
        animate(animState, 11);
    }
}

void Item::draw(Texture* texture, SDL_Rect cameraBox) {
    if (isTileLocationOnScreen(&hitbox, &cameraBox))
        texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}

int Item::getValue() { return value; }
ItemType Item::getType() { return type; }
