#ifndef TILE_H
#define TILE_H

#include "SDL.h"

class Texture;

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILE_OFFSET = 0;

class Tile {
    protected:
        SDL_Rect srcBox;
        SDL_Rect destBox;

        bool isSloped;
        bool isEdge;
        bool isDeadly;
        float slope;
        bool hasCollision;
        int leftHeight;
        int rightHeight;
        bool collideTop;
        bool collideRight;
        bool collideBottom;
        bool collideLeft;

    public:
        enum class TileProperty { LEFT, RIGHT, TOP, BOTTOM, ISPOLE };

        // Inits position and type
        Tile(int srcX, int srcY, int destX, int destY, int lHeight, int rHeight,
            bool collideT, bool collideR, bool collideB, bool collideL, int layer, bool isPole, bool isPoleEdge, bool isEdge, bool isDeadly);

        // Shows the tile
        void draw(Texture* texture, SDL_Rect cameraBox);

        SDL_Rect getBox();
        bool isColliding(TileProperty tileProperty, SDL_Rect hitbox, SDL_Rect nextHitbox);
        bool isTouching(TileProperty tileProperty, SDL_Rect nextHitbox);
        bool getIsSloped();
        bool getIsEdge();
        bool getIsDeadly();
        float getSlope();
        int getLeftHeight();
        int getRightHeight();
        bool getHasCollision();
        bool getCollideTop();
        bool getCollideRight();
        bool getCollideBottom();
        bool getCollideLeft();

        bool isPole;
        bool isPoleEdge;
        int layer;
};

#endif
