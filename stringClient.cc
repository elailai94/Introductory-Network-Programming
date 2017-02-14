//=============================================================================
// CS 454 Assignment 2
//
// @description: An implementation of a client program in C++
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "message.h"

using namespace std;

pthread_mutex_t messagesNotSentLock = PTHREAD_MUTEX_INITIALIZER;
queue<Message> messagesNotSent;
bool isEOF = false;

// Checks the number and formats of the environement variables passed
void checkEnvironmentVariables() {
   char* serverAddress = getenv("SERVER_ADDRESS");
   if (serverAddress == 0) {
      exit(-1);
   } // if

   char* serverPort = getenv("SERVER_PORT");
   if (serverPort == 0) {
      exit(-1);
   } // if
} // checkEnvironmentVariables

// Handles reading input from the user
void *handleReadingInput(void *arg) {
    string line;

    while (getline(cin, line)) {
       Message messageToServer = Message(line);
       pthread_mutex_lock(&messagesNotSentLock);
       messagesNotSent.push(messageToServer);
       pthread_mutex_unlock(&messagesNotSentLock);
    } // while

    isEOF = true;

    pthread_exit(0);
} // handleReadingInput

// Handles sending requests to the server
void *handleSendingRequests(void *arg) {
   long clientSocket = (long) arg;

   while (!isEOF) {
      if (!messagesNotSent.empty()) {
         Message messageToServer = messagesNotSent.front();

         pthread_mutex_lock(&messagesNotSentLock);
         messagesNotSent.pop();
         pthread_mutex_unlock(&messagesNotSentLock);

         messageToServer.send(clientSocket);
         sleep(2);
      } // if
   } // while

   // Closes the TCP connection between the client and the server
   close(clientSocket);

   pthread_exit(0);
} // handleSendingRequests

int main() {
   // Checks the number and formats of the environment variables passed
   checkEnvironmentVariables();

   // Creates the client TCP socket
   long clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (clientSocket < 0) {
      exit(-1);
   } // if

   char* serverAddress = getenv("SERVER_ADDRESS");
   char* serverPort = getenv("SERVER_PORT");
   
   struct addrinfo serverAddressHints;
   memset(&serverAddressHints, 0, sizeof(serverAddressHints));
   serverAddressHints.ai_family = AF_INET;
   serverAddressHints.ai_socktype = SOCK_STREAM;
   
   struct addrinfo* serverAddressResults;
   
   // Performs a DNS lookup on the server host name to obtain the server's IP address
   int result = getaddrinfo(serverAddress, serverPort,
      &serverAddressHints, &serverAddressResults);
   if (result != 0) {
      exit(-1);
   } // if

   // Initiates the TCP connection between the client and the server
   result = connect(clientSocket, serverAddressResults->ai_addr,
      serverAddressResults->ai_addrlen);
   if (result < 0) {
      freeaddrinfo(serverAddressResults);
      exit(-1);
   } // if

   freeaddrinfo(serverAddressResults);

   pthread_t readingInputThread;

   // Creates a new thread to handle reading input from the user
   result = pthread_create(&readingInputThread, 0, handleReadingInput, 0);
   if (result < 0) {
      exit(-1);
   } // if

   pthread_t sendingRequestsThread;

   // Creates a new thread to handle sending requests to the server
   result = pthread_create(&sendingRequestsThread, 0, handleSendingRequests,
      (void *) clientSocket);
   if (result < 0) {
      exit(-1);
   } // if
/*
   string s = "document specification FOR CS454 a2 milestone hello WORLD";
   Message messageToServer = Message(s);
   messageToServer.send(clientSocket);
   cout << messageToServer.getText() << endl;
*/

   pthread_exit(0);
} // main
