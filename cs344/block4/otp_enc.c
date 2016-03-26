/*
 * Author: Andrew M. Calhoun
 * Date: 3/6/2016
 * CS344-400 - OTP
 * Description: The one time pad encryption program.
 */

#include <stdio.h>
#include <stdlib.h>
#include "onetimepad.h"

#define FILENAME "otp_enc"

#define HS 'e'

int main(int argc, char** argv)
{

	if(argc != 4)
   	{
		printClient(FILENAME);
		exit(1);
	}

	//char inputFile = argv[1];
	//char key = argv[2];

	int port = verifyPort(argv[3]);

	char buffer1[BUFFER]; // Input
	char buffer2[BUFFER]; // Key
	char buffer3[BUFFER]; // Output
	char errorMsg[256];   // If anything goes wrong.

	bzero(buffer1, BUFFER);
	bzero(buffer2, BUFFER);
	bzero(buffer3, BUFFER);
	bzero(errorMsg, 256);

	int commands = readInputFile(argv[1], argv[2], buffer1, buffer2);
	if(commands < 0) isError("ERROR: Unable to read input file.");

	commands = validate(buffer1, buffer2);
	switch(commands)
	{
		case -1:
			isError("ERROR: The Key is too short.");
		case -2:
			if(isValid(buffer1) < 0)
			isError("ERROR: Invalid characters in input.");
			if(isValid(buffer2) < 0)
			isError("ERROR: Invalid characters in key.");
		default:
			break;
	}


	int sockfd = clientSocket(port);
	if(sockfd < 0)
		isError("ERROR: Unable to connect on provided port.");

	int hs = cliHS(sockfd, HS);
	if(hs < 0) isError("ERROR: Unable to connect to port.");

	printf("%s\n", buffer1);
	printf("%s\n", buffer2);

	fileEncode(buffer1, buffer2, buffer3);

	printf("%s\n", buffer3);

	int r = toClientFile(sockfd, buffer1, buffer2, buffer3);
	if(r == -1)
		isError("ERROR: Failed to transmit input file.");
	if(r == -2)
		isError("ERROR: Failed to send key file.");
	if(r == -3)
		isError("ERROR: Failed to send output.");

	close(sockfd);

	printf("%s\n", buffer3);

	return 0;
}


