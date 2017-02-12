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

class Message {
   std::string text;

   // Gets data to be sent
   std::string getData();

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
