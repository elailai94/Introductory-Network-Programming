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
   iss >> textLength >> skipws >> text;
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
   cout << data << endl;
   //int num_of_bytes_sent =
      ::send(dataTransferSocket, data.c_str(), (data.length() + 1), 0);
} // send

// See interface (header file)
Message Message::receive(int dataTransferSocket) {
   char data[1024]; 
   //int num_of_bytes_received =
      ::recv(dataTransferSocket, data, sizeof(data), 0);
   cout << string(data) << endl;
   Message parsedMessage = parseData(string(data));
   return parsedMessage;
} // receive
