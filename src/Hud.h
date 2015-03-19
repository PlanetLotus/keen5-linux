#ifndef HUD_H
#define HUD_H

#include "SDL.h"
#include "Tile.h"

class Texture;

class Hud {
    /* This is a little tricky because the number values on the HUD don't align
     * well with tiles. I'll need to hardcode each value's position on the HUD
     * and update it with the 11 possible number values (as images). These
     * values are represented by a 12x16 image which I'll need to hardcode
     * positions for as well. */
    private:
        SDL_Rect srcClip;
        Texture* texture;

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

        // Number image locations in sprite sheet
        int grayEightX = TILE_WIDTH * 10;
        int grayEightY = TILE_HEIGHT * 159;
        int NineX = TILE_WIDTH * 10 + valueWidth;
        int NineY = TILE_HEIGHT * 159;
        int EightX = TILE_WIDTH * 10;
        int EightY = TILE_HEIGHT * 159 + valueHeight;
        int SevenX = TILE_WIDTH * 10 + valueWidth;
        int SevenY = TILE_HEIGHT * 159 + valueHeight;
        int SixX = TILE_WIDTH * 9;
        int SixY = TILE_HEIGHT * 160;
        int FiveX = TILE_WIDTH * 9 + valueWidth;
        int FiveY = TILE_HEIGHT * 160;
        int FourX = TILE_WIDTH * 9;
        int FourY = TILE_HEIGHT * 160 + valueHeight;
        int ThreeX = TILE_WIDTH * 9 + valueWidth;
        int ThreeY = TILE_HEIGHT * 160 + valueHeight;
        int TwoX = TILE_WIDTH * 10;
        int TwoY = TILE_HEIGHT * 160;
        int OneX = TILE_WIDTH * 10 + valueWidth;
        int OneY = TILE_HEIGHT * 160;
        int ZeroX = TILE_WIDTH * 10;
        int ZeroY = TILE_HEIGHT * 160 + valueHeight;
    public:
        Hud(Texture* texture);
        void update();
        void draw();
};

#endif
