#include <cstdio>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#include <pthread.h>
#include "Draughts.hpp"


Player &player1 = Player::create(UNKNOWN, nullptr);
Player &player2 = Player::create(UNKNOWN, nullptr);

void *player1ThreadWork(void *arg) {
    struct cln *c = (struct cln *) arg;
    std::cout<<"THREAD 1";
    printf("new connections  : %s\n", inet_ntoa((struct in_addr) c->caddr.sin_addr));
    player1 = Player::create(PLAYER1, c);
    player1.send("color black");
    return nullptr;
}

void *player2ThreadWork(void *arg) {
    struct cln *c = (struct cln *) arg;
    std::cout<<"THREAD 2"<<std::flush;
    printf("new connections  : %s\n", inet_ntoa((struct in_addr) c->caddr.sin_addr));
    player2 = Player::create(PLAYER2, c);
    player2.send("color white");
    Match match{player1, player2};
    match.start();
    return nullptr;
}


int connected = 0;

int main() {
    pthread_t tid;
    socklen_t slt;
    int sfd = socket(PF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sfd, (struct sockaddr *) &addr, sizeof(addr));
    listen(sfd, 1);

    while (1) {
        struct cln *c = static_cast<cln *>(malloc(sizeof(struct cln)));
        slt = sizeof(c->caddr);
        connected++;
        c->cfd = accept(sfd, (struct sockaddr *) &c->caddr, &slt);
        if (connected % 2 == 1) {
            pthread_create(&tid, NULL, player1ThreadWork, c);
        } else {
            pthread_create(&tid, NULL, player2ThreadWork, c);
        }
        pthread_detach(tid);
    }
    return 0;
}