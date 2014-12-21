#ifndef ITEM_H
#define ITEM_H

#include "Sprite.h"

enum ItemTypeEnum { AMMO, VITALIN, GUM, MARSHMELLOW };

class Item : public Sprite {
    private:
        int value;
        ItemTypeEnum type;
        std::vector<SDL_Rect> anims[6];
    public:
        Item(int spawnX, int spawnY, ItemTypeEnum type, int value);
        int getValue();
        ItemTypeEnum getType();
};

#endif
