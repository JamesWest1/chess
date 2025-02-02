
#ifndef SQUARE_H
#define SQUARE_H

#include <string>
// #include "chess.h"
class Chess;

class Square {
    public:
        bool validCoord(int row, int col) {
            return row < 8 && col < 8 && row >= 0 && col >= 0;
        }
        virtual bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) = 0;
        virtual bool move(int startRow, int startCol, int endRow, int endCol, Chess &game) = 0; // true if a piece was moved, false otherwise
        virtual bool isEmpty() = 0;
        virtual std::string toString() = 0;
};

#endif