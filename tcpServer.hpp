#include <iostream>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

class guiListener {
    public:
        virtual void startGame()=0;
        virtual void oneMinuteLeft()=0;
        virtual void stopGame()=0;

};

struct clients{
    int maxClients;
    int client[];
    clients(int maxClients):
        maxClients(maxClients)
    {}
};

class tcpServer {
    private:
        int opt = 1;
        unsigned int port;
        int sock, newSock, clilen, n, sd, activity, nSock, max_sd, addrlen;
        sockaddr_in serv_addr, cli_addr;
        char buffer[256];
        struct clients c;
    public:
        tcpServer(unsigned int port, clients c);
//        ~tcpServer();
        void listenNewClient();
        void recieve();
//        void startGame() override;
//        void oneMinuteLeft() override;
//        void stopGame() override;


};
