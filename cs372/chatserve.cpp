
/* Andrew M. Calhoun
calhouna@oregonstate.edu
Due Date: 2/7/2016
Project I 
chatserver.cpp

// Some Patterns are based off of Beej's guide: 
// http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleserver

Multithreading based off of: 
// http://www.cplusplus.com/forum/unices/116977/

This project includes multithreading, and allows up to 5 connections.

Chat Server Defines and Includes */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

using namespace std;

// Message Buffer Should Be 500 or less.
#define MSGBUFFER 500
#define USERNAME "calhouna"
#define DEBUG 0
char myServer[12] = "calhouna > ";


// Create Class, as global variables while useable, are 
// a bit... unfavorable due to the potential problems they
// can present. A class felt a bit more clean and organized.
class ChatServer {
public:

// void *task(void *);	// A relic from the C++ multithreading thread found on the cplusplus.com forum. 
						// It broke the program, so I removed it from operation, but may come back to work on it in the future.
void openSocket(int portNum); 
void actSocket();				// Broke these three functions up to refresh myself on data persistence in classes. Could be combined technically.
void createConnection();
void rcvMsg(int);				// receive messages from the client.
void sMsg(int);					// send messages to the client.
//void clrMsg();				// Not used, as bzero served the purposes this function would have fulfilled. Basic memset(outgoing/incoming, 0, sizeof(message)) type commands.
void errorMsg(const char *msg); // I used error messages a lot, so it made sense to have a function that just took care of it to avoid redundant code.

private: 

	int listenfd, connfd, noThread, n, i;	// variable holders for the listening and connection socket descriptors, number of threads, n placeholder, and i index variable. 
	bool quitFlag;							// holds the bool for the quit flag. If true, program can quit without breaking the pipe. Needs to be persistence on the server end.
	pid_t procID;							// for multi-threading
	char buffer[MSGBUFFER];					// holds the message buffer
	char input[MSGBUFFER];					// input buffer for outgoing
	char outMessage[MSGBUFFER];				// the outgoing mesage
	char recMsg[MSGBUFFER];					// the incoming message
	char serverName[MSGBUFFER];				// the server name (should be myServer or USERNAME, but can be changed for purposes of program)
	size_t msgLn;							// Holds the msg Length, basically a short hand for an unsigned int, specialized for strings: http://www.cplusplus.com/reference/cstring/size_t/
	struct sockaddr_in serverAddress, clientAddress;	// sockaddr_in - allows use of the sin_family members
	pthread_t threadA[3];					// Not used, but pulled from CPP forum demonstration/discussion on multi-threading
	socklen_t clientLength;					// from socket.h, holds the socket length for the client address.
};

/*
** FUNCTION: errorMsg()
** Description: tells the user if something went wrong
** Parameters: const char *msg
** Pre: Something went wrong...
** Out: Exits the program and lets the user know what went wrong.
*/


void ChatServer::errorMsg(const char *msg)
{
	cout << "Something went wrong." << endl;
	cout << msg << endl;
	exit(1);
}

/*
** FUNCTION: openSocket()
** Description: starts up the socket so the server can start
** receiving transmissions from the client.
** Parameters: int portNum - the port number.
** Pre: Program start and portnum given.
** Out: Socket Created and assigned to serverAddress struct.
*/

void ChatServer::openSocket(int portNum)
{
	listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// Tried this based on user suggestion from cplusplus forum. Works okay.
	
	if(DEBUG == 1)
		cout << "Listen FD: " << listenfd << endl;


	if(listenfd < 0)
		errorMsg("Problem opening server socket.");

	//memset(&serverAddress, '0', sizeof(serverAddress)); // learned from CS344 and CS362, helps keep memory cleared 
														// and primes a structure and structure member for work.
	
	bzero((char *) &serverAddress, sizeof(serverAddress));


	serverAddress.sin_family = PF_INET;					// http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html / http://beej.us/net2/html/syscalls.html
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(portNum);

}

/*
** FUNCTION: actSocket()
** Description: Binds the socket for transmission and begins listening. .
** Parameters: N/A
** Pre: Socket has been created.
** Out: Socket is bound and ready to be attached.
*/

// http://www.linuxhowtos.org/manpages/2/bind.htm
// http://www.linuxhowtos.org/manpages/2/listen.htm

void ChatServer::actSocket()
{

	if(bind(listenfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		errorMsg("Cannot bind to server socket");
	

	// if(bind(listenfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
	//	errorMsg("Cannot bind to socket.");

	listen(listenfd, 5);
	clientLength = sizeof(clientAddress);

}

/*
** FUNCTION: createConnection()
** Description: Now the magic begins! The connection is created and
				we can start sending and receiving messages.
** Pre: The program is started and a socket is created and bound.
** Out: Once the client connects -- A nice chat.
*/

void ChatServer::createConnection()
{
	cout << "Server initiated, awaiting client..." << endl;

	quitFlag = 0;

	while(1)
	{

		connfd = accept(listenfd, (struct sockaddr*) &clientAddress, &clientLength);

		if(connfd < 0)
			errorMsg("Could not accept connection.");

		procID = fork();

		if(procID < 0)
		{
			errorMsg("Error on fork");
		}

		if(procID == 0)
		{
		//	close(listenfd);
		//	bzero(recMsg,500);

			n = recv(connfd, recMsg, 7, 0);
			if(n < 0)
				errorMsg("Error reading from socket.");

			if(strcmp(recMsg, "PORTNUM") == 0)
			{
				cout << "Connection established." << endl;
			}

			while(quitFlag == false)
			{
				// bzero(outMessage, 500); -- zeroed out the messages
				// bzero(recMsg, 500); -- zeroed out the messages
				rcvMsg(connfd); // Has to receive first.
				sMsg(connfd); // then we can send.
			}

			if(quitFlag==true)
			{
			close(connfd);
			close(listenfd);

			}
		}
	}
}

/*
** FUNCTION: sMsg()
** Description: The server sends a message to the client.
** Pre: The server has received a message from the client, and is ready to reply.
** Out: The message is sent to the client.
*/


void ChatServer::sMsg(int connfd)
{
	char *quit = (char*)malloc(4);

	cout << USERNAME << " >> ";
	bzero(input, 500);
	fgets(input, 500, stdin);

	
	msgLn = strlen(buffer) - 1;
	if(buffer[msgLn] == '\n')
		buffer[msgLn] == '\0';

	bzero(outMessage, 500);
	strcat(outMessage, USERNAME);
	strcat(outMessage, " >> ");
	strcat(outMessage, input);

	n = send(connfd, outMessage, 500, 0);
	if(n < 0)
		errorMsg("Error writing to the socket.");

	bzero(quit, 5);
	strncpy(quit, outMessage + 13, 5);

	if(DEBUG == 1)
    {
            int r = strlen(outMessage + 10);
            int s = strlen(quit);
            printf("Outmessage Length: %d\n", r);
            printf("Quit Length: %d\n", s);
            printf("Quit message: %s\n", quit);
    }


	if(strcmp(quit, "\\quit") == 0)
	{
		n = send(connfd, "Server has terminated connection", 500, 0);
		cout << "Server sent a quit message. Terminating connection.\n" << endl;
		quitFlag = true;
		exit(0);
	}
}

/*
** FUNCTION: rcvMsg()
** Description: the receive message protocol
** Parameters: int connfd
** Pre: A message is sent to the server.
** Out: Message is received, interpreted, and displayed to the server.
*/


void ChatServer::rcvMsg(int connfd)
{
	int i;
	char *quit = (char *)malloc(4);
	
	bzero(recMsg, 500);
	n = recv(connfd, recMsg, 500, 0);
	if(n < 0)
		errorMsg("Error reading from socket.");

	msgLn = strlen(recMsg);
	for(i=0; i < msgLn; i++)
	{
		if(recMsg[i] == '>')
			break;
	}
	i = i + 5;
	bzero(quit, 5);
	strncpy(quit, recMsg + i, 5);

	if(strcmp(quit, "\\quit") == 0)
	{
		cout << "Client has closed the connection. Will continue listening..." << endl;
		quitFlag = true;
	}

	cout << recMsg << endl;

	
}


/*void *ChatServer::task(void *dummyTd)
{
	cout << "Thread No: " << pthread_self() << endl;
	char test[500];
	bzero(test, 500);
	bool loop = false;
	while(!loop)
	{
		bzero(test, 301);
		read(connfd, test, 300);
		string tester(test);
		cout << tester << endl;

		if(tester == "\\quit")
			break;

		cout << "Closing thread and conn" << endl;
		close(connfd);

	}
}
*/

// The main function. Has the basic argument protection, and then starts the program as a whole.

int main(int argc, char *argv[])
{
	
	ChatServer cs; 

	if(argc < 2)
	{
		cout << (stderr, "No port provided.\nUsage: \"$ chatserve [port]\"") << endl;
		exit(1);
	}

	int portNum = atoi(argv[1]);
	if((portNum > 65535) || (portNum < 1024))
		cs.errorMsg("Please use range 1024-65335");

	cs.openSocket(portNum);
	cs.actSocket();
	cs.createConnection();


	return 0;

}
