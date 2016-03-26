/*	
*	Loyd Maverick Ty
*	CS372 Project 2
*	ftserver.cpp
*	This is the server portion of the client-server network application.
*	This program uses the C++ socket API to listen for aconnection from
*	a client host in a port that is specified by the command-line argument.
*	It will listen for commands sent by the client and respond accordingly.
*/

#define _BSD_SOURCE
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <string>
#include <limits>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

//initializeSocket function
//INPUT: int portNum, the portNum that the socket will be initialize/bind to
//OUTPUT: int sockfd, the socket that is created by this function
//This function is to initialize the socket, and to bind the socket.
int initializeSocket(int portNum){
	int sockfd, optval;																					//Variables																					
	struct sockaddr_in server;																			//Server struct

	bzero((char *) &server, sizeof(server));															//Sets server buffer to zeros
	server.sin_family = AF_INET;																		//Prepare server information
	server.sin_addr.s_addr = INADDR_ANY;																//Prepare server information
	server.sin_port = htons(portNum);																	//Prepare server information
	sockfd = socket(AF_INET, SOCK_STREAM, 0);															//Create socket instance
	if (sockfd < 0) {																					//Check if socket creation is successful
		cout << "Error: Could not open socket." << endl;												//Can not open socket
		return -1;
	}
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int)) == -1) {							//Reuse port
		cout << "Error: Could not set reuse on particular port." << endl;								//Reuse option not set successfully
		return -1;
	}  
	if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0 ) {								//Bind socket
		cout << "Error: Could not bind socket." << endl;												//Binding failed
		return -1;
	}
	return sockfd;																						//Return socket
}

//acceptConnection function
//INPUT: int sockfd, this is the socket where the server will listen to connections
//hostBStringName, this is a string to hold the host name of the client
//OUTPUT: int sockNewfd, this is the socket
//This function is to accept the client host connection for commands
int acceptConnection(int sockfd, string &hostBStringName){
	int sockNewfd;																						//Variable
	char hostBName[1024];
	memset(hostBName, '\0', 1024);
	struct sockaddr_in client;																			//Client struct for connecting to client host
	socklen_t cli_len = sizeof(client);																	//Create length of client socket
	sockNewfd = accept(sockfd, (struct sockaddr*) &client, &cli_len);									//Accepting connection of a client host and puts it into the socket
	getnameinfo((struct sockaddr*)&client, sizeof(client), hostBName, sizeof(hostBName), NULL, 0, 0);
	hostBStringName = string(hostBName);
	return sockNewfd;																					//Return accepted socket
}

//listDirectory function
//INPUT: int sockDataNewfd, this is the socket for the data connection
//OUTPUT: none
//This function is the function that handles sending the directory listing to the client
void listDirectory(int sockDataNewfd){
	char buffer[1000];																					//Variables
	DIR *pDir;
	struct dirent *pEnt;
	memset(buffer, '\0', 1000);																			//Clears buffer

	pDir = opendir(".");																				//Open directory
	if (pDir == NULL) {
		cout << "Error: pDir not initialized properly." << endl;										//Error opening directory
		return;
	}
	while (pEnt = readdir (pDir)) 
	{
		if (pEnt == NULL) 
		{
			cout << "Error: pEnt not initialized properly." << endl;									//Error reading directory
			return;
		}
		read(sockDataNewfd, buffer, 1000);																//Wait for ok message
		memset(buffer, '\0', 1000);																		//Clears buffer
		strcpy(buffer, pEnt->d_name);																	//Copy directory listing
		write(sockDataNewfd, buffer, strlen(buffer));													//Send directory listing
	}
	read(sockDataNewfd, buffer, 1000);																	//Wait for ok message
	memset(buffer, '\0', 1000);																			//Clears buffer
	strcpy(buffer, "COMPLETE");																			//Transfer complete message
	write(sockDataNewfd, buffer, strlen(buffer));														//Send message
}

//fileTransfer function
//INPUT: int filefd, this is the file descriptor for reading the file
//int sockDataNewfd, this is the socket for the data connection
//int myLength, this is the text length
//OUTPUT: none
//This function is the function that handles the actual sending of the file transfer 
//between the server and client. It will send it piece by piece, until it is complete.
void sendFile(int filefd, int sockDataNewfd, int myLength){
	int byteRead, byteWrite;																				//Variables
	char buffer[1000];
	char response[100];

	memset(response, '\0', 100);																			//Clear response buffer
	sprintf(response, "%d", myLength);																		//Copy the length into the response
	write(sockDataNewfd, response, strlen(response));														//Send the length message to the client
	memset(response, '\0', 100);																			//Clear response buffer
	read(sockDataNewfd, response, 100);																		//Wait for ok message

	memset(buffer, '\0', 1000);																				//Clear buffer
	while(1){																								//Loop to read and then write file buffer
		byteRead = read(filefd, buffer, 1000);																//Reads file in chunks of 1000
		if (byteRead == 0)																					//End of file reading	
			break;
		for(int i = 0; i < byteRead; i += byteWrite) {														//Writes file in chunks of read value
			byteWrite = write(sockDataNewfd, buffer+i, byteRead-i);											//Write file chunk
			if (byteWrite < 0){
				cout << "There was an error during file transfer." << endl;									//Error writing to socket
				memset(response, '\0', 100);																//Clears response buffer
				strcpy(response, "ERROR: Writing to socket");												//Copy error message
				write(sockDataNewfd, response, strlen(buffer));												//Send message
				return;
			}
		}
	}
	read(sockDataNewfd, response, 100);																		//Wait for ok message
	memset(response, '\0', 100);																			//Clears response buffer
	strcpy(response, "COMPLETE");																			//Transfer complete message
	write(sockDataNewfd, response, strlen(response));														//Send message
	close(filefd);																							//Close file
}

//fileTransfer function
//INPUT: int sockDataNewfd, this is the socket for the data connection
//char *fileName, the filename of the file
//string hostBname, the name of the client host
//int portDataNum, the port of the data connection
//OUTPUT: none
//This function is the function that handles the file transfer between the server and client.
//This will first check if the file is in the directory, then if it is it will call the sendFile
//function to send the file piece by piece
void fileTransfer(int sockDataNewfd, char* fileName, string hostBName, int portDataNum){
	int textfd, fileFound, textLength;
	char buffer[1000];																						//Variables
	DIR *pDir;
	struct dirent *pEnt;

	pDir = opendir(".");																					//Open directory
	fileFound = 0;																							//Initial value
	if (pDir == NULL) {
		cout << "Error: pDir not initialized properly." << endl;											//Cannot open directory			
		return;
	}
	while (pEnt = readdir (pDir)) 
	{
		if (pEnt == NULL) 
		{
			cout << "Error: pEnt not initialized properly." << endl;										//Cannot read directory
			return;
		}
		if (strcmp(fileName, pEnt->d_name) == 0){
			fileFound = 1;																					//File found in the directory
			break;
		}
	}
	if (fileFound == 1) {
		textfd = open(fileName, O_RDONLY);																	//Open plaintext file
		if (textfd < 0) {																					//Check if text file can be opened
			cout << fileName << " could not be opened." << endl;											//Error opening plain text
			memset(buffer, '\0', 1000);																		//Clears buffer
			strcpy(buffer, "CANNOT OPEN FILE");																//Copy error message
			write(sockDataNewfd, buffer, strlen(buffer));													//Send message
			return;
		}
		textLength = lseek(textfd, 0, SEEK_END);															//Find length of plain text
		lseek(textfd, 0, SEEK_SET);																			//Set pointer back to beginning
		cout << "Sending \"" << fileName << "\" to " << hostBName << ":" << portDataNum << endl;			//Print out to the screen
		memset(buffer, '\0', 1000);																			//Clears buffer
		strcpy(buffer, "OK");																				//Copy okay message
		write(sockDataNewfd, buffer, strlen(buffer));														//Send message
		memset(buffer, '\0', 1000);																			//Clears buffer
		read(sockDataNewfd, buffer, 1000);																	//Reads socket to make sure its okay to proceed
		if (strcmp(buffer, "OK") == 0)
			sendFile(textfd, sockDataNewfd, textLength);													//Calls send file function
		else
			cout << "\"" << fileName << "\" transfer to " << hostBName << ":" << portDataNum << " was aborted." << endl;	//Transfer was aborted, already exists
	}
	else{
		cout << "File not found. Sending error message to " << hostBName << ":" << portDataNum << endl;		//File not found
		memset(buffer, '\0', 1000);																			//Clears buffer
		strcpy(buffer, "FILE NOT FOUND");																	//Copy error message
		write(sockDataNewfd, buffer, strlen(buffer));														//Send message
	}
}

//receiveCommand function
//INPUT: int sockNewfd, this is the connected socket for communcation between host and client
//OUTPUT: none
//This function is the function that handles the command from the client. It also handles creating the
//data connection between the server and client. It will call the other functions to do the server listing
//or file transfer.
void receiveCommand(int sockNewfd){
	char buffer[1000];																					//Variables
	int sockDatafd, sockDataNewfd, portDataNum;															
	char portNumString[6];
	char command[3];
	char fileName[500];
	string hostBName;

	memset(buffer, '\0', 1000);																			//Clears buffer
	memset(command, '\0', 3);																			//Clears command
	memset(portNumString, '\0', 6);																		//Clears port number
	memset(fileName, '\0', 500);																		//Clears file name

	read(sockNewfd, buffer, 1000);																		//Read client's command
	if (strcmp(buffer, "-g") == 0 || strcmp(buffer, "-l") == 0)
		strcpy(command, buffer);																		//Command is good, copy to command variable
	else {
		cout << "Invalid command: " << command << endl;													//Invalid command message
		strcpy(buffer, "Invalid command: ");															//Set up message to send to client
		strcat(buffer, command);																		//Set up message to send to client
		write(sockNewfd, buffer, strlen(buffer));														//Send message
		return;																							//Exit this function
	}

	memset(buffer, '\0', 1000);																			//Clears buffer
	strcpy(buffer, "OK");																				//Set up message to send to client that its okay to proceed
	write(sockNewfd, buffer, strlen(buffer));															//Send message
	memset(buffer, '\0', 1000);																			//Clears buffer
	read(sockNewfd, buffer, 1000);																		//Read port number
	strcpy(portNumString, buffer);																		//Stores port number into string variable
	portDataNum = atoi(portNumString);																	//Convert string to int

	sockDatafd = initializeSocket(portDataNum);															//Initialize Socket, this is for the data connection
	if (sockDatafd < 0) {
		cout << "Error: Failed to initialize socket." << endl;											//Failed to initialize socket
		return;																							//Exit this function
	}

	listen(sockDatafd, 5);																				//Listen to incomming connections, this is for the data connections
	while(1) {
		memset(buffer, '\0', 1000);																		//Clears buffer
		strcpy(buffer, "OK");																			//Set up message to send to client that its okay to proceed
		write(sockNewfd, buffer, strlen(buffer));														//Send message
		sockDataNewfd = acceptConnection(sockDatafd, hostBName);										//Accept connection, this is for the data connection
		if (sockDataNewfd < 0) {
			cout << "Error: Could not accept new socket." << endl;										//Error accepting new socket
			return;																						//Exit this function
		}
		if (strcmp(command, "-l") == 0){
			cout << "List directory requested on port " << portDataNum << endl;							//List command, ouput to screen
			cout << "Sending directory contents to " << hostBName << ":" << portDataNum << endl;		//Output to screen
			listDirectory(sockDataNewfd);																//Calls listDirectory function to handle sending directory list
		}
		else if (strcmp(command, "-g") == 0){
			read(sockDataNewfd, fileName, 500);															//Read file name
			cout << "File \"" << fileName << "\" requested on port " << portDataNum << endl;			//Get file command, ouput to screen
			fileTransfer(sockDataNewfd, fileName, hostBName, portDataNum);								//Calls fileTransfer function to handle file transfer
		}
		close(sockDataNewfd);																			//Close connection socket for the data connection
		close(sockDatafd);																				//Close the listening socket for the data connection
		break;
	}


}



//main function
//INPUT: int argc, char *argv[], these are the command line arguments
//OUTPUT: 1 for error, EXIT_SUCCESS for successful program (never actually hit)
//This function is the main function that calls the other functions to start the sockets
//and loops listening for incomming connections and loops calling the receive command function.
int main(int argc, char *argv[]){
    int sockfd, sockNewfd, portNum;																		//Variables
	char hostAName[1024];
	string hostBName;
	memset(hostAName, '\0', 1024);
	gethostname(hostAName, 1023);																		//Gets host name, this is just for outputs to screen
	if (argc < 2) {
		cout << "Usage: chatserve <port#>" << endl;														//Not enough command line arguments
		exit(1);
	}
	portNum = atoi(argv[1]);																			//Convert port number
	sockfd = initializeSocket(portNum);																	//Initialize Socket
	if (sockfd < 0) {
		cout << "Error: Failed to initialize socket." << endl;											//Failed to initialize socket
		exit(1);
	}
	else
		cout << "Server open on " << portNum << endl;													//Server open message

	listen(sockfd, 5);																					//Listen to incomming connections
	while(1) {
		sockNewfd = acceptConnection(sockfd, hostBName);												//Accept connection
		if (sockNewfd < 0) {
			cout << "Error: Could not accept new socket." << endl;										//Error accepting new socket
			exit(1);
		}
		else
			cout << "Connection from " << hostBName << endl;
		receiveCommand(sockNewfd);																		//Receive commands
		close(sockNewfd);																				//Close connected socket
	}
    close(sockfd);																						//Close server socket
    exit(EXIT_SUCCESS);																					//End of program
}