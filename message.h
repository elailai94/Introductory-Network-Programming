//=============================================================================
// CS 454 Assignment 2
//
// @description: Module for providing functions to work with Message objects
// @author: Ah Hoe Lai
// @userid: ahlai
// @version: 1.0 09/02/2017
//=============================================================================

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>
#include <sstream>

class Message {
   std::string text;
/*
   // Writes an integer into the buffer 
   void putInt(std::stringbuf &buffer, int value);

   // Writes a string into the buffer
   void putString(std::stringbuf & buffer, std::string value, int length);
*/
   // Gets data to be sent
   char* getData();

   // Reads an integer from the buffer
   static int getInt(std::stringbuf &buffer);

   // Reads a string from the buffer
   static std::string getString(std::stringbuf &buffer, int length);

   // Parses data received
   static Message parseData(std::string data);

public:
   Message(std::string text); // Constructor

   // Returns the text
   std::string getText();

   // Creates a message to send data and writes it out to the data
   // transfer socket
   void send(int dataTransferSocket);

   // Creates a message to receive data and reads into it from the data
   // transfer socket
   static Message receive(int dataTransferSocket);
};

#endif
