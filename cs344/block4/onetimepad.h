/*
** Name: Andrew M. Calhoun
** Date: 03/06/2015
** CS344 - 400 - Assignment4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <time.h>

#define BUFFER 262144 // 256K
#define CP 27		  // Capacity is 27, given there are 27 potential characters
#define NL NULL

// Prototypes

// Utility Functions
void isError(const char *msg);
int isValid(char* string);
int verifyPort(char *inputport);
int validate(char *string1, char *string2);
void removeNewLine(char* newLine);
int convAZ(char letter);
char convNum(int number);
int clientSocket(int port);
int serverSocket(int port);

// Handshake
int cliHS(int sockfd, char hs);
int serHS(int sockfd, char hs);


// Data Transmission Function
int sendData(int sockfd, char *msg);
int recvData(int sockfd, char* msg, int dataRcv);
int sendNum(int sockfd, int num);
int recvNum(int sockfd);
int readInputFile(char *file, char *key, char *input1, char *input2);
int readFile(char *fileName, char *string);
int toClientFile(int sockfd, char *input1, char *input2, char *output);
int toServerFile(int sockfd, int flag);

// Encoding / Decoding
void fileEncode(char *pt, char *key, char *cipher);
void fileDecode(char *cipher, char *key, char *pt);
void printServer(char *cmd);
void printClient(char *cmd);

/* Function Definitions */

void isError(const char *msg)
{
	printf("Something went wrong: %s\n", msg);
	exit(0);
}

int isValid(char *string)
{
	int i;
	for(i=0; i < strlen(string); i++)
	{
		if(convAZ(string[i]) < 0)
			return -1;
	}

	return 0;
}

int verifyPort(char *inputPort)
{
	int port = atoi(inputPort);

	if(port < 0 || port > 65535)
	{
		printf("Port must be [0..65535]\n");
		exit(0);
	}

	return port;

}

int validate(char *string1, char *string2)
{
	int size1 = strlen(string1);
	int size2 = strlen(string2);
	int i;

	if(size1 > size2) return -1;

	for(i = 0; i < strlen(string1); i++)
	{
		if(convAZ(string1[i]) < 0) return -2;

		if(convAZ(string2[i]) < 0) return -2;
	}

	if(isValid(string1) < 0) return -2;
	if(isValid(string2) < 0) return -2;

	return 1;

}

void removeNewLine(char *string)
{
	int last = strlen(string) - 1;
	if(string[last] == '\n') string[last] = '\0';
}

char convNum(int number)
{
	switch(number)
	{
		case 0:
			return 'A';
			break;
		case 1:
			return 'B';
			break;
		case 2:
			return 'C';
			break;
		case 3:
			return 'D';
			break;
		case 4:
			return 'E';
			break;
		case 5:
			return 'F';
			break;
		case 6:
			return 'G';
			break;
		case 7:
			return 'H';
			break;
		case 8:
			return 'I';
			break;
		case 9:
			return 'J';
			break;
		case 10:
			return 'K';
			break;
		case 11:
			return 'L';
			break;
		case 12:
			return 'M';
			break;
		case 13:
			return 'N';
			break;
		case 14:
			return 'O';
			break;
		case 15:
			return 'P';
			break;
		case 16:
			return 'Q';
			break;
		case 17:
			return 'R';
			break;
		case 18:
			return 'S';
			break;
		case 19:
			return 'T';
			break;
		case 20:
			return 'U';
			break;
		case 21:
			return 'V';
			break;
		case 22:
			return 'W';
			break;
		case 23:
			return 'X';
			break;
		case 24:
			return 'Y';
			break;
		case 25:
			return 'Z';
			break;
		case 26:
			return ' ';
			break;
		default:
			return -1;
	}
}

int convAZ(char letter)
{
	switch(letter)
	{
		case 'A':
			return 0;
			break;
		case 'B':
			return 1;
			break;
		case 'C':
			return 2;
			break;
		case 'D':
			return 3;
			break;
		case 'E':
			return 4;
			break;
		case 'F':
			return 5;
			break;
		case 'G':
			return 6;
			break;
		case 'H':
			return 7;
			break;
		case 'I':
			return 8;
			break;
		case 'J':
			return 9;
			break;
		case 'K':
			return 10;
			break;
		case 'L':
			return 11;
			break;
		case 'M':
			return 12;
			break;
		case 'N':
			return 13;
			break;
		case 'O':
			return 14;
			break;
		case 'P':
			return 15;
			break;
		case 'Q':
			return 16;
			break;
		case 'R':
			return 17;
			break;
		case 'S':
			return 18;
			break;
		case 'T':
			return 19;
			break;
		case 'U':
			return 20;
			break;
		case 'V':
			return 21;
			break;
		case 'W':
			return 22;
			break;
		case 'X':
			return 23;
			break;
		case 'Y':
			return 24;
			break;
		case 'Z':
			return 25;
			break;
		case ' ':
			return 26;
			break;
		default:
			return '!';
	}
}

int clientSocket(int port)
{
	// Useful Structs and the socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent *server;
	struct sockaddr_in serverAddress;

	// initiation failure

	if(sockfd < 0) isError("ERROR: Unable to initiate client socket.");

	// naming failure

	server = gethostbyname("localhost");
	if(server == NL) isError("ERROR: Unable to get server name.");

	serverAddress.sin_family = AF_INET;
	bcopy((char * ) server->h_addr, (char *) &serverAddress.sin_addr.s_addr, server->h_length);
	serverAddress.sin_port = htons(port);

	// server failure

	if(connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) isError("ERROR: Could not connect to server.");

	// successfully logged in

	return sockfd;

}

int serverSocket(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(sockfd < 0) isError("Error: Could not initiate server socket.");

	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) isError("ERROR: Could not bind to server socket.");

	if(listen(sockfd, 5) == -1) isError("ERROR: Problem listening for connection on server.");

	return sockfd;

}


// Handshake Function Definitions

int cliHS(int sockfd, char hs)
{
	char rChar;
	int r = write(sockfd, &hs, 1);
	if(r<0) return -1;

	r = read(sockfd, &rChar, 1);
	if(r<0) return -1;
	if(rChar == hs) return 0;
	else return -2;
}

int serHS(int sockfd, char hs)
{
	char buffer;
	int r = read(sockfd, &buffer, 1);
	if(r<0) return -1;

	r = write(sockfd, &hs, 1);

	if(r<0) return -1;
	if(hs == buffer) return 0;
	else return -2;
}

// Data Transmission Definition Functions

int sendData(int sockfd, char *msg)
{
	int r;
	int sendData = strlen(msg+1);
	int total = 0;

	while(total < sendData)
	{
		r = write(sockfd, msg+total, sendData - total);
		total += r;
		printf("TOTAL: %d\n", total);

		if(r < 0) return -1;
		else if(r==0) total = sendData - total;
	}

	return 0;
}


int recvData(int sockfd, char *msg, int dataRcv)
{
	char buffer[BUFFER];
	int r, total = 0;

	while (total < dataRcv)
	{
		r = read(sockfd, buffer+total, dataRcv-total);
		printf("File Size: %d\n", r);
		total += r;

		if(r < 0) return -1;
		else if(r==0) total = dataRcv - total;
	}

	strncpy(msg, buffer, dataRcv);

	return 0;
}


int sendNum(int sockfd, int num)
{
	int r = write(sockfd, &num, sizeof(int));
	if(r<0) return -1;
	else return 0;
}

int recvNum(int sockfd)
{
	unsigned int num;
	int r = read(sockfd, &num, sizeof(int));
	if(r<0) return -1;
	else return num;
}

int readInputFile(char *file1, char *file2, char *input1, char *input2)
{
	int success = readFile(file1, input1); 	// the success boolean
	if(success <0) return -1;
	success = readFile(file2, input2);
	if(success <0) return -2;

	// returned OK
	return 0;
}

int readFile(char *fileName, char *string)
{
	FILE *filePtr;
	filePtr = fopen(fileName, "r");
	if(filePtr == NL) return -1;

	if(fgets(string, BUFFER, filePtr) == NL) return -1;

	removeNewLine(string);
	return 0;
}

int toClientFile(int sockfd, char *input1, char *input2, char *output)
{
	int r = sendNum(sockfd, strlen(input1)+1);
	if(r < 0)
		isError("ERROR: No data to input or file could not be read.");

	r = sendData(sockfd, input1);
	if(r < 0) isError("ERROR: No data could be read or sent to file.");

	r = sendNum(sockfd, strlen(input2)+1);
	if(r < 0) isError("ERROR: No data or could not read key.");

	r = sendData(sockfd, input2);
	if(r<0) isError("ERROR: Could not send data.");

	int data = recvNum(sockfd);
	if(data < 0) isError("ERROR: Could not receive data or file is too small or corrupted.");

	r = recvData(sockfd, output, data);
	if(r < 0) return -3;

	return 0;
}

int toServerFile(int cliSock, int flag)
{
	int data = recvNum(cliSock);
	if(data < 0) isError("ERROR: Unable to receive data.");

	char input1[data];
	bzero(input1, data);

	int r = recvData(cliSock, input1, data);
	if(r<0) isError("ERROR: Unable to receive data on text input.");

    printf("SERVER FILE CHECK #1: %s\n", input1);

    data = recvNum(cliSock);
    if(data < 0)
        isError("BONK");

	char input2[data];
	bzero(input2, data);

	r = recvData(cliSock, input2, data);
	if(r < 0) isError("ERROR: Unable to receive data on key input.");

	printf("SERVER FILE CHECK #2: %s\n", input2);

	char output[strlen(input1)+1];
	bzero(output, strlen(input1)+1);

	// encode or decode the data depending on the buffer flag.
	if(flag == 1)
		fileEncode(input1, input2, output);
	else
		fileDecode(input1, input2, output);

	// send the output size
	r = sendNum(cliSock, strlen(output)+1);
	if(r < 0) isError("ERROR: Could not calculate or send file size.");

	// time to do the output
	r = sendData(cliSock, output);
	if(r < 0) isError("ERROR: Could not send output to socket.");

	// Everything works!
	return 0;
}



// Encoding/Decoding Functions

void fileEncode(char *text, char *key, char *cipher)
{
	int textNum, keyNum, cipherNum, mod, i;
	int length = strlen(text);

	for(i = 0; i < length; i++)
	{
		textNum = convAZ(text[i]);
		keyNum = convAZ(key[i]);

		mod = textNum + keyNum;

		if(mod > CP) mod -= CP;

		mod %= CP;

		cipher[i] = convNum(mod);
	}
}

void fileDecode(char *cipher, char *key, char *text)
{
	int textNum, keyNum, cipherNum, mod, i;
	int length = strlen(cipher);

	for(i=0; i < length; i++)
	{
		cipherNum = convAZ(cipher[i]);
		keyNum = convAZ(key[i]);

		mod = cipherNum - keyNum;

		if(mod < 0) mod += CP;

		mod %= CP;

		text[i] = convNum(mod);
	}
}

void printServer(char *cmd)
{
	printf("usage: %% %s <port#>\n", cmd);
	printf("<port#> should be [0..65535]\n");
}

void printClient(char *cmd)
{
	printf("usage: %% %s <textfile> <key> <port#>\n");
	printf("<text file>, <key>, and <port#> are all required arguments.\n");
	printf("<port#> should be [0..65535]\n");
}

