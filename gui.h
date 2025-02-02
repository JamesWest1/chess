#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include "chess.h"
#include "piece.h"

#define WINDOWSIZE 500
#define PIECESIZE (WINDOWSIZE / 9)

enum GameMode {local, online};

class gui {
    public:
        gui();
        void shutDown();
        void renderPath(std::string path, int x, int y, int size);
        void renderFrame();     // renders the current state
        void loadBoard();       // loads the boards state into the renderer
        void clearScreen();     // clears the current state (visually)
        std::pair<int, int> rowColToCoord(int row, int col);
        void pressMouse(int x, int y);
        void unpressMouse(int x, int y);
        
        
    private:
        std::pair<int, int> correctPerspective(int row, int col);
        std::string getImagePath(int row, int col);
        void renderFromPlayersTurn();
        void renderFromBlack();
        void renderFromWhite();
        Chess game;
        GameMode gm;
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool mouseDown;
        std::pair<int, int> selectedPiece;
};