#include <iostream>
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

class tcpServer {
    private:
        unsigned int port;
        int sock, newSock, clilen, n;
        sockaddr_in serv_addr, cli_addr;
        char buffer[256];
    public:
        tcpServer(unsigned int port);
//        ~tcpServer();
        void listenNewClient();
        void recieve();
//        void startGame() override;
//        void oneMinuteLeft() override;
//        void stopGame() override;


};
