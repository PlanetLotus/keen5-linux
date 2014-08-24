#ifndef SPARKY_H
#define SPARKY_H

#include "SDL.h"

class Sparky : public Sprite {
    private:
        void fall();
        std::vector<SDL_Rect> anims[2];
        void animate(int nextState, int frametime = FRAMETIME);

    public:
        Sparky();
        void update();
        void draw(SDL_Rect cameraBox);
        void takeShotByPlayer();
};

#endif
