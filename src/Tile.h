#ifndef TILE_H
#define TILE_H

#include "SDL.h"
#include "Texture.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILE_OFFSET = 0;

class Tile {
    private:
        SDL_Rect srcBox;
        SDL_Rect destBox;

        bool isSloped;
        bool isEdge;
        float slope;
        bool hasCollision;
        int leftHeight;
        int rightHeight;
        bool collideTop;
        bool collideRight;
        bool collideBottom;
        bool collideLeft;

    public:
        enum TileProperty { LEFT, RIGHT, TOP, BOTTOM, ISPOLE };

        // Inits position and type
        Tile(Texture* texture, int srcX, int srcY, int destX, int destY, int lHeight, int rHeight,
            bool collideT, bool collideR, bool collideB, bool collideL, int layer, bool isPole, bool isEdge);

        // Shows the tile
        Texture* gMaskTexture;
        void render(SDL_Rect cameraBox);

        SDL_Rect getBox();
        bool isColliding(TileProperty tileProperty, SDL_Rect hitbox, SDL_Rect nextHitbox);
        bool isTouching(TileProperty tileProperty, SDL_Rect nextHitbox);
        bool getIsSloped();
        bool getIsEdge();
        float getSlope();
        int getLeftHeight();
        int getRightHeight();
        bool getHasCollision();
        bool getCollideTop();
        bool getCollideRight();
        bool getCollideBottom();
        bool getCollideLeft();

        bool isPole;
        int layer;
};

#endif
