#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include <vector>
#include "Laser.h"
#include "SDL.h"

class Enemy;
class Texture;

class BlasterShot : public Laser {
    private:
        static const std::vector<Enemy*>& enemyBatchRef;
    public:
        BlasterShot(int startX, int startY, float velocityX, float velocityY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
};

#endif
