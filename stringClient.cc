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

pthread_mutex_t messagesNotSentMutex = PTHREAD_MUTEX_INITIALIZER;
queue<Message> messagesNotSent;
bool isEOF = false;

// Checks the number and formats of the environment variables passed
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

// Sets up the client socket
long setUpClientSocket() {
   struct addrinfo serverAddressHints;
   struct addrinfo* serverAddressResults;

   // Creates the client socket
   long clientSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (clientSocket < 0) {
      exit(-1);
   } // if

   // Gets the environment variables passed
   char* serverAddress = getenv("SERVER_ADDRESS");
   char* serverPort = getenv("SERVER_PORT");
   
   // Sets up the server address hints and results to perform the DNS
   // lookup on the server's host name to obtain the server's IP address
   memset(&serverAddressHints, 0, sizeof(serverAddressHints));
   serverAddressHints.ai_family = AF_INET;
   serverAddressHints.ai_socktype = SOCK_STREAM;
   
   // Performs a DNS lookup on the server's host name to obtain the
   // server's IP address
   int result = getaddrinfo(serverAddress, serverPort,
      &serverAddressHints, &serverAddressResults);
   if (result != 0) {
      exit(-1);
   } // if

   // Initiates the TCP connection between the client and the server
   result = connect(clientSocket, serverAddressResults->ai_addr,
      serverAddressResults->ai_addrlen);
   if (result < 0) {
      exit(-1);
   } // if

   // Frees up memory allocated for the server address results
   freeaddrinfo(serverAddressResults);

   return clientSocket;
} //setUpSocket

// Handles reading input from the user
void *handleReadingInput(void *arg) {
    string text = "";

    while (getline(cin, text)) {
       // Creates a message to send data to the server and inserts it
       // into the messages not sent queue
       Message messageToServer = Message(text);
       pthread_mutex_lock(&messagesNotSentMutex);
       messagesNotSent.push(messageToServer);
       pthread_mutex_unlock(&messagesNotSentMutex);
    } // while

    isEOF = true;

    // Terminates the current thread
    pthread_exit(0);
} // handleReadingInput

// Handles sending requests to the server
void *handleSendingRequests(void *arg) {
   long clientSocket = (long) arg;

   //while ((!isEOF && !messagesNotSent.empty()) ||
   //       (isEOF && !messagesNotSent.empty())  ||
   //       (!isEOF && messagesNotSent.empty())) {
   while (!isEOF) {
      if (!messagesNotSent.empty()) {
         // Removes a message from the messages not sent queue and
         // writes it out to the client socket
         Message messageToServer = messagesNotSent.front();
         pthread_mutex_lock(&messagesNotSentMutex);
         messagesNotSent.pop();
         pthread_mutex_unlock(&messagesNotSentMutex);
         messageToServer.send(clientSocket);

         // Creates a message to receive data from the server and reads
         // into it from the client socket
         Message messageFromServer = Message("");
         int result = 0;
         Message::receive(clientSocket, messageFromServer, result);
         if (result == 0) {
            string titleCaseText = messageFromServer.getText();
            cout << "Server: " << titleCaseText << endl;
         } // if
         
         // Delays for two seconds between successive requests
         sleep(2);
      } // if
   } // while

   while (!messagesNotSent.empty()) {
      // Removes a message from the messages not sent queue and
         // writes it out to the client socket
         Message messageToServer = messagesNotSent.front();
         pthread_mutex_lock(&messagesNotSentMutex);
         messagesNotSent.pop();
         pthread_mutex_unlock(&messagesNotSentMutex);
         messageToServer.send(clientSocket);

         // Creates a message to receive data from the server and reads
         // into it from the client socket
         Message messageFromServer = Message("");
         int result = 0;
         Message::receive(clientSocket, messageFromServer, result);
         if (result == 0) {
            string titleCaseText = messageFromServer.getText();
            cout << "Server: " << titleCaseText << endl;
         } // if
         
         // Delays for two seconds between successive requests
         sleep(2);
   }

   // Terminates the current thread
   pthread_exit(0);
} // handleSendingRequests

int main() {
   pthread_t readingInputThread;
   pthread_t sendingRequestsThread;

   // Checks the number and formats of the environment variables passed
   checkEnvironmentVariables();

   // Sets up the client socket
   long clientSocket = setUpClientSocket();

   // Creates a new thread to handle reading input from the user
   int result = pthread_create(&readingInputThread, 0, handleReadingInput, 0);
   if (result != 0) {
      exit(-1);
   } // if

   // Creates a new thread to handle sending requests to the server
   result = pthread_create(&sendingRequestsThread, 0, handleSendingRequests,
      (void *) clientSocket);
   if (result != 0) {
      exit(-1);
   } // if

   // Waits for the reading input thread to terminate
   result = pthread_join(readingInputThread, 0);
   if (result != 0) {
      exit(-1);
   } // if

   // Waits for the sending requests thread to terminate
   result = pthread_join(sendingRequestsThread, 0);
   if (result != 0) {
      exit(-1);
   } // if

   // Closes the client socket
   close(clientSocket);

   // Terminates the current thread
   pthread_exit(0);
} // main
