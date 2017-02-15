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
void Message::writeTextLength(char* buffer) {
   int textLength = text.length() + 1;
   buffer[0] = textLength >> 24;
   buffer[1] = textLength >> 16;
   buffer[2] = textLength >> 8;
   buffer[3] = textLength;
} // writeTextLength

// See interface (header file)
void Message::writeText(char *buffer) {
   strcpy(buffer + sizeof(int), text.c_str());
} // writeText

// See interface (header file)
char* Message::getData() {
   int bufferLength = sizeof(int) + text.length() + 1;
   char* buffer = new char[bufferLength]();
   writeTextLength(buffer);
   writeText(buffer);
   return buffer;
} // getData

// See interface (header file)
int Message::readTextLength(char *buffer) {
   int textLength = buffer[0] << 24 | 
                    buffer[1] << 16 |
                    buffer[2] << 8  |
                    buffer[3];
   return textLength;
} // readTextLength

// See interface (header file)
Message::Message(string text)
   : text(text) {} // Constructor

// See interface (header file)
string Message::getText() {
   return text;
} // getText

// See interface (header file)
void Message::convertToTitleCase() {
   for (unsigned int i = 0; i < text.length(); ++i) {
      if (i == 0 || isspace(text[i - 1])) {
         text[i] = toupper(text[i]);
      } else {
         text[i] = tolower(text[i]);
      } // if
   } // for
} // convertToTitleCase

// See interface (header file)
void Message::send(int dataTransferSocket) {
   char* data = getData();
   int totalNumOfBytesData = sizeof(int) + text.length() + 1;
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

   delete[] data;
} // send

// See interface (header file)
void Message::receive(int dataTransferSocket, Message &parsedMessage,
   int &result) {
   char textLengthBuffer[sizeof(int)] = {0};
   int numOfBytesReceived = 
      ::recv(dataTransferSocket, textLengthBuffer, sizeof(int), 0);
   if (numOfBytesReceived < 0 || numOfBytesReceived == 0) {
      result = -1;
      return;
   } // if
   int textLength = readTextLength(textLengthBuffer);

   char textBuffer[textLength];
   int totalNumOfBytesData = textLength;
   int numOfBytesLeft = totalNumOfBytesData;
   int totalNumOfBytesReceived = 0;

   while (totalNumOfBytesReceived < totalNumOfBytesData) {
      numOfBytesReceived =
         ::recv(dataTransferSocket, textBuffer + totalNumOfBytesReceived,
            numOfBytesLeft, 0);
      if (numOfBytesReceived < 0 || numOfBytesReceived == 0) {
         result = -1;
         return;
      } // if

      totalNumOfBytesReceived += numOfBytesReceived;
      numOfBytesLeft -= numOfBytesReceived;
   } // while

   string text = string(textBuffer);
   parsedMessage.text = text;
   //Message parsedMessage = Message(string(text));
} // receive
