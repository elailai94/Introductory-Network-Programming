//=============================================================================
// CS 454 Assignment 2
//
// @description: An implementation of a server program in C++
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "message.h"

using namespace std;

// Returns the server address
string getServerAddress() {
   char hostname[MAXHOSTNAMELEN] = {0};
   gethostname(hostname, sizeof(hostname));
   return string(hostname);
} // getServerAddress

// Returns the server port
int getServerPort(int welcomeSocket) {
   struct sockaddr_in serverAddress;
   socklen_t serverAddressLength = sizeof(serverAddress);
   
   int result = getsockname(welcomeSocket, (struct sockaddr*) &serverAddress,
      &serverAddressLength);
   if (result < 0) {
      exit(-1);
   } // if

   return ntohs(serverAddress.sin_port);
} // getServerPort

// Sets up the welcome socket
int setUpWelcomeSocket() {
   // Creates the welcome socket
   int welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (welcomeSocket < 0) {
      exit(-1);
   } // if

   // Sets up the server address hints and results to get the server's
   // IP address and the next available port
   struct addrinfo serverAddressHints;
   memset(&serverAddressHints, 0, sizeof(serverAddressHints));
   serverAddressHints.ai_family = AF_INET;
   serverAddressHints.ai_socktype = SOCK_STREAM;
   serverAddressHints.ai_flags = AI_PASSIVE;
   struct addrinfo* serverAddressResults;

   // Gets the server's IP address and the next available port
   int result = getaddrinfo(NULL, "0", &serverAddressHints,
      &serverAddressResults);
   if (result != 0) {
      exit(-1);
   } // if

   // Binds the socket to the server's IP address and the next
   // available port
   result = bind(welcomeSocket, serverAddressResults->ai_addr,
      serverAddressResults->ai_addrlen);
   if (result < 0) {
      exit(-1);
   } // if

   // Frees up memory allocated for the server address results
   freeaddrinfo(serverAddressResults);

   // Listens for TCP connection requests from the client
   result = listen(welcomeSocket, SOMAXCONN);
   if (result < 0) {
      exit(-1);
   } // if

   // Prints the server address and the server port on the server's
   // standard output
   cout << "SERVER_ADDRESS " << getServerAddress() << endl;
   cout << "SERVER_PORT " << getServerPort(welcomeSocket) << endl;

   return welcomeSocket;
} // setUpWelcomeSocket

int main() {
   // Sets up the welcome socket
   int welcomeSocket = setUpWelcomeSocket();

   while (true) {
      struct sockaddr clientAddress;
      socklen_t clientAddressLength = sizeof(clientAddress);

      // Creates the connection socket when a connection is made to the
      // welcome socket
      int connectionSocket = accept(welcomeSocket, &clientAddress,
         &clientAddressLength);
      if (connectionSocket < 0) {
         continue;
      } // if

      // Creates a message to receive data from the client and reads into
      // it from the connection socket
      Message messageFromClient = Message::receive(connectionSocket);
      string clientText = messageFromClient.getText();
      cout << clientText << endl;

      // Writes it out to the connection socket
      messageFromClient.convertToTitleCase();
      Message messageToClient = messageFromClient;
      messageToClient.send(connectionSocket);

      // Closes the TCP connection between the client and the server
      close(connectionSocket);
   } // while

   // Closes the welcome socket
   close(welcomeSocket);
} // main
