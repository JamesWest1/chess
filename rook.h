#include <memory>
#include <vector>

#include "piece.h"
#include "empty.h"

#ifndef ROOK_H
#define ROOK_H
class Rook : public Piece {
    public:
        Rook() = delete;
        Rook(bool white)
        : Piece(white) {}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            if (!validCoord(endRow, endCol)) return false;          // rook ends up at invalid coord
            if (startRow != endRow && startCol != endCol) return false; // rook must only move along the column OR row
            if (startRow != endRow) {                               
                return canMoveAlongRow(startRow, endRow, startCol, game);//is there a piece blocking the rooks path?
            }
            else { 
                return canMoveAlongCol(startCol, endCol, startRow, game);//is there a piece blocking the rooks path?
            }
            return false;
        }
        std::string toString() {
            return std::string{"R"};
        }
    private:
        bool canMoveAlongRow(int startRow, int endRow, int col, const Chess &game) {
            int current = startRow;
            int operation = 1;
            if (startRow - endRow > 0) {
                current = startRow - 1;
                operation = -1; // endRow < startRow, must decrease to get closer
            }
            else if (startRow - endRow < 0) {
                current = startRow + 1; // endRow > startRow, must increase to get closer
            }
            else {
                return false; // cannot end and start on the same square
            }
            while (current != endRow) {
                if (!game.getSquare(current, col)->isEmpty()) {
                    return false;
                }
                current += operation;
            }
            if (game.getSquare(endRow, col)->isEmpty()) return true;
            return canCapture(endRow, col, game);
        }
        bool canMoveAlongCol(int startCol, int endCol, int row, const Chess &game) {
            int current = startCol;
            int operation = 1;
            if (startCol - endCol > 0) {
                current = startCol - 1;
                operation = -1; // endCol < startCol, must decrease to get closer
            }
            else if (startCol - endCol < 0) {
                current = startCol + 1; // endCol > startCol, must increase to get closer
            }
            else {
                return false; // cannot end and start on the same square
            }
            while (current != endCol) {
                if (!game.getSquare(row, current)->isEmpty()) {
                    return false;
                }
                current += operation;
            }
            if (game.getSquare(row, endCol)->isEmpty()) return true;
            return canCapture(row, endCol, game);
        }
};

#endif