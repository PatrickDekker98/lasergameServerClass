#include <iostream>
#include "tcpServer.hpp"
#include <sys/socket.h>

tcpServer::tcpServer(unsigned int port):
    port(port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << " error creating socket\n";
       exit(1); 
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(sock, (struct sockaddr *) &serv_addr, 
                sizeof(serv_addr)) <0){
        std::cout << "no bind\n";
        exit(1);   
    }

}

void tcpServer::listenNewClient(){
    listen(sock,5);
    clilen = sizeof(cli_addr);
    newSock = accept(sock, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
    if(newSock < 0) {
        std::cout << "no accept\n";
    } else {
        std::cout << "new connection!\n";
    }
}

void tcpServer::recieve(){
    bzero(buffer, 256);
    n = read(newSock, buffer, 255);
    if (n < 0){
        std::cout << "no msg\n";
    } else {
        std::cout << n << "\n";
    }
}

