#include <iostream>

#include "Texture.h"
#include "globals.h"
#include "helpers.h"
#include "Timer.h"
#include "Player.h"

int main (int argc, char **args) {
    // Initialize variables
    bool running = true;
    SDL_Event event;
    Timer fps;

    // Initialize SDL
    if (!init()) return 1;
    if (!load_files()) return 1;
    if (!set_tiles()) return 1;

    // Redirect stdout back to console
    /*
    FILE *ctt = fopen("CON", "w");
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    */

    // Create player object
    Player character;

    while (running) {
        // Start timer
        fps.start();

        // Handle user input

        // For non-game-related commands, like pressing Esc to get to the menu,
        // call that function separately here.

        // For game-related keypresses, pass it as a parameter...? into the
        // game processing loop.

        if (SDL_PollEvent(&event)) {
            // If a key was pressed
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_RIGHT: character.walk(RIGHT); break;
                    //case SDLK_DOWN: character.set_state(STANDR); break;
                }
            } else {
                // Later: Make this call "idle" which checks the last state
                // If it's a movement, figure out which way you should be standing
                // Else, Add to the time he's been idling for animation purposes
                character.set_state(STANDR);
            }

            // If the user X'd out of the window
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render masks
        SDL_Rect camera = {32, 32, 32, 32};
        for (unsigned int i=0; i<gTiles.size(); i++) {
            gTiles[i]->render(0, 0, camera);
        }

        // Render objects
        /*
        SDL_Rect clip = {66, 34, 32, 32};
        clip.x = 0;
        clip.y = 0;
        clip.w = 40;
        clip.h = 80;
        gKeenTexture->render(64, 16, &clip);
        */

        // Render units
        character.draw();

        // Update screen
        SDL_RenderPresent(renderer);

        // Cap frame rate
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
            SDL_Delay((1000/FRAMES_PER_SECOND) - fps.get_ticks());
    }

    // Prepare for quit
    clean_up();
    //fclose(ctt);

    return 0;
}
