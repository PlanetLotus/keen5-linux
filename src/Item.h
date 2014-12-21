#ifndef ITEM_H
#define ITEM_H

#include "globals.h"
#include "Sprite.h"

// Not zero-indexed because this doesn't contain NONE as the level editor does
enum ItemTypeEnum { AMMO = 1, GUM, MARSHMELLOW, VITALIN };

class Item : public Sprite {
    private:
        int value;
        ItemTypeEnum type;
        std::vector<SDL_Rect> anims[6];

        void animate(int nextState, int frametime = FRAMETIME);
    public:
        Item(int spawnX, int spawnY, int type);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        int getValue();
        ItemTypeEnum getType();
};

#endif