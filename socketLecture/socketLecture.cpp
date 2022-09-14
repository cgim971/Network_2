#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <thread>
#include <vector>
#include <string>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


struct client_type {
	int id;
	SOCKET socket;
};

const char OPTION_VALUE = 1;
const int MAX_CLIENTS = 5;

int process_client(client_type& new_client, std::vector<client_type>& client_array, std::thread& thread);
int main();

int process_client(client_type& new_client, std::vector<client_type>& client_array, std::thread& thread) {
	return 0;
}

int  main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET server_socket = INVALID_SOCKET;

	struct addrinfo* server = NULL;
	struct addrinfo hints;

	std::string msg = "";
	std::vector<client_type> client(MAX_CLIENTS);
	int num_clients = 0;
	int temp_id = -1;
	std::thread my_thread[MAX_CLIENTS];

	// Initialize Winsock
	std::cout << "Initializing Winsock..." << std::endl;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::cout << "Setting up server..." << std::endl;
	getaddrinfo(NULL, DEFAULT_PORT, &hints, &server);

	std::cout << "Creating server socket..." << std::endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int));
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int));

	std::cout << "Binding socket..." << std::endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

	std::cout << "Listening..." << std::endl;
	listen(server_socket, SOMAXCONN);

	for (int i = 0; i < MAX_CLIENTS; i++) {
		client[i] = { -1, INVALID_SOCKET };
	}

	while (1) {
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL);

		if (incoming == INVALID_SOCKET) continue;

		num_clients = -1;
		temp_id = -1;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (client[i].socket == INVALID_SOCKET && temp_id == -1) {
				client[i].socket = incoming;
				client[i].id = i;
				temp_id = i;
			}

			if (client[i].socket != INVALID_SOCKET) {
				num_clients++;
			}
		}

		if (temp_id != -1) {
			std::cout << "Client #" << client[temp_id].id << "Accepted" << std::endl;
			msg = std::to_string(client[temp_id].id);
			send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);

			my_thread[temp_id] = std::thread(process_client, std::ref(client[temp_id]), std::ref(client), std::ref(my_thread[temp_id]));
		}
		else {
			msg = "Server is Full";
			send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << msg << std::endl;
		}
	}




	return 0;
}