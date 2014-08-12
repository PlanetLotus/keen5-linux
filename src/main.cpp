#include <iostream>
#include "BlasterShot.h"
#include "globals.h"
#include "helpers.h"
#include "Player.h"
#include "Timer.h"

using namespace std;

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

    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    Sprite* character = new Player();
    Player* characterPlayerPtr = dynamic_cast<Player*>(character);
    gSpriteBatch[0] = character;

    while (running) {
        // Start timer
        fps.start();

        // Handle user input

        // For non-game-related commands, like pressing Esc to get to the menu,
        // call that function separately here.

        // For game-related keypresses, pass it as a parameter...? into the
        // game processing loop.

        while (SDL_PollEvent(&event)) {
            // If a key was pressed
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LCTRL: gController.IsHoldingCtrl = false; break;
                    case SDLK_SPACE: gController.IsHoldingSpace = false; break;
                    case SDLK_LALT: gController.IsHoldingAlt = false; break;
                }
            }
            // If the user X'd out of the window
            else if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render tiles - Layer 0 (Before units)
        for (unsigned int i=0; i<gTiles.size(); i++) {
            for (unsigned int j=0; j<gTiles[i].size(); j++) {
                if (gTiles[i][j] != NULL && gTiles[i][j]->Layer == 0)
                    gTiles[i][j]->render(&camera);
            }
        }

        // Update units and draw them
        // Be sure to update THEN draw. Don't let the order get messed up across several units
        // All logic should be handled before any of the units on-screen get updated
        for (unsigned int i = 0; i < gSpriteBatch.size(); i++)
            gSpriteBatch[i]->update();
        for (unsigned int i = 0; i < gSpriteBatch.size(); i++)
            gSpriteBatch[i]->draw(&camera);

        // Render tiles - Layer 1 (After units)
        for (unsigned int i=0; i<gTiles.size(); i++) {
            for (unsigned int j=0; j<gTiles[i].size(); j++) {
                if (gTiles[i][j] != NULL && gTiles[i][j]->Layer == 1)
                    gTiles[i][j]->render(&camera);
            }
        }

        UpdateCamera(&camera, character->GetBox(), characterPlayerPtr->GetIsOnGround());

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
