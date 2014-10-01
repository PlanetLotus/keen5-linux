#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

class Texture {
    public:
        Texture(SDL_Renderer* rendererFromMain);
        ~Texture();

        // Loads image at specified path
        bool loadFromFile(std::string path, bool doSetColorKey);

        // Deallocates texture
        void free();

        // Set color modulation
        void setColor(Uint8 red, Uint8 green, Uint8 blue);

        // Set blending
        void setBlendMode(SDL_BlendMode blending);

        // Set alpha modulation
        void setAlpha(Uint8 alpha);

        // Renders texture at given point
        void render(int destX, int destY, SDL_Rect* srcClip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        // Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        // The actual hardware texture
        SDL_Texture* texture;

        SDL_Renderer* renderer;

        // Image dimensions
        int width;
        int height;
};

#endif
