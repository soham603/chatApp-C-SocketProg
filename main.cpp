#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <thread>
#include<string.h>
#include <string>
using namespace std;

#pragma comment (lib, "ws2_32.lib")

/*
	//init winsock
	//create socket
	conectr to server
	send, recv
	close the socket
*/

bool Init() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMessageToServer(SOCKET s, const string& message) {
	send(s, message.c_str(), message.length(), 0);
}

void SendMessagee(SOCKET s) {
	cout << "enter your chat name: " << endl;
	string name;
	getline(cin, name);
	string message;
	while (1) {
		getline(cin, message);
		string msg = name + ':' + message;
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			cout << "Error sending messgae" << endl;
			break;
		}
		if (message == "quit") {
			cout << "stopping the application " << endl;
			break;
		}
	}

}

void RecieveMessage(SOCKET s) {
	char buffer[4096];
	int recvlenght;
	string recmsg = "";
	while (1) {
		recvlenght = recv(s, buffer, sizeof(buffer), 0);
		if (recvlenght <= 0) {
			cout << "disconnected from the server" << endl;
			break;
		}
		else {
			recmsg = string(buffer, recvlenght);
			cout << recmsg << endl;
		}
	}

}

int main() {
	if (!Init()) {
		cout << "Socket INIT Failed" << endl;
		return 1;
	}

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "Invalid socket created " << endl;
		return 1;
	}

	string serveraddress = "127.0.0.1";
	int port = 12345;

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "not able to connect to server" << endl;
		cout << ": " << WSAGetLastError();
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "Successfully connected to server" << endl;

	// Send login credentials
	string username, password;
	cout << "Enter username: ";
	getline(cin, username);
	cout << "Enter password: ";
	getline(cin, password);
	string loginData = username + ":" + password;
	SendMessageToServer(s, loginData);

	//thread senderThread(SendMessagee, s);
	//thread recieveThread(RecieveMessage, s);

	// Receive server response
	char buffer[4096];
	int recvLength = recv(s, buffer, sizeof(buffer), 0);
	if (recvLength > 0) {
		string response(buffer, recvLength);
		cout << "Server response: " << response << endl;
		if (response == "Login successful") {
			// Start sending and receiving messages
			thread senderThread(SendMessagee, s);
			thread recieveThread(RecieveMessage, s);

			senderThread.join();
			recieveThread.join();
		}
		else {
			// Handle login failure
			cout << "Login failed. Exiting..." << endl;
		}
	}

	//senderThread.join();
	//recieveThread.join();

	closesocket(s);
	WSACleanup();
	
	return 0;
}