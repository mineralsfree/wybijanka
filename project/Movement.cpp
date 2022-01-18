#include "Draughts.hpp"
#include <cmath>
#include <iostream>

Movement::Movement(Position origin, Position destiny, Board* board, bool expand): origin(origin), destiny(destiny), board(board), realDestiny(destiny.getRow(), destiny.getCol()) {
    originalPiece = (*board)(origin.getRow(), origin.getCol());
    destinyPiece = (*board)(destiny.getRow(), destiny.getCol());

    if (tryingToEat() && canEat()) {
        realDestiny = getNextPosition();
        if (expand) {
            Movement* longest = findLongestPath();
            if (nullptr != longest) {
                nextMove = longest->nextMove;
            }
        }
    }

}

Movement::~Movement() {
    if (nullptr != nextMove) {
        delete nextMove;
    }
}


Position Movement::getOrigin() {
    return origin;
}

Position Movement::getDestiny() {
    return destiny;
}

bool Movement::isDiagonal() {
    return std::abs(origin.getRow() - destiny.getRow()) ==
           std::abs(origin.getCol() - destiny.getCol());
}

bool Movement::hasValidOrigin(PlayerTurn player) {
    Position origin = getOrigin();
    return origin.withinBounds() &&
           board->getOwner(origin) == player;
}

bool Movement::hasValidDestiny(PlayerTurn player) {
    Position destiny = getDestiny();

    return destiny.withinBounds() &&
           (board->getOwner(destiny) != player ||
            board->getOwner(destiny) == UNKNOWN);
}

bool Movement::canEat() {
    Position nextPos = getNextPosition();
    // std::cout<<"nextPos"<<nextPos.getRow()<<" "<<nextPos.getCol()<<"\n";
    return 	nextPos.withinBounds() &&
              (*board)(origin.getRow(), origin.getCol()).owner != (*board)(destiny.getRow(), destiny.getCol()).owner &&
              EMPTY == (*board)(nextPos.getRow(), nextPos.getCol()).type;
}

bool Movement::tryingToEat() {


    return	(PLAYER1 == (*board)(origin.getRow(), origin.getCol()).owner &&
               PLAYER2 == (*board)(destiny.getRow(), destiny.getCol()).owner) ||
              (PLAYER2 == (*board)(origin.getRow(), origin.getCol()).owner &&
               PLAYER1 == (*board)(destiny.getRow(), destiny.getCol()).owner);
}


int Movement::getLength() {
    return std::abs(origin.getRow() - destiny.getRow());
}

int Movement::pathLength() {
    int depth = 0;
    Movement* next = this;
    while (nullptr != next->nextMove) {
        next = next->nextMove;
        depth++;
    }

    return depth;
}

Position Movement::getNextPosition() {
    int 	rowDir = -sign(origin.getRow() - destiny.getRow()),
            colDir = -sign(origin.getCol() - destiny.getCol());

    int 	nextRow = destiny.getRow() + rowDir,
            nextCol = destiny.getCol() + colDir;

    return Position{nextRow, (Position::Col) nextCol};
}

bool Movement::isValid(PlayerTurn player) {

    Position origin = getOrigin();
    Position destiny = getDestiny();
//    std::cout<<"hasValidOrigin(player)"<<hasValidOrigin(player)<<"\n";
//    std::cout<<"hasValidDestiny(player)"<<hasValidDestiny(player)<<"\n";
//    std::cout<<"isDiagonal"<<isDiagonal()<<"\n";

    //    std::cout<<"\n\n\n" <<origin.getRow() << origin.getCol() << destiny.getRow()<< destiny.getCol() <<"\n";
    if (	!hasValidOrigin(player) ||
            !hasValidDestiny(player) ||
            !isDiagonal()) {
        return false;
    }
//    std::cout<<"tryingToEat()"<<tryingToEat()<<"\n";
//    std::cout<<"canEat"<<canEat()<<"\n";


    if (tryingToEat() && !canEat()) {
        return false;
    }
    if ((*board)(origin.getRow(), origin.getCol()).type == MAN) {
        return getLength() == MAX_LENGTH_MAN;
    } else {
        return clearPath();
    }

}

bool Movement::clearPath() {
    int 	rowDir = -sign(origin.getRow() - destiny.getRow()),
            colDir = -sign(origin.getCol() - destiny.getCol());

    int 	nextRow = origin.getRow(),
            nextCol = origin.getCol();

    while (true) {
        nextRow += rowDir;
        nextCol += colDir;
        if(nextRow == destiny.getRow() && nextCol == destiny.getCol()) break;
        if ((*board)(nextRow, nextCol).type != EMPTY) return false;
    }

    return true;
}

void Movement::execute() {

    (*board)(origin.getRow(), origin.getCol()) = EMP;
    (*board)(destiny.getRow(), destiny.getCol()) = EMP;

    if (realDestiny.getRow() == 0 && originalPiece.owner == PLAYER1) {
        (*board)(realDestiny.getRow(), realDestiny.getCol()) = KING1;
    } else if (realDestiny.getRow() == (NUM_ROWS - 1) && originalPiece.owner == PLAYER2) {
        (*board)(realDestiny.getRow(), realDestiny.getCol()) = KING2;
    } else {
        (*board)(realDestiny.getRow(), realDestiny.getCol()) = originalPiece;
    }

    if (nullptr != nextMove) {
        nextMove->execute();
    }

}

void Movement::undo() {

    if (nullptr != nextMove) {
        nextMove->undo();
    }

    (*board)(realDestiny.getRow(), realDestiny.getCol()) = EMP;
    (*board)(destiny.getRow(), destiny.getCol()) = destinyPiece;
    (*board)(origin.getRow(), origin.getCol()) = originalPiece;

}

Movement* Movement::findLongestPath() {
    execute();
    std::vector<Movement*> paths = realDestiny.getPossibleEats(*board);

    for(Movement *path: paths) {
        Movement* newPath = path->findLongestPath();
        int newLength = nullptr == newPath ? 1 : newPath->pathLength();
        if (	nextMove == nullptr ||
                newLength >= pathLength()) {
            if (nullptr != nextMove) delete nextMove;
            nextMove = path;
        } else {
            delete path;
        }
    }

    undo();
    return paths.empty() ? nullptr : this;

}

std::ostream& operator <<(std::ostream& outputStream, const Movement& m) {
    outputStream << (char) (m.origin.getCol() + 'A') << m.origin.getRow()+1 << " -> " << (char) (m.destiny.getCol() + 'A') << m.destiny.getRow()+1 << " ";

    if (nullptr != m.nextMove)  {
        outputStream << " >> "<< *m.nextMove;
    } else {
        outputStream << " >> END";
    }

    return outputStream;
}
