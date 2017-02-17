//=============================================================================
// Introductory Network Programming
//
// @description: Module for providing functions to work with Message objects
// @author: Elisha Lai
// @version: 1.0 09/02/2017
//=============================================================================

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>

class Message {
   std::string text;

   // Writes the length of the text into the buffer 
   void writeTextLength(char* buffer);

   // Writes the text into the buffer
   void writeText(char* buffer);

   // Gets data to be sent
   char* getData();

   // Reads the length of the text from the buffer
   static unsigned int readTextLength(char* buffer);

public:
   Message(std::string text); // Constructor

   // Returns the text
   std::string getText();

   // Converts the text to title case by capitalizing the first
   // character of every word and converting the rest of the word
   // to lower case
   void convertToTitleCase();

   // Creates a message to send data and writes it out to the data
   // transfer socket
   void send(int dataTransferSocket);

   // Creates a message to receive data and reads into it from the data
   // transfer socket
   static void receive(int dataTransferSocket, Message &parsedMessage,
      int &result);
};

#endif
