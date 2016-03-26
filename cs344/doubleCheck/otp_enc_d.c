/*
 * Author: Andrew M. Calhoun
 * OTP - CS3440 - 400 otp_enc.c
 * Description: This is the encryption server for the OTP encryption system.
                OTP_ENC_D works as the daemon for otp_enc, and
                will return encryption of a plain text file using a provided key from
                the client.

                usage: otp_enc_d <port#>
 * Date: 3/6/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "otp.h"

#define FLAG 1 // determines that it is an encryption protocol. 1 == encrypt, 0 == decrypt
#define DEBUG 0

int main(int argc, char** argv)
{
    int sockfd, newsockfd, port;
    socklen_t clilen;
    struct sockaddr_in serverAddress; 
    struct sockaddr_in clientAddress;
    int optval;    // allows UNIX to reuse port

    int status;         // process status
    pid_t pid;          // process ids for multi thread suppoert

    int serverType = FLAG;
    int clientType;
    int sendBytes, readBytes;

    long ptSize, keySize;
    char buffer[MAXBUFFER];
    bzero(buffer, sizeof(buffer));

    if(argc != 2)
    {
        serverUsage();
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // printf("sockfd: %d\n", sockfd);

    if(sockfd == -1)
    {
        isError("ERROR: Could not create socket.");
    }

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    port = atoi(argv[1]);
    verifyPort(port);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
            isError("ERROR: Server could not bind to socket.");

    if(DEBUG)
        printf("Bound on port: %d\n", port);

    if(listen(sockfd, 5) == -1)
        isError("ERROR: Server could not listen on provided port.");

    if(DEBUG)
        printf("Listening on port: %d\n", port);

    // loop and accept new clients.
    while(1)
    {
        clilen = sizeof(clientAddress);
        if(DEBUG) printf("Client Length: %d\n", clilen);

        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddress, &clilen);
        if(DEBUG) printf("New Sock File Descriptor: %d\n", newsockfd);

        if(newsockfd == -1) isError("ERROR: Server unable to accept socket binding.");

        // start multi-threading

        pid = fork();

        if(DEBUG) printf("PID: %d\n", pid);

        if(pid==-1)
        {
            close(newsockfd);
            newsockfd = -1;
            printf("Warning: Server failed to fork.\n");
        }

        if(pid==0) // server creates successful child process
        {
            readBytes = recv(newsockfd, &clientType, sizeof(clientType), 0);
            if(readBytes == 0)
                printf("Warning: Unable to accept data from client.\n");

            sendBytes = send(newsockfd, &serverType, sizeof(serverType), 0);
            if(sendBytes == 0)
            {
                printf("Warning: Unable to send data back to client.");
            }

            if(clientType != serverType || serverType != 1 || clientType != 1)
            {
                printf("WARNING: Server rejected client. Incorrect type.\n");
                continue;
            }

            // check to make sure that the client can be reached and send response.

            else
            {
                char response[] = "OK";
                sendBytes = send(newsockfd, response, sizeof(response), 0);
                if(sendBytes < 0)
                {
                    printf("Warning: Unable to send response to client.\n");
                }
            }

            // read data from the client

            readBytes = recv(newsockfd, &ptSize, sizeof(ptSize), 0);

            if(readBytes < 0)
                printf("Warning: Unable to receive plaintext file from the client.\n");

            readBytes = recv(newsockfd, &keySize, sizeof(keySize), 0);
            if(readBytes < 0) printf("Warning: Unable to receive key file from the client.\n");

            if(keySize < ptSize)
            {
                printf("Warning: Key file is too small for the plain text.\n");
                continue;
            }

            else
            {
                char response[] = "key accepted";
                sendBytes = send(newsockfd, response, sizeof(response), 0);
                if(sendBytes < 0) printf("Warning: Unable to transmit response / client did not receive response.\n");
            }

            // file data suite -- reads the plaintext and key, then creates the ciphertext

            char plainText[ptSize];
            bzero(plainText, sizeof(plainText));
            readBytes = recv(newsockfd, plainText, sizeof(plainText), MSG_WAITALL);

            if(readBytes < 0)
            {
                printf("Warning: Did not receive plain text.\n");
            }

            char key[keySize];
            bzero(key, sizeof(key));
            readBytes = recv(newsockfd, key, sizeof(key), MSG_WAITALL);
            if(readBytes < 0) printf("Warning: Did not receive valid key file.\n");

            char cipherText[ptSize];
            bzero(cipherText, sizeof(cipherText));
            encrypt(plainText, key, cipherText);

            sendBytes = send(newsockfd, cipherText, sizeof(cipherText), 0);
            if(sendBytes < 0)
            printf("Warning: Could not send cipher text to client.");

            close(newsockfd);
            newsockfd = -1;

            exit(0); // prevent zombies!
        }


        else
        {
            close(newsockfd);
            newsockfd = -1;
            wait(NULL);
        }
    }

    close(sockfd);

    return 0;
}
