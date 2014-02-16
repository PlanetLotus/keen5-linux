#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

class Texture
{
    public:
        Texture();
        ~Texture();

        // Loads image at specified path
        bool loadFromFile( std::string path, bool doSetColorKey );

        #ifdef _SDL_TTF_H
        // Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        #endif

        // Deallocates texture
        void free();

        // Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        // Set blending
        void setBlendMode( SDL_BlendMode blending );

        // Set alpha modulation
        void setAlpha( Uint8 alpha );

        // Renders texture at given point
        void render( int destX, int destY, SDL_Rect* srcClip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        // Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        // The actual hardware texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth;
        int mHeight;
};

#endif
