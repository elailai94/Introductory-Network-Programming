//=============================================================================
// CS 454 Assignment 2
//
// @description: An implementation of a server program in C++
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// Returns the server address
string getServerAddress() {
   char hostname[128];

   gethostname(hostname, sizeof(hostname));
   return string(hostname);
} // getServerAddress

// Returns the server port
int getServerPort(const int welcomeSocket) {
   struct sockaddr_in serverAddress;
   socklen_t serverAddressLength = sizeof(serverAddress);
   const int result = getsockname(welcomeSocket, (struct sockaddr*) &serverAddress, &serverAddressLength);
   if (result < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if
   return ntohs(serverAddress.sin_port);
} // getServerPort

int main() {
   // Creates the server TCP socket
   const int welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (welcomeSocket < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   // Get next available port
   struct addrinfo hints;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;
   struct addrinfo* res;
   int result = getaddrinfo(NULL, "0", &hints, &res);
   if (result != 0) {
      cerr << "ERROR: DNS lookup failed" << endl;
      exit(-1);
   } // if

   // Bind
   result = bind(welcomeSocket, res->ai_addr, res->ai_addrlen);
   if (result < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   freeaddrinfo(res);
   cout << "SERVER_ADDRESS " << getServerAddress() << endl;
   cout << "SERVER_PORT " << getServerPort(welcomeSocket) << endl;

   // Listen
   result = listen(welcomeSocket, 5);
   if (result < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   while (true) {
      // Accept
      struct sockaddr clientAddress;
      socklen_t clientAddressLength = sizeof(clientAddress);
      const int connectionSocket = accept(welcomeSocket, &clientAddress, &clientAddressLength);
      if (connectionSocket < 0) {
         cerr << "ERROR: " << endl;
         continue;
      } // if

      char buf[1024];
      int bytes_received = recv(connectionSocket, buf, sizeof(buf), 0);
      if (bytes_received < 0) {
         cerr << "ERROR: " << endl;
         exit(-1);
      } // if
      cout << buf << endl;

      close(connectionSocket);
   } // while

   // Closes the TCP connection between the client and the server
   close(welcomeSocket);
} // main