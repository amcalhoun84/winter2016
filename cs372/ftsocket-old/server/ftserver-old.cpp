/* 
* Name: Andrew M. Calhoun
* Project 2 - FTP Socket
* Description: This is the server for an FTP services. Connected
* clients can request or send a file. The FTP control is managed
* over the connection. After closing, a connection with one
* client, it'll listen for another connection until an interrupt
* (Ctrl-C) is sent.

* Sources used: http://stackoverflow.com/questions/10837514/creating-a-c-ftp-server
* 				http://en.cppreference.com/w/cpp/io/c/fscanf
				http://beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html
*/

#include <cassert>
#include <cctype>
#include <clocale>
#include <cstdio>
#include <sstream>
#include <dirent.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>	// great for directory checks and changing directories. Learned about in CS 344 - Program #2
#include <sys/types.h>
#include <fcntl.h>

#define BL 10			 // Queue size for connection requests
#define CXN_ATMPTS 16    // Number of connection requests 
#define PYLD_MAX 512	 // Number of bytes in payload
#define TAG_LEN 8		 // # of byes reserved in tag field
#define NO_ALWD_CHARS 32
#define NL NULL

using namespace std;

/* Global / Out of Class Prototypes */

int isInt(char *str, int *n);
void isError(const char *msg);
void intHandler(int s);

class ftpServer
{
private:
	int s;				// signal integer variable -- for signal interrupts
	char *dNm; 			// directory name
	int *nFiles;		// number of files
	void *bff;			// buffer for files and packets
	int size;		
	int ctrlSock;	
	char *tag, *data;
	int nBytes;
	char *flNm;
	int *dPrt;

public:
	char **lstFls(char *dNm, int *nFiles);
	void rcvAll(int sock, void *bff, int size);
	void rcvPkt(int sock, char *tag, char *data);
	int runCtrlSsn(int ctrlSock, char *cmdTg, int *dPrt, char *flNm);
	int runDtSsn(int ctrlSock, int dSock, char *cmdTg, char *flNm);
	void sndAll(int sock, void *bff, int nBytes);
	void sndPkt(int sock, char *tag, char *data);
	void strtSvr(int port);

};


void intHandler(int s)
{
	int status;

	// credit: http://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event-c

	struct sigaction sIH; // sigIntHandle

	sIH.sa_handler = SIG_DFL;
	status = sigaction(SIGINT, &sIH, NULL);

	if (status == -1) { 
		isError("ERROR: sigaction invalid");
		exit(1);
	}

	cout << "Caught signal: " << s << endl;
	cout << "Server Closed." << endl;
	exit(1);

	status = raise(SIGINT);
	if(status == -1) {
		isError("Raise Error");
	}

}

char **ftpServer::lstFls(char *dNm, int *nFiles)
{
	char **flList;
	DIR *dir;
	struct dirent *entry;
	struct stat info; 

	dir = opendir(dNm);
	if(dir == NL) isError("Unable to open directory.");

	*nFiles = 0;
	flList = NL;
	while ((entry = readdir(dir)) != NL)
	{
		stat(entry->d_name, &info);	
		if(S_ISDIR(info.st_mode)) 
			continue;

	if(flList == NL) 
		flList = new char*;

	assert(flList != NL);
	assert(flList[*nFiles] != NL);
	
	strcpy(flList[*nFiles], entry->d_name);
	(*nFiles)++;

	}

}

void ftpServer::strtSvr(int port)
{
	int servSock;
	unsigned short status; // statuses should never exceed 65,535 honestly...
	struct sigaction sIH;
	struct sockaddr_in svrAdd;

	svrAdd.sin_family = AF_INET; // IPv4
	svrAdd.sin_port = htons(port);
	svrAdd.sin_addr.s_addr = INADDR_ANY; // local host

	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(servSock==-1) isError("Socket Error");

	status = bind(servSock, (struct sockaddr*) &svrAdd, sizeof(svrAdd));
	if(status==-1) isError("Unable to bind to connection");

	status = listen(servSock, BL);
	if(status==-1) isError("Unable to listen for connection");

	sIH.sa_handler = &intHandler;
	sIH.sa_flags = 0;
	sigemptyset(&sIH.sa_mask);
	status = sigaction(SIGINT, &sIH, 0);

	if(status == -1) isError("SigAction Struct Error");

	// FTP SERVER CONTINUES UNTIL TERMINATION CALL / SIG INT
	while(1)
	{
		char *cliIPv4; // bind to dotted decimal address
		char cmdTg[TAG_LEN + 1];
		char flN[PYLD_MAX+1];
		int ctrlSock, dSock, dPort;
		struct hostent *he;
		socklen_t addLn;	// Address Length.
		struct sockaddr_in cliAddr;

		addLn = sizeof(struct sockaddr_in);
		ctrlSock = accept(servSock, (struct sockaddr*) &cliAddr, &addLn);
		if(ctrlSock == -1) isError("Unable to accept connection");

		cliIPv4 = inet_ntoa(cliAddr.sin_addr);
		cout << endl << "ftserver: FTP Server Control Connection Established with " << cliIPv4 << endl;

		status = runCtrlSsn(ctrlSock, cmdTg, &dPort, flN);

		if(status != -1) { 
			int connA; // connection attempts

			dSock = socket(AF_INET, SOCK_STREAM, 0); // maintain ipv4 standards
			if(dSock == -1) isError("Bad Socket Initialization");

			cliAddr.sin_port = htons(dPort);
			connA = 0;

			do {
				status = connect(dSock, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
			} while(status == -1 && connA < CXN_ATMPTS);

			if(status==-1) isError("Error connecting.");

		}

		cout << "ftserver: FTP data connection established with client: " << cliIPv4 << endl;
		
		// transfer file information over FTP
		runDtSsn(ctrlSock, dSock, cmdTg, flN);

		// Wait for client to ack rcvd data
		rcvPkt(ctrlSock, NL, NL);

		status = close(dSock);
		if(status == -1) isError("Cannot properly close socket.");

		cout << "ftserver: FTP connection closed" << endl << endl;

	}

}

int ftpServer::runCtrlSsn(int ctrlSock, char *cmdTg, int *dPrt, char *flNm)
{
	// Data encapsulation and payloads, i = in/rcvd, o = out/sent

	char iData[PYLD_MAX+1];
	char iTag[TAG_LEN+1];
	char oData[PYLD_MAX+1];
	char oTag[TAG_LEN+1];

	cout << "Receiving data ... " << endl;
	rcvPkt(ctrlSock, iTag, iData);
	if(strcmp(iTag, "DPORT") == 0) *dPrt = atoi(iData);

	cout << "Receiving commands..." << endl;
	rcvPkt(ctrlSock, iTag, iData);
	strcpy(cmdTg, iTag);
	strcpy(flNm, iData);

	if(strcmp(iTag, "LIST") != 0 && strcmp(iTag, "GET") != 0) {
		cout << "Transmitting command error..." << endl;
		strcpy(oTag, "Error!");
		strcpy(oData, "Command must be either -l or -g");
		sndPkt(ctrlSock, oTag, oData);
		return -1;
	}

	else {
		cout << "Transmitting data-connection confirmation" << endl;
		strcpy(oTag, "OKAY");
		sndPkt(ctrlSock, oTag, "");
		return 0;
	}


}

int ftpServer::runDtSsn(int ctrlSock, int dSock, char *cmdTg, char *flNm)
{
	int r = 0;		// return values
	char **flList;	// List of filenames
	int numFls;		// number of files in the current directory

	flList = lstFls(".", &numFls);

	if(strcmp(cmdTg, "LIST") == 0) {
		cout << "Transmitting file listing." << endl;
		for(int i=0; i < numFls; i++)
		{
			sndPkt(dSock, "FNAME", flList[i]);
		}

	}


	else if(strcmp(cmdTg, "GET") == 0)
	{
		do { 
			char bff[PYLD_MAX+1];	// file reader
			int rdB;				// read bytes
			int flExs; 				// flag indicating if file exists.
			FILE *iFl;				// reference to input

			flExs=0;
			for(int i=0; i<numFls; i++)
			{
				if(strcmp(flNm, flList[i]) == 0) flExs=1; 
				
			}
			
			if(!flExs)
			{
				printf("File transmission error\n");
				sndPkt(ctrlSock, "ERROR", "File or directory not found.\n");
				r=-1; break;
			}

			iFl = fopen(flNm, "r");
			if(iFl==NL) {
				printf("Cannot transmit - file read/access error...\n");
				sndPkt(ctrlSock, "ERROR", "Unable to open file.\n");
				r=-1; break;
			}

			sndPkt(dSock, "FILE", flNm);

			printf("Transmitting file\n");
			do
			{
				rdB = fread(bff, sizeof(char), PYLD_MAX, iFl);
				bff[rdB] = '\0';
				sndPkt(dSock, "FILE", bff);
			}while(rdB>0);
			if(ferror(iFl)) { 
				//perror("fread");
				r=-1;
			}
			fclose(iFl);

		} while(0);
	}

	else {
		printf("ftserver: command tag must be -g or -l. Received %s as command.", cmdTg);
		r=-1;

	}

	sndPkt(dSock, "DONE", "");

	printf("Transmitting connection termination.\n");
	sndPkt(ctrlSock, "CLOSE", "");


}

void ftpServer::rcvPkt(int sock, char *tg, char *data)
{
	unsigned short pkLn; // 65,535 is the limit of packet lengths. Int is too large.
	unsigned short dLn; 
	char tmpTg[TAG_LEN+1], tmpD[PYLD_MAX+1]; // Transfer buffer and payload buffer

	rcvAll(sock, &pkLn, sizeof(pkLn));
	pkLn = ntohs(pkLn);

	rcvAll(sock, tg, TAG_LEN);
	tg[TAG_LEN] = '\0';
	if(tg != NL) strcpy(tg, tmpTg);

	dLn = (pkLn - TAG_LEN - sizeof(pkLn));
	rcvAll(sock, tmpD, dLn);
	tmpD[dLn] = '\0';
	if(data!=NL) strcpy(data, tmpD);
}

void ftpServer::rcvAll(int sock, void *bff, int size)
{
	int r=0, rcvB=0;	// return value, recieved bytes.
	while(rcvB < nBytes)
	{
		r=recv(sock, bff+rcvB, nBytes-rcvB, 0);

		if(r==-1) isError("Recieve All Error");

	}

}

void ftpServer::sndAll(int sock, void *bff, int nBytes)
{
	int r=0, sntB=0; // return value for send and number of bytes

	while(sntB < nBytes)
	{
		r=send(sock, bff+sntB, nBytes - sntB, 0);
		if(r==-1) isError("Problem sending.");
		else sntB += r;
	}


}

void ftpServer::sndPkt(int sock, char *tag, char *data)
{
	unsigned short pkLn; // 65,535 is the limit of packet lengths. Int is too large.
	char tBff[TAG_LEN];

	pkLn = ntohs(sizeof(pkLn) + TAG_LEN + strlen(data));
	sndAll(sock, &pkLn, sizeof(pkLn));

	strcpy(tBff, tag);
	sndAll(sock, tBff, TAG_LEN);
	sndAll(sock, data, strlen(data));


}

int main(int argc, char **argv)
{

	int port;
	ftpServer ftp;

	if(argc != 2) {
		isError("usage: ftserver <port>");
	}

	if(!isInt(argv[1], &port)) {
		isError("Invalid input, port must be integer.");
	}

	if(port < 1024 || port > 65535) {
		isError("Port number must be in range 1024-65535.");
	}

	ftp.strtSvr(port);
	
	return 0;
}

int isInt(char *str, int *n)
{
	char c;
	int mth = sscanf(str, "%d %c", n, &c);
	return mth;

}

void isError(const char *msg)
{
	cout << "Something went wrong." << endl;
	cout << msg << endl;
	exit(1);
}