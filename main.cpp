#include <iostream>
#include "tcpServer.hpp"

int main(){
    tcpServer S = tcpServer(8080);
    S.listenNewClient();
    S.recieve();


}
