#include "Draughts.hpp"
#include <cstdlib>
#include <iostream>

Match::Match(Player &player1, Player &player2) : player1(player1), player2(player2) {};

void Match::start() {
    PlayerTurn turn = PLAYER1;
    Player *currentPlayer;

    while (true) {
        if (!board.playerCanMove(PLAYER1) && !board.playerCanMove(PLAYER2)) {
            player1.send("won! Draw");
            player2.send("won! Draw");
            player1.closeConnection();
            player2.closeConnection();
            break;
        } else if (!board.playerCanMove(PLAYER1)) {
            player1.send("won! white");
            player2.send("won! white");
            player1.closeConnection();
            player2.closeConnection();
            break;
        } else if (!board.playerCanMove(PLAYER2)) {
            player1.send("won! black");
            player2.send("won! black");
            player1.closeConnection();
            player2.closeConnection();
            break;
        }

        switch (turn) {
            case PLAYER1:
                currentPlayer = &player1;
                break;
            case PLAYER2:
                currentPlayer = &player2;
                break;
            case UNKNOWN:
                break;
        }

        std::string f = getField(board);
        player1.send(f);
        player2.send(f);
        Movement nextMove = currentPlayer->getMovement(&board);
        std::cout<<"ok";

        if (!nextMove.isValid(currentPlayer->getTurn())) {
            currentPlayer->send("Not a valid movement, please retry");
            continue;
        }
        nextMove.execute();
        if (turn == PLAYER1) turn = PLAYER2;
        else turn = PLAYER1;

    }

}
