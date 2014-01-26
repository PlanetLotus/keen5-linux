//#include <string>

#include "helpers.h"
#include "Texture.h"
#include "globals.h"

using namespace std;

// Surface clips
SDL_Rect clipsRight[5];
SDL_Rect clipsLeft[5];

SDL_Texture *load_image(std::string path) {
    SDL_Surface *loadedSurface = NULL;
    SDL_Texture *newTexture = NULL;

    // Load image
    loadedSurface = IMG_Load(path.c_str());

    // If nothing went wrong loading the image
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }

    // Create an optimized image
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }

    // Free old image
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip=NULL) {
    // Make a temp rect to hold the offsets
    SDL_Rect offset;

    // Give the offsets to the rect
    offset.x = x;
    offset.y = y;

    // Blit the surface
    //SDL_BlitSurface(source, clip, destination, &offset);
}

bool init() {
    // Init all SDL subsystems
    if (SDL_Init( SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO ) == -1) {
        printf("SDL couldn't be initialized. SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init IMG
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if (initted&flags != flags) {
        printf("Couldn't init SDL_image. IMG_Error: %s\n", IMG_GetError());
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning: Linear texture filtering not enabled!\n");

    // Set up screen
    SDL_CreateWindowAndRenderer(
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN,
                                &window,
                                &renderer
                                );

    // Make sure window was successfully set up
    if (window == NULL || renderer == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Add screen to window
    //surfaces.push_back(screen);
    //screen = SDL_GetWindowSurface(window);

    // Initialize SDL_ttf
    if (TTF_Init() == -1) return false;

    // Update screen
    SDL_RenderPresent(renderer);

    return true;
}

bool load_files() {
    // Load images
    if (!background->loadFromFile("../data/background.png")) {
        printf("Failed to load background texture!\n");
        return false;
    }
    if (!keen->loadFromFile("../data/keensprite.png")) {
        printf("Failed to load keen texture!\n");
        return false;
    }

    // Add to surfaces
    //surfaces.push_back(background);
    //surfaces.push_back(keen);

    // Open the fonts
    //TTF_Font *font = TTF_OpenFont ("lazy.ttf", 28);

    //if (font == NULL) return false;

    return true;
}

void clean_up() {
    // Free images
    /*for (vector<SDL_Surface*>::iterator i=surfaces.begin(); i!=surfaces.end(); ++i) {
        SDL_FreeSurface(*i);
    }*/

    // Close font
    //TTF_CloseFont(font);

    // Quit SDL_TTF
    TTF_Quit();

    // Quit SDL
    SDL_Quit();
}
