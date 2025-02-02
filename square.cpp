#include "square.h"


bool Square::validCoord(int row, int col) {
    return row >= 8 || col >= 8 || row <0 || col < 0;
}