
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER 262144 // allow up to 256K
#define DEBUG 1
#define NL NULL

void isError(const char *msg, int exitCode)
{
	printf("%s\n\n", msg);
	exit(exitCode);
}

int main(int argc, char** argv)
{
	char buffer1[BUFFER], buffer2[BUFFER], buffer3[BUFFER];
	int sockfd, newsockfd, filedesc;
	int i, keyLen, pTlen, ciphLen, dataSent, dataRecv, port;
	pid_t pid;
	socklen_t clientLength;

	struct sockaddr_in server;
	struct sockaddr_in client; 

	if(argc < 2)
		isError("usage: opt_dec <port>", 1);

	sscanf(argv[1], "%d", &port);
	if(port < 0 || port > 65535) isError("ERROR: Port must be in range [0..65535]", 2);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0) isError("ERROR: Program could not create socket.", 2);

	// reset the memory address
	memset(&server, '\0', sizeof(server));

	// set up a new address

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
		isError("ERROR: otp_dec_c was unable to bind to socket.", 3);

	if(DEBUG)
	{
		fflush(stdout);
		printf("Socket established and bound on port: %d\n", port);
	}

	if(listen(sockfd, 5) == -1) isError("ERROR: otp_dec_d unable to listen.", 1);

	if(DEBUG)
		printf("Socket is listening on port: %d\n", port);

	clientLength = sizeof(client);

	while(1)
	{
		newsockfd = accept(sockfd, (struct sockaddr*)&client, &clientLength);
	
		if(newsockfd < 0)
		{
			printf("ERROR: otp_dec_c could not accept/establish the connection.\n");
			continue;
		}

		pid = fork();
		if(pid < 0) isError("ERROR: Error on fork in otp_dec_c.", 0);
		if(pid == 0)
		{
			if(DEBUG) printf("otp_dec_d: connection established with client.\n");
	

			memset(buffer1, 0, BUFFER);
			ciphLen=read(newsockfd, buffer1, BUFFER);
			if(ciphLen <0) isError("ERROR: otp_end_d could not read cipher.", 1);

			printf("\n");

			if(DEBUG) printf("otp_dec_d: ciphertext read: %d characters\n", ciphLen);

			dataSent = write(newsockfd, "OK", 1);
			if(dataSent < 0)
			isError("otp_dec_d: ERROR sending to client.", 1);

			if(DEBUG) printf("ACK SEND\n");

			for(i=0; i<ciphLen; i++)
			{
				if((int)buffer1[i] > 90 || ((int)buffer1[i] < 65 && (int)buffer1[i] != 32))
					isError("ERROR: otp_dec_d ciphertext has a bad character", 1);
				if(DEBUG) 
				{
					printf("Checking the Cipher\n");
					for(i=0; i < ciphLen; i++)
					printf("%c", buffer1[i]);
				}
			}

			printf("\n");

			memset(buffer2, 0, BUFFER);
			keyLen=read(newsockfd, buffer2, BUFFER);
			
			if(keyLen<0) isError("ERROR: otp_end_d could not read key file.", 1);


			for(i=0; i<keyLen; i++)
			{
				if((int)buffer2[i] > 90 || ((int)buffer2[i] < 65 && (int)buffer2[i] != 32))
					isError("ERROR: otp_dec_d key has a bad character", 1);
			}

			if(DEBUG)
			{
				printf("Key Buffer 2:\n");				
				for(i=0; i < keyLen; i++)
				{
					printf("%c", buffer2[i]);
				}
			}

			if(DEBUG) printf("keyLen: %d\nciphLen: %d\n", keyLen, ciphLen);

			if(keyLen < ciphLen) isError("Key is too short.", 0);

			for(i=0; i<ciphLen; i++)
			{
				if(buffer1[i] == ' ')
					buffer1[i] == '@';
				if(buffer2[i] == ' ')
					buffer2[i] == '@';

				int msg = (int)buffer1[i];
				printf("%c", buffer1[i]);
				int key = (int)buffer2[i];
				printf("%c", buffer2[i]);

				msg = msg - 64;
				key = key - 64;
				
				int decrypt = msg - key;
				
				if(decrypt < 0)
				{
				
					decrypt = decrypt + 27;
				}

				decrypt = decrypt + 64;

				buffer3[i] = (char)decrypt + 0;
				
				if(buffer3[i] == '@')
					buffer3[i] = ' ';
				

			}	

			if(DEBUG) printf("RES SENT\n");

			dataSent = write(newsockfd, buffer3, ciphLen);
			if(dataSent < ciphLen) isError("ERROR: Cannot write to socket.", 0);


			close(newsockfd);
			close(sockfd);

			exit(0);
		}
		else close(newsockfd);
	}

return 0; 

}