#include <stdio.h>
#include <signal.h>
#include "onetimepad.h"

#define FILENAME "otp_enc_d"
#define HS 'e'
#define FLAG 1  // 1 = encode, 0 = decode.

void sigintHandle(int sigNum)
{
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);

}

int main(int argc, char** argv)
{

	signal(SIGCHLD, sigintHandle);
		
	if(argc != 2)
		printServer(FILENAME);

	int port = verifyPort(argv[1]);

	int sockfd = serverSocket(port);
	if(sockfd < 0) isError("ERROR: Could not connect or listen on port.");
	
	printf("Listening now on port: %d\n", port);

	int pid;
	int status;
	
	while(1)
	{
		int cliSock = accept(sockfd, NL, NL);
		if(cliSock < 0)
			isError("Could not connect to client.");

		else {
			
			pid = fork();
			if(pid < 0)
				isError("Could not fork.");

			else if(pid==0)
			{
				int hs = serHS(cliSock, HS);
				if(hs < 0)
				{
					close(cliSock);
					_exit(0);
				}

				int oF = toServerFile(cliSock, FLAG);
				if(oF == -1)
				{
					printf("Error: Failed to receive input.\n");
					close(cliSock);
				}
				else if(oF == -2)
				{
					printf("ERROR: Failed to receive key.\n");
					close(cliSock);
				}
				else if(oF == -3) 
				{
					printf("ERROR: Could Not Send Output\n");
					close(cliSock);
				}
				else
				{
					close(cliSock);
					_exit(0);
				}
			}
			else close(cliSock);
		}
	}
	return 0;
}	
