/* 
** Name:		Andrew M. Calhoun
** Assignment:  FTP Socket - Project 2
** Description: This is the server for an FTP services. Connected
** clients can request or send a file. The FTP control is managed
** over the connection. After closing, a connection with one
** client, it'll listen for another connection until an interrupt
** (Ctrl-C) is sent.
**
** Sources:		http://beej.us/guide/bgnet/
** 				http://en.cppreference.com/w/cpp/io/c/fscanf
**				http://stackoverflow.com/questions/10837514/creating-a-c-ftp-server
**				http://man7.org/linux/man-pages/man3/getnameinfo.3.html
*/

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>

#define HOST 1024
#define BL 5
#define NL NULL
#define BUFFER 504

using namespace std;

void isError(string errorMsg);

class FTPServer {

private:
	bool fileExists;
	bool dirExists; 

	int sockfd;
	int controlSock;
	int dataSock;
	int newSock;
	int serverPort;
	int optionValue;

	string type;
	string newLine;
	string start;
	string message;

	char buffer[2048];
	char host[1024];
	char service[20];

	socklen_t addressSize;

public:
	int initFTP(int srvPort);
	int openConnect(int sockfd, string type);
	void sendDirectory(int sockfd);
	void sendFile(int dataSock, int controlSock, string fileName);
	bool handleReq(int newSock, int sock);

};

void isError(string errorMsg)
{
	cout << "Something went wrong!" << endl;
	cout << errorMsg << endl;
	exit(1);
}

int FTPServer::initFTP(int srvPort)
{
	struct sockaddr_in serverAddress;
	optionValue = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0) isError("Error creating socket.");

	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(srvPort);

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) == -1) isError("Error setting SO_REUSEADDR");
	if(bind(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) isError("ERROR: Cannot bind to host address.");
	if(listen(sockfd, 5) < 0) isError("ERROR: Cannot listen for connection.");

	return sockfd;
}

int FTPServer::openConnect(int sockfd, string type)
{
	struct sockaddr_in clientAddress;
	addressSize = sizeof(clientAddress);
	newSock = accept(sockfd, (struct sockaddr *) &clientAddress, &addressSize);
	if((newSock) < 0) isError("ERROR: Cannot accept connection.");
	
	else
	{
		getnameinfo((struct sockaddr *)&clientAddress, sizeof(clientAddress), host, sizeof(host), service, sizeof(service), 0);
		cout << type << " connection from " << host << " has been successfully established. Please begin transmission." << endl;
	}

	return newSock;
}

void FTPServer::sendDirectory(int sockfd)
{
	DIR *dir;
	struct dirent *current;
	dir = opendir(".");
	newLine = "\n";
	start = "start";

	if(!dir) isError("ERROR: Invalid Argument for opendir().");
	if(send(sockfd, start.c_str(), strlen(start.c_str()), 0) < 0) isError("ERROR: Cannot send start message to client.");

	while((current = readdir(dir)))
	{
		cout << current->d_name << endl;	// learned about in CS344, Project 2 & 3.
		if(send(sockfd, current->d_name, strlen(current->d_name), 0) < 0)
			cout << "ERROR: Problem sending directory name to client!" << endl;
		usleep(200);

	}

	closedir(dir);
}

void FTPServer::sendFile(int dataSock, int controlSock, string fileName)
{
	DIR *dir;
	FILE *filePtr;
	fileExists = false;
	dir = opendir(".");
	struct dirent *current;

	if(!dir) isError("ERROR: Invalid Argument for opendir().");

	while((current = readdir(dir)))
	{
		if(strcmp(current->d_name, fileName.c_str()) == 0)
		{
			fileExists = true;
			break;
		}
	}

	closedir(dir);

	if(fileExists)
	{
		filePtr = fopen(fileName.c_str(), "r");
		if(filePtr==NL)
			cout << "ERROR: Cannot open file" << endl;

		message = "SUCCESS";
		if(send(controlSock, message.c_str(), strlen(message.c_str()), 0) < 0)
			cout << "ERROR: Could not send success message." << endl;

		while(fgets(buffer, sizeof(buffer), filePtr) != NL)
		{
			if(send(dataSock, buffer, strlen(buffer), 0) < 0)
				cout << "ERROR: Could not send file data." << endl;
			usleep(10);
		}

		fclose(filePtr);
	}

	else 
	{
		cout << "File not found, sending error message to client." << endl;
		message = "File not found. Please try again or a different fine.";
		if(send(controlSock, message.c_str(), strlen(message.c_str()), 0) < 0)
			cout << "ERROR: Could not send message to client." << endl;
	}
	

}

bool FTPServer::handleReq(int newSock, int sockfd)
{
	int bytesRead, welcomeSock, dPort;
	char buffer[BUFFER];
	char *tok, *args[BUFFER];
	const char *msg;
	
	bytesRead = recv(newSock, buffer, sizeof(buffer), 0);

	if(bytesRead>0) buffer[bytesRead] = '\0';
	else if(bytesRead<0) isError("Error receiving command from client.");
	else if(bytesRead==0) return false;
	
	tok = strtok(buffer, "[',]\n");
	for(int i=0; tok != NL; i++)
	{
		args[i] = tok;
		tok = strtok(NL, "[',]\n");
	}

	if(strcmp(args[2], "-l") == 0) 
	{
		dPort = atoi(args[3]);
		welcomeSock = initFTP(dPort);

		cout << "List directory on port " << dPort << endl;
		if(welcomeSock < 1) isError("ERROR: Problem creating welcome sock.");
	
		msg="SUCCESS";
		if(send(newSock, msg, strlen(msg), 0) < 0)
			cout << "Error sending data connection message to client." << endl;

		dataSock = openConnect(welcomeSock, "DATA");
		if(dataSock<1) isError("ERROR: Could not create data connection.");

		cout << "Sending directory contents to port: " << dPort << endl;

	}

	else if(strcmp(args[2], "-g") == 0)
	{
		dPort = atoi(args[4]);
		welcomeSock = initFTP(dPort);

		cout << "File " << args[3] << " requested on port: " << args[4] << endl;

		if(welcomeSock < 1)
			isError("ERROR: Problem creating welcome sock.");
		
		message = "SUCCESS";
		if(send(newSock, msg, strlen(msg), 0) < 0)
				isError("ERROR: Could not send data connection message to client.");

		dataSock = openConnect(welcomeSock, "DATA");
		if(dataSock < 1)
			isError("ERROR: Could not data connection.");

		cout << "Sending " << args[3] << " to port: " << args[4] << endl;
		sendFile(dataSock, newSock, args[3]);
	}

	else 
	{
		msg = "INVALID COMMAND";
		if(send(newSock, msg, strlen(msg), 0) < 0)
			isError("ERROR: Problem sending message to client.");
	}

	if((close(dataSock) == -1) || (close(welcomeSock) == -1)) isError("ERROR: Could not close socket.");

	return true;
}

int main(int argc, char* argv[])
{
	FTPServer ftpS;
	int serverPort;
	int controlSock;
	int sockfd = ftpS.initFTP(serverPort);

	if(argc != 2) isError("Proper Usage: ftserver <port>");

	serverPort = atoi(argv[1]);

	if(serverPort < 1204 || serverPort > 65535)
		isError("Server Port must be between 1024 and 65535");



	while(1) {
		cout << "Server has been opened on port: " << serverPort << endl;
		controlSock = ftpS.openConnect(sockfd, "Control port");

		while(1)
		{
			if(ftpS.handleReq(controlSock, sockfd)) break;
		}
	}

	return 0;
}