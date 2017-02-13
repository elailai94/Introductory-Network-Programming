//=============================================================================
// CS 454 Assignment 2
//
// @description: Module for providing functions to work with Message objects
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include "message.h"

using namespace std;
/*
// See interface (header file)
void Message::putInt(char* buffer, int value) {
   buffer[0] = value >> 24;
   buffer[1] = value >> 16;
   buffer[2] = value >> 8;
   buffer[3] = value;
} // putInt

// See interface (header file)
void Message::putString(char *buffer, char *value, int length) {
   buffer.sputn(value.c_str(), length);
} // putString
*/
// See interface (header file)
char* Message::getData() {
   int dataLength = (sizeof(int) + text.length() + 1);
   char* buffer = new char[dataLength];
   int textLength = (text.length() + 1);
   buffer[0] = textLength >> 24;
   buffer[1] = textLength >> 16;
   buffer[2] = textLength >> 8;
   buffer[3] = textLength;

   int i = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
   cout << i << endl;

   strcpy(buffer + 4, text.c_str());

   //putInt(buffer, text.length() + 1);
   //putString(buffer, text, text.length());
   return buffer;
} // getData

// See interface (header file)
int Message::getInt(stringbuf &buffer) {
   int value = (buffer.sbumpc() << 24) | 
               (buffer.sbumpc() << 16) |
               (buffer.sbumpc() << 8)  |
               buffer.sbumpc();
   return value;
} // getInt

// See interface (header file)
string Message::getString(stringbuf &buffer, int length) {
   char value[length];
   buffer.sgetn(value, length);
   return string(value);
} // getString

// See interface (header file)
Message Message::parseData(string data) {
   stringbuf buffer(data);
   int textLength = getInt(buffer);
   cout << textLength <<endl;
   string text = getString(buffer, textLength - 1);
   cout << text << endl;
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
   char* data = getData();
   int totalNumOfBytesData = sizeof(data);
   int numOfBytesLeft = totalNumOfBytesData;
   int totalNumOfBytesSent = 0;

   while (totalNumOfBytesSent < totalNumOfBytesData) {
      int numOfBytesSent =
         ::send(dataTransferSocket, data + totalNumOfBytesSent,
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
   char data[20]; 
   int num_of_bytes_received =
      ::recv(dataTransferSocket, data, sizeof(data), 0);
   cout << num_of_bytes_received << endl;
   Message parsedMessage = parseData(string(data));
   return parsedMessage;
} // receive
