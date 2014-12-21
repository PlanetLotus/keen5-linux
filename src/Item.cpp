#include <vector>
#include "Item.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

Item::Item(int spawnX, int spawnY, ItemTypeEnum type, int value) {
    this->type = type;
    this->value = value;

    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH;
    hitbox.y = TILE_HEIGHT;

    frame = 0;
    animState = 0;

    SDL_Rect vitalin0 = { TILE_WIDTH * 28, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalin1 = { TILE_WIDTH * 29, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalin2 = { TILE_WIDTH * 30, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalin3 = { TILE_WIDTH * 31, 0, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect vitalinDie0 = { TILE_WIDTH * 32, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalinDie1 = { TILE_WIDTH * 33, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalinDie2 = { TILE_WIDTH * 34, 0, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect vitalinDie3 = { TILE_WIDTH * 35, 0, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect ammo0 = { TILE_WIDTH * 28, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect ammo1 = { TILE_WIDTH * 29, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect ammoDie = { TILE_WIDTH * 30, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect marshmellow0 = { TILE_WIDTH * 31, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect marshmellow1 = { TILE_WIDTH * 32, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect gum0 = { TILE_WIDTH * 33, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };
    SDL_Rect gum1 = { TILE_WIDTH * 34, TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT };

    SDL_Rect vitalinArray[4] = { vitalin0, vitalin1, vitalin2, vitalin3 };
    SDL_Rect vitalinDieArray[4] = { vitalinDie0, vitalinDie1, vitalinDie2, vitalinDie3 };
    SDL_Rect ammoArray[2] = { ammo0, ammo1 };
    SDL_Rect ammoDieArray[1] = { ammoDie };
    SDL_Rect marshmellowArray[2] = { marshmellow0, marshmellow1 };
    SDL_Rect gumArray[2] = { gum0, gum1 };

    vector<SDL_Rect> vitalinAnim(vitalinArray, vitalinArray + sizeof(vitalinArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> vitalinDieAnim(vitalinDieArray, vitalinDieArray + sizeof(vitalinDieArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> ammoAnim(ammoArray, ammoArray + sizeof(ammoArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> ammoDieAnim(ammoDieArray, ammoDieArray + sizeof(ammoDieArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> marshmellowAnim(marshmellowArray, marshmellowArray + sizeof(marshmellowArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> gumAnim(gumArray, gumArray + sizeof(gumArray) / sizeof(SDL_Rect));

    anims[0] = ammoAnim; anims[1] = ammoDieAnim;
    anims[2] = vitalinAnim; anims[3] = vitalinDieAnim;
    anims[4] = gumAnim; anims[5] = marshmellowAnim;

    if (type == AMMO)
        animState = 0;
    else if (type == VITALIN)
        animState = 2;
    else if (type == GUM)
        animState = 4;
    else if (type == MARSHMELLOW)
        animState = 5;

    animate(animState);
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

void Item::update() {
    animate(animState);
}

void Item::draw(Texture* texture, SDL_Rect cameraBox) {
    texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}

int Item::getValue() { return value; }
ItemTypeEnum Item::getType() { return type; }
