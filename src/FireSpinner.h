#ifndef FIRESPINNER_H
#define FIRESPINNER_H

#include <vector>
#include "globals.h"
#include "Tile.h"

class FireSpinner : public Tile {
    private:
        void animate(int frametime = FRAMETIME);
        std::vector<SDL_Rect> spinAnim;
        unsigned int frame;

    public:
        FireSpinner(int destX, int destY, int layer);
        void update();
};

#endif
