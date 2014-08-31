#ifndef AMPTON_H
#define AMPTON_H

#include "globals.h"
#include "Player.h"
#include "SDL.h"

class Ampton : public Sprite {
    private:
        void animate(int nextState, int frametime = FRAMETIME);

    public:
        Ampton(Player* player);
        void update();
        void draw(SDL_Rect cameraBox);

};

#endif
