#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include <thread>
#include <vector>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

/*
	//init winsock libraries
	//create the socket
	// get ip and port
	//bind ip/port with socket
	//accept
	//recieve and send 
	//close the soclet
	//cleanup the winsock
*/

//1 - initialize windows socket library:
bool InitSocket() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET> &clients) {
	cout << "Client connected: "<< endl;
	char buffer[4096];
	while (1) {
		int bytesRecv = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (bytesRecv <= 0) {
			cout << "Client Disconnected" << endl;
		}
		string message(buffer, bytesRecv);
		cout << "Message from client " << message << endl;

		for (auto client : clients) {
			if (client != clientSocket) {
				send(client, message.c_str(), message.length(), 0);
			}
			
		}
	}
	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}

	closesocket(clientSocket);
}



int main() {
	if (!InitSocket()) {
		cout << "Winsock Initialization failed"<<endl;
	}

	
	cout << "Server Running: "<< endl;


	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM,0);
	if (listenSocket == INVALID_SOCKET) {
		cout << "Socket Creation Failed" << endl;
	}

	//2 - create address structure
	sockaddr_in serveraddr;
	int port = 12345;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	//3 - convert ip addr put inside sin_family in binary form
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << "Setting address structure failed: " << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	// bind ip and address
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "Binding Failed: "<< endl;
		closesocket(listenSocket);
		WSACleanup;
		return 1;
	}

	//listen the socket, amount of time in queue
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Lsiten Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server Started listening on port" << port << endl;
	vector<SOCKET> clients;
	while (1) {
		//accept //ClientSocket = connected client and listenSocket for comming clents
		SOCKET ClientSocket = accept(listenSocket, nullptr, nullptr);
		if (ClientSocket == INVALID_SOCKET) {
			cout << "Invalid Socket" << endl;
		}

		clients.push_back(ClientSocket);
		thread t1(InteractWithClient, ClientSocket, std::ref(clients));
		t1.detach();
	}
 
	closesocket(listenSocket);

	WSACleanup();

	return 0;
}