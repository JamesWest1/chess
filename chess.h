#include <vector>
#include <stdlib.h>
#include <cstdint>
#include <memory>
#include <utility>

#include "square.h"

#ifndef CHESS_H_
#define CHESS_H_
#define BOARDSIZE 8
enum Colour {white, black};

class Chess {
    public:
        Chess();
        const std::vector<std::vector<std::unique_ptr<Square>>>*getBoard() const;
        Square *getSquare(int row, int col) const;
        std::string toString() const;
        void move(int startRow, int startCol, int endRow, int endCol); // moves a piece if the move is valid, nothing otherwise
        const std::pair<std::pair<int,int>, std::pair<int, int>> getPreviousMove() const;

        friend class Piece;
        friend class Pawn;
        friend class gui;
    private:
        void removeWhiteShort(); // remove castling privilege
        void removeWhiteLong();
        void removeBlackShort();
        void removeBlackLong();
        bool tryMove(int startRow, int startcol, int endRow, int endCol); // returns true if the piece was moved and false otherwise
        bool isInCheck(Colour colour) const;
        std::pair<int, int> kingCoord(Colour colour) const;
        bool isAttacked(int row, int col, Colour colour) const; // does a piece of 'Colour' colour attack the square (row, col)
        bool isKing(int row, int col) const;
        void validateCastlingStatus(int row, int col);                  // if row and col are the coordinates of a rooks starting square, update the castling rights associated with that rook
        void movePiece(int startRow, int startCol, int endRow, int endCol);
        bool isCastle(int startRow, int startCol, int endRow, int endCol) const;
        void castleRook(int kingEndRow, int kingEndCol);                                 // moves the rook into the correct position after castling
        std::vector<std::vector<std::unique_ptr<Square>>>board_;
        Colour playerColour_;
        std::pair<std::pair<int,int>, std::pair<int, int>> previousMove_; // first pair is the starting coord, second pair is the end coord for the previously played move
        bool whiteMove_;
        bool whiteShortCastle_;
        bool whiteLongCastle_;
        bool blackShortCastle_;
        bool blackLongCastle_;
};


#endif