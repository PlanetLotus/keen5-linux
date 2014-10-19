#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include <vector>
#include "Enemy.h"
#include "SDL.h"
#include "Sprite.h"

class BlasterShot : public Sprite {
    private:
        int expireTimer;

        std::vector<SDL_Rect> anims[2];
        void animate(int nextState);
        static const std::vector<Enemy*>& enemyBatchRef;
        static std::vector<BlasterShot*>& blasterShotBatchRef;
    public:
        BlasterShot(int startX, int startY, float velocityX, float velocityY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        void expire();
        void die();
};

#endif
