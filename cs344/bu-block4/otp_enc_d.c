
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
#define DEBUG 0
#define NL NULL

void isError(const char *msg, int exitCode)
{
    printf("%s\n\n", msg);
    exit(exitCode);

}

int main(int argc, char** argv)
{
    char buffer1[BUFFER], buffer2[BUFFER], buffer3[BUFFER];
    int sockfd, newsockfd;
    int i, keyLength, plainTextLength, dataSent, port;
    pid_t pid;
    socklen_t clientLength;

    struct sockaddr_in server;
    struct sockaddr_in client;

    if(argc < 2) isError("usage: opt_enc_d <port>", 1);

    sscanf(argv[1], "%d", &port);

    if(port < 0 || port > 65535)
    {
        isError("ERROR: Port must be in range [0..65535]", 2);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0) isError("ERROR: Program could not create socket.", 1);

    memset(&server, '\0', sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0) isError("ERROR: Could not bind to socket.", 1);

    if(DEBUG)
    {
        fflush(stdout);
        printf("Bound on port: %d\n", port);

    }

    if(listen(sockfd, 5) == -1) isError("ERROR: Problem listening on socket.", 1);

    if(DEBUG)
    {
        printf("Listening on port: %d\n", port);
    }

    clientLength = sizeof(client); // should be 16.
    // assert(clientLength == 16);

// create infinite loop and accept connections.
    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &client, &clientLength);
        if(newsockfd < 0)
        {
            printf("ERROR: Could not accept new connection");
            continue;   // Normally, I would drop this out, but we need to multiprocess...
        }

        pid = fork();

        if(pid < 0) isError("ERROR: error on the fork\n", 0);
        if(pid==0)
        {
            if(DEBUG) printf("ESTABLISHED NEW CHILD CONNECTION\n");

            // Zero out the buffers for each process.
            memset(buffer1, 0, BUFFER);

            plainTextLength = read(newsockfd, buffer1, BUFFER);
            if(plainTextLength < 0) isError("ERROR: Could not read buffer on socket.", 2);

            if(DEBUG) printf("%d CHARACTERS CONTAINED IN FILE.\n", plainTextLength);

            dataSent = write(newsockfd, "K", 1);
            if(dataSent < 0) isError("ERROR: Could not send acknowledgment to client!", 2);
            if(DEBUG) printf("ACK SENT\n");

            memset(buffer2, 0, BUFFER);

            keyLength = read(newsockfd, buffer2, BUFFER);
            if(keyLength<0) isError("ERROR: Could not read buffer into socket.", 2);

            if(DEBUG) printf("ESTABLISHED CONNECTION\nBUFFER1\n");

            for(i = 0; i < plainTextLength; i++)
            {
                if((int)buffer1[i] > 90 || ((int)buffer1[i] < 65 && (int)buffer1[i] != 32)) isError("otp_enc_d found bad characters in the plain text", 1);
                    printf("%c", buffer1[i]);
            }

            printf("\n");

            for(i=0; i < keyLength; i++)
            {
                  if((int)buffer2[i] > 90 || ((int)buffer2[i] < 65 && (int)buffer2[i] != 32)) isError("otp_enc_d found bad key characters", 1);
                    printf("%c", buffer2[i]);
            }

           printf("\n");

           if(keyLength < plainTextLength) isError("ERROR: Key is too short!", 1);

           for(i = 0; i < plainTextLength; i++)
           {

               printf("%d\n", i);

                if(buffer1[i]==' ')
                {
                    buffer1[i]=='@';

                }
                if(buffer2[i]==' ')
                {
                    buffer2[i]=='@';

                }

                int msg = (int)buffer1[i];
                int key = (int)buffer2[i];

                msg -= 64;
                key -= 64;

                int cipher = (msg+key) % 27;
                cipher += 64;

                printf("%d", cipher);

                buffer3[i] = (char)cipher;

                if(buffer3[i] == '@')
                    buffer3[i] == ' ';

                printf("%c", buffer3[i]);
          }

       dataSent = write(sockfd, buffer3, plainTextLength);
       if(dataSent < plainTextLength) isError("...cannot connect to socket.", 2);



        close(newsockfd);
        close(sockfd);

        exit(0);


        }


    else close(newsockfd);
    }


    return 0;
}
