#include "piece.h"
#include "bishop.h"
#include "rook.h"

class Queen : public Piece {
    public:
        Queen() = delete;
        Queen(bool isWhite) : Piece(isWhite) {}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            Bishop b = Bishop(isWhite());
            Rook r = Rook(isWhite());
            return b.tryMove(startRow, startCol, endRow, endCol, game)
                || r.tryMove(startRow, startCol, endRow, endCol, game);
        }
        std::string toString() {
            return std::string{"Q"};
        }
};