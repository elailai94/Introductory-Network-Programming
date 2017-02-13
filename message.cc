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

// See interface (header file)
void Message::putInt(stringbuf &buffer, int value) {
   buffer.sputc(value >> 24);
   buffer.sputc(value >> 16);
   buffer.sputc(value >> 8);
   buffer.sputc(value);
} // putInt

// See interface (header file)
void Message::putString(stringbuf &buffer, string value, int length) {
   buffer.sputn(value.c_str(), length);
} // putString

// See interface (header file)
string Message::getData() {
   stringbuf buffer;
   putInt(buffer, text.length() + 1);
   putString(buffer, text, text.length());
   return buffer.str();
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
   string text = getString(buffer, textLength - 1);
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
   char* cstr = new char[data.length() + 1];
   strcpy(cstr, data.c_str());
   int totalNumOfBytesData = data.length() + 1;
   int numOfBytesLeft = totalNumOfBytesData;
   int totalNumOfBytesSent = 0;

   while (totalNumOfBytesSent < totalNumOfBytesData) {
      int numOfBytesSent =
         ::send(dataTransferSocket, cstr + totalNumOfBytesSent,
            numOfBytesLeft, 0);
      cout << cstr << endl;
      cout << numOfBytesSent << endl;
      if (numOfBytesSent < 0) {
         break;
      } // if

      totalNumOfBytesSent += numOfBytesSent;
      numOfBytesLeft -= numOfBytesSent;
   } // while
} // send

// See interface (header file)
Message Message::receive(int dataTransferSocket) {
   char data[18]; 
   int num_of_bytes_received =
      ::recv(dataTransferSocket, data, sizeof(data), 0);
   cout << num_of_bytes_received << endl;
   cout << strlen(data) << endl;
   Message parsedMessage = parseData(string(data));
   return parsedMessage;
} // receive
