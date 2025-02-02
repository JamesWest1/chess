#include "piece.h"


#ifndef BISHOP_H
#define BISHOP_H
class Bishop : public Piece {
public:
        Bishop() = delete;
        Bishop(bool white)
        : Piece(white),
        moveOffsets_{std::vector<std::vector<int>>{
                std::vector<int>{1, 1},
                std::vector<int>{1, -1},
                std::vector<int>{-1, 1},
                std::vector<int>{-1, -1}}} {}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            if (!validCoord(endRow, endCol)) return false;          // cant go out of bounds
            if (startRow == endRow && startCol == endCol) return false; // cant end on the same square
            for (const auto &offset : moveOffsets_) {
                int currentRow = startRow;
                int currentCol = startCol;
                while (validCoord(currentRow, currentCol) && currentRow != endRow && currentCol != endCol) {
                    if (currentRow != startRow && currentCol != startCol && !game.getSquare(currentRow, currentCol)->isEmpty()) break; // square in the path is occupied
                    currentRow += offset[0];
                    currentCol += offset[1];
                }
                if (currentRow == endRow && currentCol == endCol) {// if there was nothing between the bishops current square and final square, check final square can be moved to
                    if (game.getSquare(endRow, endCol)->isEmpty()) return true;
                    return canCapture(endRow, endCol, game);
                }
            }
            return false;
        }
        std::string toString() {
            return std::string{"B"};
        }
    private:
        bool white_;
        std::vector<std::vector<int>> moveOffsets_;
};

#endif