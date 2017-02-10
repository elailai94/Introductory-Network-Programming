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

using namespace std;

int main() {
   const string serverAddress(getenv("SERVER_ADDRESS"));
   const string serverPort(getenv("SERVER_PORT")); 
	cout << serverAddress << endl;
   cout << serverPort << endl;
} // main
