#include "square.h"
#include "piece.h"


class King : public Piece {
    public:
        King() = delete;
        King(bool isWhite) : Piece(isWhite) {}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            if (!validCoord(endRow, endCol)) return false;
            int rowDiff = startRow - endRow;
            int colDiff = startCol - endCol;
            if (abs(rowDiff) > 1) return false;
            if (abs(colDiff) > 1) return false;
            return game.getSquare(endRow, endCol)->isEmpty() || canCapture(endRow, endCol, game);
        }
        std::string toString() {
            return std::string{"K"};
        }
};