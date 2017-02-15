##==============================================================================
## Introductory Network Programming
##
## @description: Makefile for stringClient.cpp and stringServer.cpp
## @author: Elisha Lai
## @version: 1.0 09/02/2017
##==============================================================================

CXX = g++
CXXFLAGS = -Wall -MMD
CLIENTNAME = stringClient
SERVERNAME = stringServer

all:
	@echo "Compiling..."
	${CXX} ${CXXFLAGS} -o ${CLIENTNAME} ${CLIENTNAME}.cc message.cc -lpthread
	${CXX} ${CXXFLAGS} -o ${SERVERNAME} ${SERVERNAME}.cc message.cc
	@echo "Granting file execution permissions..."
	chmod u+x ${CLIENTNAME}
	chmod u+x ${SERVERNAME}

.PHONY: clean

clean:
	find . -name ${CLIENTNAME} -delete
	find . -name ${SERVERNAME} -delete
	find . -name "*.d" -delete
	find . -name "*.o" -delete
