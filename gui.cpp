#include "gui.h"


gui::gui() : game(Chess()), gm(GameMode::local), mouseDown(false){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "init failed\n";
        return;
    }
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWSIZE, WINDOWSIZE, 0);
    if (window == nullptr) {
        std::cout << "window creation failed\n";
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    return;
}

void gui::renderPath(std::string path, int x, int y, int size) {
    SDL_Surface* boardSurf = IMG_Load(path.c_str());
    if (boardSurf == nullptr) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, boardSurf);
    if (texture == nullptr) return;
    SDL_Rect destRect = {x, y, size, size};
    if (SDL_RenderCopy(renderer, texture, NULL, &destRect) < 0) std::cout << "didn't copy texture properly\n";
    SDL_FreeSurface(boardSurf);
    SDL_DestroyTexture(texture);
}


void gui::renderFrame() {
    SDL_RenderPresent(renderer);
}

void gui::shutDown() {
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    SDL_Quit();
}


void gui::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}


void gui::renderFromPlayersTurn() {
    if (game.whiteMove_) renderFromWhite();
    else renderFromBlack();
}

void gui::loadBoard() {
    if (gm == GameMode::local) {
        renderFromPlayersTurn();
    }
    else { // online game

    }
}


std::pair<int, int> gui::rowColToCoord(int row, int col) {
    int yPos = (WINDOWSIZE / BOARDSIZE) * row + (PIECESIZE / 17);
    int xPos = (WINDOWSIZE / BOARDSIZE) * col + (PIECESIZE / 12);
    return std::pair<int, int>{xPos, yPos};
}

void gui::renderFromWhite() {
    renderPath("../images/white-board.PNG", 0, 0, WINDOWSIZE);
    for (int row = 0; row < BOARDSIZE; row++) {
        for (int col = 0; col < BOARDSIZE; col++) {
            if (game.board_[row][col]->isEmpty()) continue;
            std::string imgPath = getImagePath(row, col);
            std::pair<int, int> pos = rowColToCoord(7 - row, col); // invert the board
            renderPath(imgPath, pos.first, pos.second, PIECESIZE);
        }
    }
}

void gui::renderFromBlack() {
    renderPath("../images/black-board.PNG", 0, 0, WINDOWSIZE);
    for (int row = 0; row < BOARDSIZE; row++) {
        for (int col = 0; col < BOARDSIZE; col++) {
            if (game.board_[row][col]->isEmpty()) continue;
            std::string imgPath = getImagePath(row, col);
            std::pair<int, int> pos = rowColToCoord(row, col);
            renderPath(imgPath, pos.first, pos.second, PIECESIZE);
        }
    }
}

std::string gui::getImagePath(int row, int col) {
    Piece* pc = dynamic_cast<Piece*>(game.getSquare(row,col));
    if (pc->isWhite()) {
        if (pc->toString() == "K") return std::string{"../images/white-king.png"};
        else if (pc->toString() == "Q") return std::string{"../images/white-queen.png"};
        else if (pc->toString() == "B") return std::string{"../images/white-bishop.png"};
        else if (pc->toString() == "H") return std::string{"../images/white-knight.png"};
        else if (pc->toString() == "R") return std::string{"../images/white-rook.png"};
        else return std::string{"../images/white-pawn.png"};
    }
    else {
        if (pc->toString() == "K") return std::string{"../images/black-king.png"};
        else if (pc->toString() == "Q") return std::string{"../images/black-queen.png"};
        else if (pc->toString() == "B") return std::string{"../images/black-bishop.png"};
        else if (pc->toString() == "H") return std::string{"../images/black-knight.png"};
        else if (pc->toString() == "R") return std::string{"../images/black-rook.png"};
        else return std::string{"../images/black-pawn.png"};
    }
}

std::pair<int, int> gui::correctPerspective(int row, int col) {
    if (gm == GameMode::local) {
        if (game.whiteMove_) return std::pair<int, int>{7-row, col};
        else return std::pair<int, int>{row, col};
    }
    else {
        return std::pair<int, int>{-1, -1};
    }
}

void gui::pressMouse(int x, int y) {
    int squareSize = WINDOWSIZE / BOARDSIZE;
    int col = x / squareSize;
    int row = y / squareSize;
    selectedPiece = correctPerspective(row, col);
    mouseDown = true;
}


void gui::unpressMouse(int x, int y) {
    int squareSize = WINDOWSIZE / BOARDSIZE;
    int col = x / squareSize;
    int row = y / squareSize;
    std::pair<int, int> dest = correctPerspective(row, col);
    game.move(selectedPiece.first, selectedPiece.second, dest.first, dest.second);
    mouseDown = false;
}