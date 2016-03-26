
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string>
#include <netdb.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

// Define Buffer Limits

#define HOST 1024
#define BUFFER 504
#define RESPONSE 128
#define BACKLOG 5 	// Queueing limit
#define NL NULL
#define DEBUG 1

using namespace std;

class FTPServer
{
private:

	bool fileExists;
	bool dirExists;

	unsigned short dataPort;
	unsigned short serverPort;
	int sockfd;
	int filefd;
	int controlsockfd;
	int datasockfd;
	int newsockfd;
	int fileLength;

	// Commands, CPP gives us the flexibility of true strings.
	char *fileName;
	string commandTag;
	string hostName;

public:
	int initializeServer(unsigned short serverPort);
	int acceptConnection(int sockfd, string &hostName, unsigned short serverPort);
	void listDirectory(int newsockfd);
	void sendFile(int filefd, int newsockfd, int fileLength);
	void fileTransfer(int newsockfd, char *fileName, string hostName, unsigned short dataPort);
	void receiveCmd(int newsockfd);

};

class FTPHelpers : public FTPServer
{
private:
	string errorMsg;
	string userName;
	string password;

public:
	void isError(string errorMsg);
	int authenticate(string userName, string password);
};

void FTPHelpers::isError(string errorMsg)
{
	cout << "Something went wrong" << endl;
	cout << errorMsg << endl;
	exit(0);
}


int FTPServer::initializeServer(unsigned short serverPort)
{
	FTPHelpers ftH;
	int optval;
	struct sockaddr_in serverAdd;

	if(DEBUG)
		cout << "If you see this, the test is working for now in initializeServer." << endl;

	bzero((char *) &serverAdd, sizeof(serverAdd));
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_family = INADDR_ANY;
	serverAdd.sin_family = htons(serverPort);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		ftH.isError("ERROR: Could not open socket.");
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		ftH.isError("ERROR: Could not set reuse on this particular port.");
	}
	if(bind(sockfd, (struct sockaddr*) &serverAdd, sizeof(serverAdd)) < 0)
	{
		ftH.isError("ERROR: Could not bind to socket.");
	}

	return sockfd;

}

int FTPServer::acceptConnection(int sockfd, string &hostName, unsigned short serverPort)
{
	if(DEBUG)
		cout << "If you see this, the test is working for now in acceptConnection." << endl;

	char hostName_string[HOST];
	memset(hostName_string, '\0', HOST);
	struct sockaddr_in client;

	if(DEBUG)
		cout << "Client Struct Initialized" << endl;


	socklen_t clientLength = sizeof(client);

	if(DEBUG)
		cout << "Client Length Initialized: " << clientLength << endl;
		cout << "Waiting to accept connection..." << endl;


	newsockfd = accept(sockfd, (struct sockaddr*) &client, &clientLength);

	if(DEBUG)
		cout << "Client Connection Established: " << clientLength << endl;

	getnameinfo((struct sockaddr*)&client, sizeof(client), hostName_string, sizeof(hostName_string), NL, 0, 0);
	hostName = string(hostName_string); // force the char to become string
	
	cout << "Connection accepted from " << hostName << " accepted on " << sockfd << " " << "port: " << serverPort << endl;

	return newsockfd;

}

void FTPServer::listDirectory(int newsockfd)
{
	if(DEBUG)
		cout << "If you see this, the test is working for now in receiveCmd." << endl;

	char buffer[BUFFER];
	DIR *curDir;
	struct dirent *current;
	memset(buffer, '\0', BUFFER);

	curDir = opendir(".");
	if(curDir==NL)
	{
		cout << "WARNING: Could not read directory." << endl;
		return; 
	}
	while (current = readdir(curDir))
	{
		if(current==NL)
		{
			cout << "WARNING: Current directory is not properly initialized." << endl;
			return;
		}

		read(newsockfd, buffer, BUFFER);
		memset(buffer, '\0', BUFFER);
		strcpy(buffer, current->d_name);
		write(newsockfd, buffer, strlen(buffer));
	}

	cout << "Command recieved on: " << newsockfd << endl;
}

void FTPServer::sendFile(int filefd, int newsockfd, int fileLength)
{
	int byteRead, byteWrite;
	char buffer[BUFFER];
	char response[RESPONSE];

	memset(response, '\0', RESPONSE);
	sprintf(response, "%d", fileLength);
	write(newsockfd, response, strlen(response));
	memset(response, '\0', RESPONSE);
	read(newsockfd, response, RESPONSE);

	memset(buffer, '\0', BUFFER);
	while(1) {
		byteRead = read(filefd, buffer, BUFFER);
		if(byteRead==0) break;
		for(int i=0; i<byteRead; i+=byteWrite)
		{
			byteWrite = write(newsockfd, buffer+i, byteRead-i);
			if(byteWrite<0)
			{
				cout << "ERROR: There was a problem during file transfer." << endl;
				memset(response, '\0', 100);
				strcpy(response, "ERROR: Problem writing to socket.");
				write(newsockfd, response, strlen(buffer));
				return;
			}
		}
	}

	read(newsockfd, response, RESPONSE);
	memset(response, '\0', 100);
	strcpy(response, "COMPLETE");
	write(newsockfd, response, strlen(response));
	close(filefd);
}

void FTPServer::receiveCmd(int newsockfd)
{
	FTPHelpers ftH;

	char buffer[BUFFER];
	char command[3];
	char dataPort_string[6];
	char fileName[BUFFER];
	string otherHost;

	memset(buffer, '\0', BUFFER);
	memset(command, '\0', 3);
	memset(dataPort_string, '\0', 6);
	memset(fileName, '\0', BUFFER);


	read(newsockfd, buffer, BUFFER);
	if(strcmp(buffer, "-g") == 0) strcpy(command, buffer);
	else if(strcmp(buffer, "-l") == 0) strcpy(command, buffer);
	else
	{
		cout << "Invalid command: " << command << endl;
		strcpy(buffer, "Invalid command: ");
		strcat(buffer,  command);
		write(newsockfd, buffer, strlen(buffer));
		return;
	}

	memset(buffer, '\0', BUFFER);
	strcpy(buffer, "OK");
	write(newsockfd, buffer, strlen(buffer));
	memset(buffer, '\0', BUFFER);
	read(newsockfd, buffer, BUFFER);
	strcpy(dataPort_string, buffer);
	dataPort = atoi(dataPort_string);

	datasockfd = initializeServer(dataPort);
	if(datasockfd < 0) ftH.isError("ERROR: Failed to initialize data port socket.");

	listen(datasockfd, 5);
	while(1)
	{
		memset(buffer, '\0', BUFFER);
		strcpy(buffer, "OK");
		write(newsockfd, buffer, strlen(buffer));
		newsockfd = acceptConnection(datasockfd, otherHost, dataPort);
		if(newsockfd < 0)
		{
			cout << "ERROR: Could not accept new socket." << endl;
			return;
		}

		if(strcmp(command, "-l") == 0){
			cout << "List directory requested on " << dataPort << endl;
			cout << "Sending directory contents to " << otherHost << ":" << dataPort << endl;
			listDirectory(newsockfd);
		}
		else if(strcmp(command, "-g") == 0){
			read(newsockfd, fileName, BUFFER);
			cout << "File \'" << fileName << "\' requested on port " << dataPort << endl;
			fileTransfer(newsockfd, fileName, otherHost, dataPort);
		}

		close(newsockfd);
		close(datasockfd);
		break;

	}


}

void FTPServer::fileTransfer(int datasockfd, char *fileName, string hostName, unsigned short dataPort)
{
	int textfd, textLength;
	char buffer[BUFFER];
	DIR *curDir;
	struct dirent *current;

	curDir = opendir(".");
	fileExists=0;
	if(curDir==NL) 
	{
		cout << "ERROR: Current directory did not initialize properly." << endl;
	}
	while(current = readdir(curDir))
	{
		if(current==NL)
		{
			cout << "ERROR: current not initialized properly." << endl;
		}
		if(strcmp(fileName, current->d_name) == 0)
		{
			fileExists=1;
			break;
		}
	}
	if(fileExists)
	{
		textfd = open(fileName, O_RDONLY);
		if(textfd<0)
		{
			cout << fileName << " cannot be opened." << endl;
			memset(buffer, '\0', BUFFER);
			strcpy(buffer, "CANNOT OPEN FILE");
			write(newsockfd, buffer, strlen(buffer));
			return;
		}

		textLength = lseek(textfd, 0, SEEK_END);
		lseek(textfd, 0, SEEK_SET);
		cout << "Sending file." << endl;
		memset(buffer, '\0', BUFFER);
		strcpy(buffer, "SENDING FILE");
		write(newsockfd, buffer, strlen(buffer));
		memset(buffer, '\0', BUFFER);
		read(datasockfd, buffer, BUFFER);
		if(strcmp(buffer, "OK") == 0)
			sendFile(textfd, newsockfd, textLength);
		else
			cout << "File transfer was aborted." << endl;
		}
	else
	{
		cout << " FILE NOT FOUND " << endl;
	}

}

int main(int argc, char *argv[])
{
	int serverPort;
	int sockfd, newsockfd;
	string hostName;

	FTPServer ftpS;
	FTPHelpers ftH;

	char hostNameInput[HOST];
	memset(hostNameInput, '\0', 1024); // May not be necessary as memory is handled better in C++, but having in case of random seg faults occur.
	gethostname(hostNameInput, 1023);

	if (argc != 2)
	{
		ftH.isError("Usage: ftserver <port>");
	}

	serverPort = atoi(argv[1]);

	if(serverPort < 1024 || serverPort > 65535) ftH.isError("ERROR: Server Port must be in range [1024..65535]");

	sockfd = ftpS.initializeServer(serverPort);
	if(sockfd < 0) ftH.isError("ERROR: Failed to initialize server socket. Please check parameters or README.TXT and try again.");
	else cout << "Server is open on port: " << serverPort << endl;

	listen(sockfd, 5);
	while(1) { 
			newsockfd = ftpS.acceptConnection(sockfd, hostName, serverPort);
			if(newsockfd < 0) ftH.isError("ERROR: Could not accept new socket!");
		
			else
			{
				cout << "Connection from " << hostName << " initialized!" << endl;
				ftpS.receiveCmd(newsockfd);
				close(newsockfd);
			}
			if(DEBUG)
				break;			
		}


	close(sockfd);
	exit(EXIT_SUCCESS);
}