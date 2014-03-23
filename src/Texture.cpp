#include "Texture.h"
#include "globals.h"

Texture::Texture() {
    // Init
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture() {
    // Deallocate
    free();
}

bool Texture::loadFromFile(std::string path, bool doSetColorKey) {
    // Get rid of preexisting texture
    free();

    // Final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return false;
    }

    // Color key image
    /*
    if (doSetColorKey)
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
        */

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
    if ( newTexture == NULL ) {
        printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    } else {
        // Get image dimensions
        mWidth = loadedSurface->w;
        mHeight = loadedSurface->h;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    // Return success
    mTexture = newTexture;
    return true;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    return false;
}

void Texture::free() {
    // Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor( Uint8 red, Uint8 green, Uint8 blue ) {
    // Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending ) {
    // Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void Texture::setAlpha( Uint8 alpha ) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void Texture::render(int destX, int destY, SDL_Rect* srcClip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { destX, destY, mWidth, mHeight };

    // Set clip rendering dimensions
    if ( srcClip != NULL )
    {
        renderQuad.w = srcClip->w;
        renderQuad.h = srcClip->h;
    }

    // Render to screen
    SDL_RenderCopyEx( renderer, mTexture, srcClip, &renderQuad, angle, center, flip );
}

int Texture::getWidth() {
    return mWidth;
}

int Texture::getHeight() {
    return mHeight;
}
