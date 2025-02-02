#include "square.h"
#include "piece.h"
#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "rook.h"
#include "bishop.h"
#include "queen.h"


#include "chess.h"
#include <iostream>

Chess::Chess() : board_(8), previousMove_{{-1,-1},{-1,-1}}, whiteMove_(true),
    whiteShortCastle_(true), whiteLongCastle_(true), blackShortCastle_(true), blackLongCastle_(true) {
    for (int i = 2; i < 6; i++) {
        for (int y = 0; y < BOARDSIZE; y++) {
            auto value = std::make_unique<Empty>(Empty());
            board_[i].push_back(std::move(value));
        }
    }
    for (int i = 0; i < BOARDSIZE; i++) {
        board_[1].push_back(std::make_unique<Pawn>(Pawn(true)));
        board_[6].push_back(std::make_unique<Pawn>(Pawn(false)));
    }
    board_[0].push_back(std::make_unique<Rook>(Rook(true)));
    board_[7].push_back(std::make_unique<Rook>(Rook(false)));
    board_[0].push_back(std::make_unique<Knight>(Knight(true)));
    board_[7].push_back(std::make_unique<Knight>(Knight(false)));
    board_[0].push_back(std::make_unique<Bishop>(Bishop(true)));
    board_[7].push_back(std::make_unique<Bishop>(Bishop(false)));
    board_[0].push_back(std::make_unique<Queen>(Queen(true)));
    board_[7].push_back(std::make_unique<Queen>(Queen(false)));
    board_[0].push_back(std::make_unique<King>(King(true)));
    board_[7].push_back(std::make_unique<King>(King(false)));
    board_[0].push_back(std::make_unique<Bishop>(Bishop(true)));
    board_[7].push_back(std::make_unique<Bishop>(Bishop(false)));
    board_[0].push_back(std::make_unique<Knight>(Knight(true)));
    board_[7].push_back(std::make_unique<Knight>(Knight(false)));
    board_[0].push_back(std::make_unique<Rook>(Rook(true)));
    board_[7].push_back(std::make_unique<Rook>(Rook(false)));
}
const std::vector<std::vector<std::unique_ptr<Square>>>* Chess::getBoard() const {
    return &board_;
}
Square* Chess::getSquare(int row, int col) const {
    return board_[row][col].get();
}

std::string Chess::toString() const {
    std::string result = "";
    for (int i = 7; i >= 0; i--) {
        for (int y = 0; y < BOARDSIZE; y++) {
            result += board_[i][y]->toString();
        }
        result += "\n";
    }
    return result;
}

void Chess::removeWhiteShort() {
    whiteShortCastle_ = false;
}
void Chess::removeWhiteLong() {
    whiteLongCastle_ = false;
}
void Chess::removeBlackShort() {
    whiteShortCastle_ = false;
}
void Chess::removeBlackLong() {
    blackLongCastle_ = false;
}

bool Chess::tryMove(int startRow, int startCol, int endRow, int endCol) {
    if (board_[startRow][startCol]->isEmpty()) return false;
    Piece* piece = dynamic_cast<Piece*>(getSquare(startRow,startCol));
    if (piece->isWhite() != whiteMove_) return false;
    if (isCastle(startRow, startCol, endRow, endCol)) {
        movePiece(startRow, startCol, endRow, endCol);
        castleRook(endRow, endCol);
        return true;
    }
    if (board_[startRow][startCol]->move(startRow, startCol, endRow, endCol, *this)) {
        if (whiteMove_ && endRow == 7 && board_[endRow][endCol]->toString() == "P") board_[endRow][endCol] = std::make_unique<Queen>(Queen(true));
        if (!whiteMove_ && endRow == 0 && board_[endRow][endCol]->toString() == "P") board_[endRow][endCol] = std::make_unique<Queen>(Queen(false));
        return true;
    }
    return false;
}

bool Chess::isCastle(int startRow, int startCol, int endRow, int endCol) const {
    int colDifference = endCol - startCol;
    if (!isKing(startRow, startCol)) return false;
    if (startRow != endRow) return false;
    if (abs(colDifference) != 2) return false;                                          // castling moves the king 2 squares
    if (whiteMove_ && startRow != 0) return false;
    if (!whiteMove_ && startRow != 7) return false;
    if (whiteMove_ && isAttacked(startRow, startCol, Colour::black)) return false; // can't castle whilst already in check
    if (!whiteMove_ && isAttacked(startRow, startCol, Colour::white)) return false;
    if (colDifference < 0) { // castle long
        if (whiteMove_ && !whiteLongCastle_) return false;
        if (!whiteMove_ && !blackLongCastle_) return false;
        if (!board_[startRow][startCol - 1]->isEmpty()) return false;               // check you aren't castling through a piece
        if (!board_[startRow][startCol - 2]->isEmpty()) return false;
        if (whiteMove_ && isAttacked(startRow, startCol-1, Colour::black)) return false; // check that they aren't castling through check
        if (whiteMove_ && isAttacked(startRow, startCol-2, Colour::black)) return false;
        if (!whiteMove_ && isAttacked(startRow, startCol-1, Colour::white)) return false;
        if (!whiteMove_ && isAttacked(startRow, startCol-2, Colour::white)) return false;
        return true;
    }
    else { // castle short
        if (whiteMove_ && !whiteShortCastle_) return false;
        if (!whiteMove_ && !blackShortCastle_) return false;
        if (!board_[startRow][startCol + 1]->isEmpty()) return false;
        if (!board_[startRow][startCol + 1]->isEmpty()) return false;
        if (whiteMove_ && isAttacked(startRow, startCol+1, Colour::black)) return false;
        if (whiteMove_ && isAttacked(startRow, startCol+2, Colour::black)) return false;
        if (!whiteMove_ && isAttacked(startRow, startCol+1, Colour::white)) return false;
        if (!whiteMove_ && isAttacked(startRow, startCol+2, Colour::white)) return false;
        return true;
    }
}

void Chess::movePiece(int startRow, int startCol, int endRow, int endCol) {
    board_[endRow][endCol] = std::move(board_[startRow][startCol]);
    board_[startRow][startCol] = std::make_unique<Empty>(Empty());
}

bool Chess::isInCheck(Colour colour) const {
    std::pair<int, int> kingPos = kingCoord(colour);
    Colour attackerColour = (colour == Colour::white ? Colour::black : Colour::white);
    return isAttacked(kingPos.first, kingPos.second, attackerColour);
}

std::pair<int, int> Chess::kingCoord(Colour colour) const {
    int kRow, kCol;
    for (int row = 0; row < BOARDSIZE; row++) {
        for (int col = 0; col < BOARDSIZE; col++) {
            auto *square = this->getSquare(row, col);
            if (square->isEmpty()) continue;
            Piece *piece = dynamic_cast<Piece*>(square);
            if (colour == Colour::white && piece->isWhite() && isKing(row, col)) return std::pair<int, int>{row, col};
            else if (colour == Colour::black && !piece->isWhite() && isKing(row, col)) return std::pair<int, int>{row, col};
        }
    }
    return std::pair<int, int>{-1, -1}; //something went horribly wrong
}

bool Chess::isAttacked(int row, int col, Colour colour) const { // is coord: (row, col) attacked by colour
    for (int i = 0; i < BOARDSIZE; i++) {
        for (int y = 0; y < BOARDSIZE; y++) {
            if (board_[i][y]->isEmpty()) continue;
            Piece *piece = dynamic_cast<Piece*>(this->getSquare(i, y));
            if (colour == Colour::white && piece->isWhite() && piece->tryMove(i, y, row, col, *this)) return true;
            else if (colour == Colour::black && !piece->isWhite() && piece->tryMove(i, y, row, col, *this)) return true;
        }
    }
    return false;
}

void Chess::move(int startRow, int startCol, int endRow, int endCol) {
    if (tryMove(startRow, startCol, endRow, endCol)) {
        bool kingMove = isKing(endRow, endCol);
        if (kingMove && whiteMove_) {
            removeWhiteLong();
            removeWhiteShort();
        }
        if (kingMove && !whiteMove_) {
            removeBlackLong();
            removeBlackShort();
        }
        // check if any piece moved from or to one of the rooks starting squares. If they did
        // remove castling rights for that rook
        validateCastlingStatus(startRow, startCol);
        validateCastlingStatus(endRow, endCol);

        previousMove_ = {{startRow, startCol}, {endRow, endCol}};
        whiteMove_ = !whiteMove_; // change turns
    }
}

void Chess::castleRook(int kingEndRow, int kingEndCol) {
    if (kingEndCol == 2) {                      // long castle
        if (whiteMove_) movePiece(0, 0, 0, 3); // move A1 rook to D1
        else movePiece(7, 0, 7, 3);             // move A8 rook to D8
    }
    else {                                      // short castle
        if (whiteMove_) movePiece(0, 7, 0, 5); // move H1 rook to F1
        else movePiece(7, 7, 7, 5);             // move H8 rook to F8
    }
}

bool Chess::isKing(int row, int col) const {
    return board_[row][col]->toString() == "K" || board_[row][col]->toString() == "k";
}

void Chess::validateCastlingStatus(int row, int col) {
    if (row == 0 && col == 0) removeWhiteLong();
    else if (row == 7 && col == 0) removeBlackLong();
    else if (row == 0 && col == 7) removeWhiteShort();
    else if (row == 7 && col == 7) removeBlackShort();
}

const std::pair<std::pair<int,int>, std::pair<int, int>> Chess::getPreviousMove() const {
    return previousMove_;
}