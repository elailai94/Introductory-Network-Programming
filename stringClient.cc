//=============================================================================
// CS 454 Assignment 2
//
// @description: An implementation of a client program in C++
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

//
/*void checkEnvironmentVariables() {
   const char* serverAddress = getenv("SERVER_ADDRESS");
   if (serverAddress == NULL) {

   } // if

   const char* serverPort = getenv("SERVER_PORT");
   if (serverPort == NULL) {

   } // if
//} // checkEnvironmentVariables
*/

// Handles reading input from the user
//void *handleReadingInput(void *) {

//} // handleReadingInput

// Handles sending requests to the server
//void *handleSendingRequests(void *) {

//} // handleSendingRequests

int main() {
   //try {

   //} catch (const string& errorMessage) {
   //   cerr << errorMessage << endl;
   //} // 
   const char* serverAddress = getenv("SERVER_ADDRESS");
   if (serverAddress == 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   const char* serverPort = getenv("SERVER_PORT");
   if (serverPort == 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   // Creates the client TCP socket
   const int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (clientSocket < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   // Performs a DNS lookup on the server host name to obtain the server's IP address
   struct addrinfo hints;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   struct addrinfo* res;
   int result = getaddrinfo(serverAddress, serverPort, &hints, &res);
   if (result != 0) {
      cerr << "ERROR: DNS lookup failed" << endl;
      exit(-1);
   } // if

   // Initiates the TCP connection between the client and the server
   result = connect(clientSocket, res->ai_addr, res->ai_addrlen);
   if (result < 0) {
      cerr << "ERROR: connection failed" << endl;
      exit(-1);
   } // if

   freeaddrinfo(res);

   // 
   const char* msg = "test string";
   const int len = strlen(msg);
   int bytes_sent = send(clientSocket, msg, len + 1, 0);
   if (bytes_sent < 0) {
      cerr << "ERROR: " << endl;
      exit(-1);
   } // if

   // Closes the TCP connection between the client and the server
   close(clientSocket);
} // main
