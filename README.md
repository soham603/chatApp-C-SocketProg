Distributed Chat Application
This repository contains a distributed chat application implemented in C++ using socket programming. The application consists of two main components: client and server.

Features
Client Side: Allows users to connect to the server, send messages, and receive messages from other connected users.
Server Side: Manages connections from multiple clients, relays messages between clients, and maintains the chat state.
Branches
client: Contains the client-side implementation.
server: Contains the server-side implementation.
How It Works
Client: Users can run the main.cpp in the client folder. Upon execution, they are prompted to connect to the server using the server's IP address and port.

Server: Run the main.cpp in the server folder. The server listens for incoming connections from clients. It manages multiple client connections and facilitates communication between them.

Chatting: Multiple users can simultaneously connect to the server. They can send messages which are broadcasted to all connected users.
