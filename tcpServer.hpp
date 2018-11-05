#include <iostream>
//#include <sys/select.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
//#include <unistd.h>

class guiListener {
    public:
        virtual void startGame()=0;
//        virtual void oneMinuteLeft()=0;
//        virtual void stopGame()=0;

};

enum CMD : uint8_t{
    T_PLAYER_NAME, //0
	T_SELECTED_DMG, //1
	T_PLAYER_ID, //2
	T_START_GAME, //3
	T_KILLED_BY, //4
	T_KILL_CONFIRM, //5
	T_LAST_MINUTE, //6
	T_GAME_OVER, //7
	R_KILLED_BY, //8
	N_DEFAULT_EMPTY //9

};

class msg {
public:
	CMD command;
	std::string naam;
	uint8_t waarde = 0;

	msg() {
		command = CMD::N_DEFAULT_EMPTY;
		naam = "";
		waarde = 0;
	}

	msg(std::string basic_string) {
		int colon = basic_string.find(':');
		int comma = basic_string.find(',');
//		int semicolon = basic_string.find(';');

		command = static_cast<CMD>(basic_string[colon+1] - '0');
		if(command == T_PLAYER_NAME || command == T_KILLED_BY){
			naam = basic_string.substr(comma+1);
		}else{
			waarde = static_cast<uint8_t>(basic_string[comma+1] - '0');
		}
	}

	std::string serialize(){
		std::string out = "CMD:";
		out += ((uint8_t)command + '0');
		out += ",";
		if(command == T_PLAYER_NAME || command == T_KILLED_BY){
			out += naam;
		}else{
			out += (waarde + '0');
		}
		out += ';';
		return out;
	}


};

struct clients{
    int maxClients;
    int  client[6] = {0,0,0,0,0,0};
    char ip[6][16];
    char names[6][15];
    clients(int maxClients):
        maxClients(maxClients)
    {}
};

class tcpServer : public guiListener{
    private:
        int opt = 1;
        unsigned int port;
        int sock, newSock, clilen, n, sd, activity, nSock, max_sd, addrlen, valread;
        sockaddr_in serv_addr, cli_addr;
        char buffer[256];
        clients & c;
    public:
        tcpServer(unsigned int port, clients &c);
//        ~tcpServer();
        void listenNewClient();
        void recieve();
        void sendCli(int cli, const char* msg);
        void sendAll(const char* msg);
        int getClient(const char *ip);
        void startGame() override;
//        void oneMinuteLeft() override;
//        void stopGame() override;


};

