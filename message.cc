//=============================================================================
// CS 454 Assignment 2
//
// @description: Module for providing functions to work with Message objects
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include "message.h"

using namespace std;

// See interface (header file)
string Message::getData() {
   ostringstream oss;
   oss << (text.length() + 1) << text;
   return oss.str();
} // getData

// See interface (header file)
Message Message::parseData(string data) {
   istringstream iss(data);
   int textLength;
   string text;
   iss >> textLength;
   getline(iss, text);
   return Message(text);
} // parseData

// See interface (header file)
Message::Message(string text)
   : text(text) {} // Constructor

// See interface (header file)
string Message::getText() {
   return text;
} // getText

// See interface (header file)
void Message::send(int dataTransferSocket) {
   string data = getData();
   int totalNumOfBytesData = data.length();

   while (totalNumOfBytesSent < totalNumOfBytesData) {
      int numOfBytesSent =
         ::send(dataTransferSocket, data.c_str() + totalNumOfBytesSent,
            numOfBytesLeft, 0);
      if (numOfBytesSent < 0) {
         break;
      } // if

      totalNumOfBytesSent += numOfBytesSent;
      numOfBytesLeft -= numOfBytesSent;
   } // while
} // send

// See interface (header file)
Message Message::receive(int dataTransferSocket) {
   char data[1024]; 
   //int num_of_bytes_received =
      ::recv(dataTransferSocket, data, sizeof(data), 0);
   Message parsedMessage = parseData(string(data)); //?
   return parsedMessage;
} // receive
