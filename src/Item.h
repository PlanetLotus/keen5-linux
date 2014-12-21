#ifndef ITEM_H
#define ITEM_H

#include "globals.h"
#include "Sprite.h"

enum ItemTypeEnum { AMMO, VITALIN, GUM, MARSHMELLOW };

class Item : public Sprite {
    private:
        int value;
        ItemTypeEnum type;
        std::vector<SDL_Rect> anims[6];

        void animate(int nextState, int frametime = FRAMETIME);
    public:
        Item(int spawnX, int spawnY, ItemTypeEnum type, int value);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        int getValue();
        ItemTypeEnum getType();
};

#endif
