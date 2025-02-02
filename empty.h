#include "square.h"

#ifndef EMPTY_H
#define EMPTY_H

class Empty : public Square {
    public:
        bool tryMove(int startRow, int startCol, int endRow, int endCol, const Chess &game) override {
            return false;
        }
        bool move(int startRow, int startCol, int endRow, int endCol, Chess &game) override {
            return false;
        }
        bool isEmpty() override {
            return true;
        }
        std::string toString() override {
            return "-";
        }
};

#endif