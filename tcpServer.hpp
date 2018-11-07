// ==========================================================================
//
// File      : tcpServer.hpp
// Part of   : LaserGameProject 
// Copyright : Patrick.p.Dekker@student.hu.nl 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

//this file contains Doxygen lines
/// @file

/// \mainpage tcpServer library
/// 
/// \author Patrick Dekker (patrick.p.dekker@student.hu.nl)
///
/// \copyright boost license
///
/// this library implements a tcp socket server for our lasergame project at school
///

#ifndef _TCPSERVER_HPP
#define _TCPSERVER_HPP

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
	T_PLAYER_NAME, //0
	T_SELECTED_DMG, //1
	T_PLAYER_ID, //2
	T_START_GAME, //3
	T_KILLED_BY, //4
	T_KILL_CONFIRM, //5
	T_LAST_MINUTE, //6
	T_GAME_OVER, //7
	R_KILLED_BY, //8
	T_HP, //9
	R_REQ_PLAYERID, //10
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
		}
		out += ';';
		return out;
	}


};

/// \brief
/// client class
/// \details
/// this class contains all data you would want to know about your client
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
	int deaths = 0;
};
/// \brief
/// tcpServer class
/// \details
/// this class is the tcp class used in the project
class tcpServer {
private:
	int id = 0;
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
	int maxClients;
	client clients[6];
	bool start = false;

	/// \brief
	/// construct a tcpServer class 
	/// \details
	/// construct a tcpServer class using a port, a array of clients, and the maximum clients that are allowed to connect
	tcpServer(std::string port, client clients[6], int maxClients);
	~tcpServer();

	/// \brief
	/// acceptcl accepts a client 
	/// \details
	/// accept a client 
	/// parameter is a pointer to a not yet initialized client
	int acceptcl(client *c);

	/// \brief
	/// listenNewClients listens for new clients
	/// \details
	/// new clients are listened to and accepted given there are no errors
	void listenNewClients();

	/// \brief
	/// recieve messages from clients 
	/// \details
	/// recieve messages from clients and act upon them
	/// this is what's wrong with this library it should just return its val but it does not
	/// instead it processes th message as well
	void receiveCli();

	/// \brief 
	/// sendCLi sends a msg to a client
	/// \details
	/// send a message to a specific client 
	void sendCli(SOCKET cli, msg m);

	/// \brief
	/// sendAll sends a msg to all clients 
	/// \details
	/// sendAll sends a msg to all clients using a for loop
	void sendAll( msg m);

	/// \brief
	/// startGame starts game 
	void startGame();

	/// \brief
	/// oneMinuteLeft notifies players only one minute is left
	void oneMinuteLeft();

	/// \brief
	/// gameOver notifies players only one minute is left
	void gameOver();
};
#endif // !_TCPSERVER_HPP

