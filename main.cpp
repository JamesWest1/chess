#include "chess.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>
#include "gui.h"


int main() {
    gui GameController = gui();
    SDL_Event event;
    bool running = true;
    while (running) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                int x, y;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    GameController.pressMouse(x, y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&x, &y);
                    GameController.unpressMouse(x, y);
                case SDL_KEYDOWN: {
                    running = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                    break;
                }
                case SDL_QUIT: {
                    running = false;
                    break;
                }
                default:
                    break;
            }
        }
    GameController.clearScreen();
    GameController.loadBoard();
    GameController.renderFrame();
    }
    GameController.shutDown();
    return 1;
}