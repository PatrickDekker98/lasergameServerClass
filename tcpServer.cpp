#include "tcpServer.hpp"
#include <iostream>
#include <vector>

extern std::vector<std::string> killedbylog;

tcpServer::tcpServer(std::string port, client clients[6], int maxClients):
	port(port),
//	clients(clients),
	maxClients(maxClients)
{
	if (maxClients > 6) {
		maxClients = 6;
	}
	for (int i = 0; i < maxClients; i++) {
		client c = client();
		clients[i] = c;
	}

	intResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (intResult != 0) {
		printf("WSAStartup failed with error: %d\n", intResult);
		return;
	}
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	intResult = getaddrinfo(NULL, port.c_str(), &hints, &result);

	if (intResult != 0) {
		printf("getaddrinfo failed with error: %d\n", intResult);
		WSACleanup();
		return;
	}

	serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	intResult = bind(serverSocket, result->ai_addr, (int)result->ai_addrlen);
	if (intResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(serverSocket);
		WSACleanup();
		exit(1);
	}
	unsigned long b = 1;
	ioctlsocket(serverSocket, FIONBIO, &b);
}

tcpServer::~tcpServer()
{
	for(int i = 0 ; i < maxClients; i++)
	{
		closesocket(clients[i].c);
	}
	WSACleanup();
}
int tcpServer::acceptcl(client *nc) {
	nc->i = sizeof(sockaddr);
	nc->c = accept(serverSocket, (sockaddr*)&nc->addr, &nc->i);
	std::string nm = "player";
	if (nc->c != 0 && nc->c != SOCKET_ERROR) {
		nc->con = true;
		FD_ZERO(&nc->set);
		FD_SET(nc->c, &nc->set);
		id++;
		nc->id = id;
		nm += (id + '0');
		nc->name = nm;
		nc->dmg = 20;
		return (true);
	}
	return (false);

}

void tcpServer::listenNewClients() {
	for (int i = 0; i < maxClients; i++) {
		if (!clients[i].con) {
			intResult = listen(serverSocket, SOMAXCONN);
			if (intResult == SOCKET_ERROR) {
				printf("listen failed with error: %d\n", WSAGetLastError());
				closesocket(serverSocket);
				WSACleanup();
				exit(1);
			}
			if (acceptcl(&clients[i])) {
				std::cout << "ok";
			}
		}
	}
}

void tcpServer::receiveCli() {
//	std::cout << "received";
	for (int i = 0; i < maxClients; i++) {
		memset(buffer, '0', 256);
		if (clients[i].con) {
//			std::cout << "received";
			intResult = recv(clients[i].c, buffer, bufferLen, 0);
			if (intResult > 0) {
//				std::cout << "received";
	//			std::cout << buffer;
				std::string s(buffer);
				std::string subs = s.substr(0, intResult);
				msg rc = msg(subs);
//				std::cout << rc.waarde;
				if ( rc.command == R_KILLED_BY) {
					msg s1;
					s1.command = T_KILLED_BY;
					for (int j = 0; j < maxClients; j++) {
						if (clients[j].id == rc.waarde){
							s1.naam = clients[j].name;
							clients[i].deaths += 1;
							sendCli(clients[i].c, s1);
							s1.command = T_KILL_CONFIRM;
							sendCli(clients[j].c, s1);
							clients[j].kills += 1;
                            killedbylog.push_back(clients[i].name + " got killed by " + clients[j].name + "!");
						}
					}
				}
			}
		}
	}
/*	intResult = recv(cl, buffer, bufferLen, 0);
	if (intResult > 0) {
		printf("Bytes received: %d\n", intResult);
	} else if (intResult == 0) {
		printf("Connection closing...\n");
	} else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		exit(1);
	}*/
}

void tcpServer::sendCli(SOCKET cli, msg m) {
	std::string msg = m.serialize();
	const char *cmsg = msg.c_str();
	//std::cout <<sizeof(msg);
	sendResult = send(cli, cmsg, strlen(cmsg), 0);
}

void tcpServer::sendAll(msg m) {
	for (int i = 0; i < maxClients; i++) {
		sendCli(clients[i].c, m);
	}
}

void tcpServer::startGame() {
	msg m;
	m.command = T_PLAYER_ID;
	for (int i = 0; i < maxClients; i++) {
		m.waarde = clients[i].id;
		sendCli(clients[i].c, m);
	}
	m.command = T_PLAYER_NAME;
	for (int i = 0; i < maxClients; i++) {
		m.naam = clients[i].name;
		sendCli(clients[i].c, m);
	}
	m.command = T_SELECTED_DMG;
	for (int i = 0; i < maxClients; i++) {
		m.waarde = clients[i].dmg;
		sendCli(clients[i].c, m);
	}
	m.command = T_START_GAME;
	sendAll(m);
}

void tcpServer::oneMinuteLeft() {
	msg m;
	m.command = T_LAST_MINUTE;
	sendAll(m);
}

void tcpServer::gameOver() {
	msg m;
	m.command = T_GAME_OVER;
	sendAll(m);
}
