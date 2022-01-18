#include "Draughts.hpp"
#include <iostream>
#include <unistd.h>

Player::Player(PlayerTurn turn, void *arg) : turn(turn) {
    connector = (struct cln *) arg;

}

Player &Player::create(PlayerTurn turn, void *arg) {
    struct cln *c = (struct cln *) arg;
    return *new Player(turn, c);
}

PlayerTurn Player::getTurn() {
    return turn;
}

Position::Col Player::getColFromChar(char col) {
    switch (col) {
        case 'A':
            return Position::Col::A;
        case 'B':
            return Position::Col::B;
        case 'C':
            return Position::Col::C;
        case 'D':
            return Position::Col::D;
        case 'E':
            return Position::Col::E;
        case 'F':
            return Position::Col::F;
        case 'G':
            return Position::Col::G;
        case 'H':
            return Position::Col::H;
        default:
            return Position::Col::NOT_VALID;
    }
}

Movement Player::getMovement(Board *board) {
//    std::cout << this->connector->cfd;
    this->send("your turn");
//    std::cout << *this << ", your turn:" ;
    int orRow, desRow;
    char orCol, desCol;
    std::string mov = this->readSocket();
//    std::cout << mov;
    orCol = mov[0];
    orRow = mov[1] - '0';
    desCol = mov[2];
    desRow = (int) mov[3] - '0';

    orRow--;
    desRow--;

    Position::Col orRealCol = getColFromChar(orCol);
    Position::Col desRealCol = getColFromChar(desCol);

    Position origin{orRow, orRealCol};
    Position destiny{desRow, desRealCol};

//    std::cout<<"COL+ROW_origin"<<origin.getCol()<<origin.getRow()<<"\n";
//    std::cout<<"COL+ROW_destiny"<<destiny.getCol()<<destiny.getRow()<<"\n";

    return Movement{origin, destiny, board, true};
}

std::ostream &operator<<(std::ostream &outputStream, const Player &p) {
    if (PLAYER1 == p.turn) {
        outputStream << "Player 1 (Black)";
    } else {
        outputStream << "Player 2 (White)";
    }

    return outputStream;
}

void Player::send(std::string str) {
    std::cout<<"SEND "<<this->connector->cfd<<": "<<str<<'\n';
    unsigned int number_of_zeros = MSG_SIZE - str.length(); // add 2 zeros
    str.insert(str.length(), number_of_zeros, 0);
    char *cstr = &str[0];
    long j = 0;
    while (j != MSG_SIZE) {
        long k = write(this->connector->cfd, cstr + j, MSG_SIZE - j);
        j += k;
    }
}ruch,
void Player::closeConnection(){
close(this->connector->cfd);

}

std::string Player::readSocket() {
    std::cout<<"READ "<<this->connector->cfd<<'\n';
    long j = 0;
    char str[MSG_SIZE];
    while (j != MSG_SIZE) {
        long k = read(this->connector->cfd, str + j, MSG_SIZE - j);
        j += k;
        std::cout<<k<<std::flush;
        sleep(1);
    }
    std::cout<<"came from socket: "<<str<<'\n';
    return str;
}
