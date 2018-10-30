#include <iostream>
#include "tcpServer.hpp"

int main(){
    clients c = clients(6);
    tcpServer S = tcpServer(8080, c);
    S.listenNewClient();
    while(1){
        S.recieve();
    }

}
