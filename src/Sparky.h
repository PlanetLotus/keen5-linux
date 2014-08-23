#ifndef SPARKY_H
#define SPARKY_H

#include "SDL.h"

class Sparky : public Sprite {
    private:
        std::vector<SDL_Rect> anims[1];
        void animate(int nextState, int frametime = FRAMETIME);
        bool stunned;

    public:
        Sparky();
        void update();
        void draw(SDL_Rect cameraBox);
        void takeShotByPlayer();
};

#endif
