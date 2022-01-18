#include "Draughts.hpp"

Position::Position(int row, Col col): row(row), col(col) {

}

int Position::getRow() const {
    return row;
}

Position::Col Position::getCol() const {
    return col;
}

bool Position::withinBounds() {
    return	0 <= getRow() && getRow() < NUM_ROWS &&
              0 <= getCol() && getCol() < NUM_COLS;
}

std::vector<Movement*> Position::getPossibleEats(Board& board) {
    std::vector<Movement*> moves;
    short offsets[] = {-1, 1};

    for(short rowOffset: offsets) {
        for (short colOffset: offsets) {
            int length = 1;
            while (true) {
                Position newDestiny {row + length*rowOffset, (Col) (col + length*colOffset)};
                if (!newDestiny.withinBounds()) break;
                if (MAN == board.getAt(*this).type && length > 1) break;
                if (board.getAt(newDestiny).owner == board.getAt(*this).owner) break;
                if ( 	board.getAt(newDestiny).type != EMPTY &&
                        board.getAt(newDestiny).owner != board.getAt(*this).owner) {
                    Movement* eatMov = new Movement(*this, newDestiny, &board);
                    if (eatMov->canEat()) {
                        moves.push_back(eatMov);
                    }
                    break;
                }

                length++;
            }
        }
    }

    return moves;
}