/* Andrew M. Calhoun (calhouna@oregonstate.edu)
** program: opt_enc.c
**
** Descriptoin: This program will take a plain text file and the key created by the 
**  key generator, put it through a socket and then return an encrypted result.
**  Once this is received, it will be sent to stdout. 
**
** 
*/


#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    char buffer1[BUFFER], buffer2[BUFFER], buffer3[1];
    int sockfd, fileDesc, newsockfd;
    int i, keyLen, ciphLen, dataRcv, dataSent, port;
    pid_t pid;
    // socklen_t clientLength;

    struct sockaddr_in serverAddress;
    struct hostent *server;



    if(argc < 4) 
    {
        isError("usage: opt_enc <filename> <key> <port>", 1);
    }

    sscanf(argv[3], "%d", &port);

    if(port < 0 || port > 65535)
    {
        isError("ERROR: Port must be in range [1024..65535]", 2);
    }

    fileDesc = open(argv[1], O_RDONLY);

    if(fileDesc < 0) isError("ERROR: Cannot open plain text file!", 1);

    // read the contents and get the length
    ciphLen = read(fileDesc, buffer1, BUFFER);

    for(i=0; i < ciphLen - 1; i++)
    {
        if((int)buffer1[i] > 90 || ((int)buffer1[i] < 65 && (int)buffer1[i] != 32))
        isError("ERROR: otp_enc detects bad characters in the file.", 1);
    }

    close(fileDesc);

    fileDesc = open(argv[2], O_RDONLY);
    if(fileDesc < 0)
        isError("ERROR: Cannot read key file.\n", 2);

    keyLen = read(fileDesc, buffer2, BUFFER);

    for(i=0; i < keyLen - 1; i++)
    {
        if((int)buffer2[i] > 90 || ((int)buffer2[i] < 65 && (int)buffer2[i] != 32))
            isError("ERROR: Key contains bad characters.", 1);
    }

    close(fileDesc);

    if(DEBUG) printf("keyLen: %d\n ciphLen: %d\n", keyLen, ciphLen);

    if(keyLen < ciphLen)
    {
        isError("ERROR: Key is too short.",1 );
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) isError("EROR: Could not contact otp_dec_d", 2);
    memset(&serverAddress, '\0', sizeof(serverAddress)); // flush the server memory address

    server = gethostbyname("localhost");
    if(server == NL)
    {
        isError("ERROR: Unable to name server, thus not able to connect.", 2);
    }

    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serverAddress.sin_addr.s_addr, 
        server->h_length);
    serverAddress.sin_port = htons(port);

    if(connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
            isError("ERROR: Could not connect to otp_dec_d on selected port.", 1);

    dataSent = write(sockfd, buffer1, ciphLen - 1);
    if(dataSent < ciphLen - 1)
        isError("ERROR: Could not send the cipher text to designated otp_dec_d port", 2);

    memset(buffer3, 0, 1);

    dataRcv = read(sockfd, buffer3, 1);
    if(dataRcv < 0)
    {
        printf("ERROR: No acknowledgment received from the otp_dec_d program.", 2);
    } 

    dataSent = write(sockfd, buffer2, keyLen - 1);

    if(dataSent < keyLen - 1) isError("ERROR: Could not send the key to the decoder daemon.", 2);

    if(DEBUG) printf("otp_dec: key sent. now reading response\n");

    memset(buffer1, 0, BUFFER);

    do { 
        dataRcv = read(sockfd, buffer1, ciphLen - 1);
    } while(dataRcv > 0);


    if(DEBUG) printf("RES RCVD\nBUFFER 1:\n");

    for(i=0; i<ciphLen; i++)
    {
        printf("%c", buffer1[i]);
    }

    if(DEBUG) printf("\nBUFFER2:\n");

    for(i=0; i<ciphLen; i++)
    {
        printf("%c", buffer2[i]);
    }

    if(DEBUG) printf("\nBUFFER3:\n");
    for(i=0; i<ciphLen; i++)
    {
        printf("%c", buffer3[i]);
    }



    printf("\n");

    close(sockfd);

    return 0;
}