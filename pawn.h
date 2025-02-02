#ifndef PAWN_H
#define PAWN_H

#include "square.h"
#include "piece.h"
#include "chess.h"
#include "empty.h"


#include <iostream>

class Pawn : public Piece {
    public:
        Pawn() = delete;
        Pawn(bool isWhite) 
        : Piece(isWhite){}
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            int rowDifference = endRow - startRow;
            int colDifference = endCol - startCol;
            if (abs(rowDifference) > 2) return false;
            if (rowDifference == 0) return false;
            if (!validCoord(endRow, endCol)) return false;
            if (startCol != endCol) {                       // capturing a piece
                return isCapture(startRow, startCol, endRow, endCol, game);
            }
            int col = startCol; // start and end col are the same (not capturing a piece)
            if (isWhite()) {
                if (rowDifference < 0) return false; // move up the board
                if (!game.getSquare(startRow + 1, col)->isEmpty()) return false;
                if (rowDifference == 2 && !game.getSquare(startRow + 2, col)->isEmpty()) return false;
                return true;
            }
            else {
                if (rowDifference > 0) return false; // move down the board
                if (!game.getSquare(startRow - 1, col)->isEmpty()) return false;
                if (rowDifference == -2 && !game.getSquare(startRow - 2, col)->isEmpty()) return false;
                return true;
            }
        }
        bool move(int startRow, int startCol, int endRow, int endCol, Chess &game) override {
            if (tryMove(startRow, startCol, endRow, endCol, game)) {
                bool enPassant = startCol != endCol && game.getSquare(endRow, endCol)->isEmpty();
                if (enPassant) {
                    std::unique_ptr<Square> val = std::move(game.board_[startRow][endCol]); // r emove adjacent piece
                    game.board_[startRow][endCol] = std::make_unique<Empty>(Empty());
                    if (resultsInCheck(startRow, startCol, endRow, endCol, game)) {
                        game.board_[startRow][endCol] = std::move(val);
                        return false;
                    }
                    return true;
                }
                return !resultsInCheck(startRow, startCol, endRow, endCol, game);
            }
            return false;
        }
        bool isCapture(int startRow, int startCol, int endRow, int endCol, const Chess &game) {
            int colDifference = endCol - startCol;
            int rowDifference = endRow - startRow;
            if (abs(colDifference) != 1) return false;
            if (game.getSquare(endRow, endCol)->isEmpty()) { // check for en passant
                const auto lastMove = game.getPreviousMove();
                const auto lastMoveDest = lastMove.second;
                if (!validCoord(lastMoveDest.first, lastMoveDest.second)) return false; // destination of the last move is invalid (there is no last move)
                bool endOnSameColumn = (endCol == lastMoveDest.second);
                bool startOnSameRow = (startRow == lastMoveDest.first);
                bool lastMoveWasTwoSquares = (abs(lastMove.first.first - lastMoveDest.first) == 2);
                bool lastMoveWasAPawn = (game.getSquare(lastMoveDest.first, lastMoveDest.second)->toString() == "P");
                return endOnSameColumn && startOnSameRow && lastMoveWasTwoSquares && lastMoveWasAPawn;
            }
            if (isWhite()) {
                bool moveForwardOne = (rowDifference == 1);
                if (!moveForwardOne) return false;
                return canCapture(endRow, endCol, game);
            }
            else {
                bool moveBackwardOne = (rowDifference == -1);
                if (!moveBackwardOne) return false;
                return canCapture(endRow, endCol, game);
            }
        }
        std::string toString() {
            return std::string{"P"};
        }
};


#endif