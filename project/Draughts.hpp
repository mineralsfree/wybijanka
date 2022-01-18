#include<vector>
#include<iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define NUM_COLS 8
#define NUM_ROWS 8
#define MAX_LENGTH_MAN 1
#define MSG_SIZE 100

inline int sign(int x) {
    return (x > 0) - (x < 0);
}
struct cln{
    int cfd;
    struct sockaddr_in caddr;
};
class Movement;
class Board;

class Position {

public:
    enum Col {
        A, B, C, D, E, F, G, H, NOT_VALID
    };

    Position(int row, Col col);
    int getRow() const;
    Col getCol() const;
    bool withinBounds();
    std::vector<Movement*> getPossibleEats(Board& board);

private:
    int row;
    Col col;

};

enum PlayerTurn {
    PLAYER1, PLAYER2, UNKNOWN
};

enum PieceType {
    EMPTY, MAN, KING
};

struct Piece {
    PieceType type = EMPTY;
    PlayerTurn owner = UNKNOWN;
};

class Movement {
    Position origin;
    Piece originalPiece;
    Piece destinyPiece;
    Position destiny;
    Position realDestiny;
    Movement* nextMove = nullptr;
    Board* board;

public:
    Movement(Position origin, Position destiny, Board* board, bool expand = false);
    ~Movement();
    Position getOrigin();
    Position getDestiny();
    bool isDiagonal();
    bool hasValidOrigin(PlayerTurn player);
    bool hasValidDestiny(PlayerTurn player);
    bool canEat();
    bool tryingToEat();
    bool isValid(PlayerTurn player);
    int getLength();
    int pathLength();
    bool clearPath();
    Position getNextPosition();
    Movement* findLongestPath();
    void execute();
    void undo();

    friend std::ostream& operator <<(std::ostream& outputStream, const Movement& m);
};



class Player {
protected:
    std::string readSocket();
    PlayerTurn turn;
    Position::Col getColFromChar(char col);
    Player(PlayerTurn turn, void *arg);

public:
    struct cln* connector;
    enum Type {
        HUMAN
    };
    static Player& create(PlayerTurn turn, void *arg);
    PlayerTurn getTurn();
    void send(std::string str);
    Movement getMovement(Board *board);
    friend std::ostream& operator <<(std::ostream& outputStream, const Player& p);

    void closeConnection();
};






class Board {
public:
    PlayerTurn getOwner(Position pos);
    bool playerCanMove(PlayerTurn player);
    bool pieceHasFreedom(Position pos);
    Piece getAt(Position pos);

    friend std::string getField(const Board& b);
    Piece& operator ()(int row, int col);

    friend std::ostream& operator <<(std::ostream& outputStream, const Board& b);

#define EMP {EMPTY, UNKNOWN}
#define MAN1 {MAN, PLAYER1}
#define MAN2 {MAN, PLAYER2}
#define KING1 {KING, PLAYER1}
#define KING2 {KING, PLAYER2}

private:
    Piece board[NUM_ROWS][NUM_COLS] =
            {
                    {EMP, MAN2, EMP, MAN2, EMP, MAN2, EMP, MAN2},
                    {MAN2, EMP, MAN2, EMP, MAN2, EMP, MAN2, EMP},
                    {EMP, MAN2, EMP, MAN2, EMP, MAN2, EMP, MAN2},
                    {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
                    {EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP},
                    {MAN1, EMP, MAN1, EMP, MAN1, EMP, MAN1, EMP},
                    {EMP, MAN1, EMP, MAN1, EMP, MAN1, EMP, MAN1},
                    {MAN1, EMP, MAN1, EMP, MAN1, EMP, MAN1, EMP}
            };

};

class Match {
private:
    Board board;
    Player& player1;
    Player& player2;

public:
    Match(Player& player1, Player& player2);
    void start();
};
