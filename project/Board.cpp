#include "Draughts.hpp"
#include <iostream>
#include <sstream>

PlayerTurn Board::getOwner(Position pos) {
    return board[pos.getRow()][pos.getCol()].owner;

}



bool Board::playerCanMove(PlayerTurn player) {
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            if (	player == board[i][j].owner &&
                    pieceHasFreedom(Position{i, (Position::Col)j})) {
                return true;
            }
        }
    }

    return false;
}

bool Board::pieceHasFreedom(Position position) {
    int movements[2] {-1, 1};

    for (int rowMov: movements) {
        for (int colMov: movements) {
            int 	newRow = position.getRow() + rowMov,
                    newCol = position.getCol() + colMov;
            Position adyPosition {newRow, (Position::Col) newCol};

            if (!adyPosition.withinBounds()) continue;

            if (	EMPTY == board[newRow][newCol].type ||
                    Movement{position, adyPosition, this}.canEat()) {
                return true;
            }
        }
    }

    return false;
}


Piece Board::getAt(Position pos) {
    return board[pos.getRow()][pos.getCol()];
}

Piece& Board::operator ()(int row, int col) {
    return board[row][col];
}

std::string getField(const Board& board){
    std::string ss = "f";
    for(int i=0; i<NUM_ROWS; i++) {
        for (int j=0; j<NUM_COLS; j++) {
            switch(board.board[i][j].type) {
                case MAN:
                    if (board.board[i][j].owner == PLAYER1) ss+='b';
                    else ss+='w';
                    break;
                case KING:
                    if (board.board[i][j].owner == PLAYER1) ss+='B';
                    else ss+='W';
                    break;
                default:
                    ss+='-';
                    break;
            }
        }
        ss+='\n';
    }
    return ss;
}
std::ostream& operator <<(std::ostream& outputStream, const Board& board) {

    outputStream << "A \tB \tC \tD \tE \tF \tG \tH" << std::endl;
    std::stringstream ss;

    for(int i=0; i<NUM_ROWS; i++) {
        for (int j=0; j<NUM_COLS; j++) {
            switch(board.board[i][j].type) {
                case MAN:
                    if (board.board[i][j].owner == PLAYER1) outputStream << 'b' << "\t";
                    else outputStream << 'w' << "\t";
                    break;
                case KING:
                    if (board.board[i][j].owner == PLAYER1) outputStream << 'B' << "\t";
                    else outputStream << 'W' << "\t";
                    break;
                default:
                    outputStream << '-' << "\t";
                    break;
            }
        }
        outputStream << i+1 << std::endl;
    }
    ss << outputStream.rdbuf();
    std::string myString = ss.str();


    return outputStream;
}