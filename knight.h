#include "piece.h"
#include <vector>
#include <memory>

#ifndef KNIGHT_H
#define KNIGHT_H

class Knight : public Piece {
    public:
        Knight() = delete;
        Knight(bool isWhite)
        : Piece(isWhite),
        moveOffsets_{std::vector<std::vector<int>>{
                std::vector<int>{2, 1},
                std::vector<int>{2, -1},
                std::vector<int>{-2, 1},
                std::vector<int>{-2, -1},
                std::vector<int>{1, 2},
                std::vector<int>{-1, 2},
                std::vector<int>{1, -2},
                std::vector<int>{-1, -2}
                }} {}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            if (!validCoord(endRow, endCol)) return false;
            if (!game.getSquare(endRow, endCol)->isEmpty()) {
                if (!canCapture(endRow, endCol, game)) return false; // can't capture the piece, can't move there
            }
            for (const auto &offset : moveOffsets_) {
                if ((startRow + offset[0] == endRow) && (startCol + offset[1] == endCol)) return true; // if the endRow and endCol are 1 of the 8 coord offsets, its a valid move
            }
            return false;
        }
        std::string toString() {
            return std::string{"H"};
        }
    private:
        std::vector<std::vector<int>> moveOffsets_;
};
#endif