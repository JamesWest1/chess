#include "square.h"
#include "chess.h"
#include "empty.h"

#include <iostream>


#ifndef PIECE_H
#define PIECE_H
class Piece : public Square {
    public:
        Piece() = delete;
        Piece(bool isWhite) : white_(isWhite) {}
        bool isWhite()  {
            return white_;
        }
        bool isEmpty() override {
            return false;
        }
        bool canCapture(int row, int col, const Chess &game) {
            auto *val = game.getSquare(row, col);
            Piece* pc = dynamic_cast<Piece*>(val);
            return pc->isWhite() == !isWhite();
        }
        bool move(int startRow, int startCol, int endRow, int endCol, Chess &game) {
            if (tryMove(startRow, startCol, endRow, endCol, game)) {
                return !resultsInCheck(startRow, startCol, endRow, endCol, game);
            }
            return false;
        }
        bool resultsInCheck(int startRow, int startCol, int endRow, int endCol, Chess &game) {
            std::unique_ptr<Square> tmp = std::move(game.board_[endRow][endCol]);
            game.board_[endRow][endCol] = std::move(game.board_[startRow][startCol]);
            game.board_[startRow][startCol] = std::make_unique<Empty>(Empty());
            if (game.isInCheck(game.whiteMove_ ? Colour::white : Colour::black)) { // this move results in the players king being in check. Undo the move
                game.board_[startRow][startCol] = std::move(game.board_[endRow][endCol]); // put the pieces back
                game.board_[endRow][endCol] = std::move(tmp);
                return true;
            }
            return false;
        }
    private:
        bool white_;
};

#endif