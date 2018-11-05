#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512

enum CMD : uint8_t {
	T_PLAYER_NAME,
	T_SELECTED_DMG,
	T_PLAYER_ID,
	T_START_GAME,
	T_KILLED_BY,
	T_KILL_CONFIRM,
	T_LAST_MINUTE,
	T_GAME_OVER,
	R_KILLED_BY,
	N_DEFAULT_EMPTY
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
		int semicolon = basic_string.find(';');

		command = static_cast<CMD>(basic_string[colon + 1] - '0');
		if (command == T_PLAYER_NAME || command == T_KILLED_BY) {
			naam = basic_string.substr(comma + 1, semicolon - 1);
		}
		else {
			waarde = static_cast<uint8_t>(basic_string[comma + 1] - '0');
		}
	}

	std::string serialize() {
		std::string out = "CMD:";
		out += ((uint8_t)command + '0');
		out += ",";
		if (command == T_PLAYER_NAME || command == T_KILLED_BY) {
			out += naam;
		}
		else {
			out += (waarde + '0');
			out += ';';
		}
		return out;
	}


};

class client {
public:
	bool con = 0;
	int id;
	sockaddr_in addr;
	SOCKET c;
	fd_set set;
	std::string name;
	uint8_t dmg;
	int i;
	int kills = 0;
};

class tcpServer {
private:
	int id = 0;
	int maxClients;
	WSADATA wsaData;
	std::string port;
	int intResult;
	int sendResult;

	SOCKET serverSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	const int bufferLen = 512;
	char buffer[512];

public:
	client clients[6];
	bool start = false;
	tcpServer(std::string port, client clients[6], int maxClients);
	~tcpServer();
	int acceptcl(client *c);
	void listenNewClients();
	void recieveCli();
	void sendCli(SOCKET cli, msg m);
	void sendAll( msg m);
	void startGame();
	void oneMinuteLeft();
	void gameOver();
};

