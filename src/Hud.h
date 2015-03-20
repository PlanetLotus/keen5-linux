#ifndef HUD_H
#define HUD_H

#include <vector>
#include "SDL.h"
#include "Tile.h"

class StatsManager;
class Texture;

class Hud {
    private:
        SDL_Rect srcClip;
        std::vector<SDL_Rect> valueSrcClips;
        Texture* texture;
        StatsManager* statsManager;

        int screenMarginX = TILE_WIDTH / 2;
        int screenMarginY = TILE_HEIGHT / 2;

        int srcClipX = TILE_WIDTH * 11;
        int srcClipY = TILE_HEIGHT * 158;
        int srcClipWidth = TILE_WIDTH * 5;
        int srcClipHeight = TILE_HEIGHT * 2;

        const int row1OffsetY = 8;
        const int row2OffsetY = 40;

        // Can multiply by valueWidth to get other values
        const int score1OffsetX = 10;
        const int lives1OffsetX = 42;
        const int ammo1OffsetX = 122;

        const int valueWidth = 12;
        const int valueHeight = 16;
        const int spaceBetweenValuesX = 4;

        // Number image locations in sprite sheet
        SDL_Rect grayEightClip = { TILE_WIDTH * 10, TILE_HEIGHT * 159, valueWidth, valueHeight };
        SDL_Rect nineClip = { TILE_WIDTH * 10 + valueWidth, TILE_HEIGHT * 159, valueWidth, valueHeight };
        SDL_Rect eightClip = { TILE_WIDTH * 10, TILE_HEIGHT * 159 + valueHeight, valueWidth, valueHeight };
        SDL_Rect sevenClip = { TILE_WIDTH * 10 + valueWidth, TILE_HEIGHT * 159 + valueHeight, valueWidth, valueHeight };
        SDL_Rect sixClip = { TILE_WIDTH * 9, TILE_HEIGHT * 160, valueWidth, valueHeight };
        SDL_Rect fiveClip = { TILE_WIDTH * 9 + valueWidth, TILE_HEIGHT * 160, valueWidth, valueHeight };
        SDL_Rect fourClip = { TILE_WIDTH * 9, TILE_HEIGHT * 160 + valueHeight, valueWidth, valueHeight };
        SDL_Rect threeClip = { TILE_WIDTH * 9 + valueWidth, TILE_HEIGHT * 160 + valueHeight, valueWidth, valueHeight };
        SDL_Rect twoClip = { TILE_WIDTH * 10, TILE_HEIGHT * 160, valueWidth, valueHeight };
        SDL_Rect oneClip = { TILE_WIDTH * 10 + valueWidth, TILE_HEIGHT * 160, valueWidth, valueHeight };
        SDL_Rect zeroClip = { TILE_WIDTH * 10, TILE_HEIGHT * 160 + valueHeight, valueWidth, valueHeight };
    public:
        Hud(Texture* texture, StatsManager* statsManager);
        void update();
        void draw();
};

#endif
