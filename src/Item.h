#ifndef ITEM_H
#define ITEM_H

#include "Sprite.h"

enum ItemTypeEnum { AMMO, VITALIN, GUM, MARSHMELLOW };

class Item : public Sprite {
    private:
        int value;
        ItemTypeEnum type;
    public:
        Item(ItemTypeEnum type, int value);
        int getValue();
        ItemTypeEnum getType();
};

#endif
