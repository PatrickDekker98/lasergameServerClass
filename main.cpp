#include <iostream>
#include "tcpServer.hpp"

int main(){
    clients c = clients(1);
    tcpServer S = tcpServer(8080, c);
    const char* msg = "hello there";
    while(c.client[c.maxClients - 1] == 0 || 1){
        S.listenNewClient();
//        S.recieve();
//        std::cout << c.maxClients;
        std::cout << "echo";
        S.sendCli(c.client[0], msg);
    }
/*    while(1){
        S.recieve();
    }*/ 

}
