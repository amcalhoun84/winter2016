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
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    char buffer1[BUFFER], buffer2[BUFFER], buffer3[1];
    int sockfd, fileDesc, newsockfd;
    int i, keyLen, pTLength, dataRcv, dataSent, port;
    pid_t pid;
    // socklen_t clientLength;

    struct sockaddr_in serverAddress;
    struct hostent *serverName;



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
    pTLength = read(fileDesc, buffer1, BUFFER);

    if(DEBUG)
        printf("Plain Text Length: %d\n", pTLength);

    for(i=0; i < pTLength; i++)
    {
        if((int)buffer1[i] > 90 || ((int)buffer1[i] < 65 && (int)(buffer1[i]) != 32))
        isError("ERROR: otp_enc detects bad characters in the file.", 1);

        printf("%c", buffer1[i]);

    }

    close(fileDesc);

    fileDesc = open(argv[2], O_RDONLY);

    if(fileDesc < 0)
        isError("ERROR: Cannot open the key file.", 1);

    keyLen = read(fileDesc, buffer2, BUFFER);

    if(DEBUG)printf("Key Length: %d\n", keyLen);

    for(i=0; i < keyLen - 1; i++)
    {
        if((int)buffer2[i] > 90 || ((int)buffer2[i] < 65 && (int)buffer2[i] != 32))
            isError("ERROR: Bad characters in the key file.", 2);
    }

    close(fileDesc);

    if(keyLen < pTLength)
        isError("ERROR: Key is too short!!!", 1);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0) isError("ERROR: Program could not create socket.", 1);

    memset(&serverAddress, '\0', sizeof(serverAddress));

    serverName = gethostbyname("localhost");
    if(serverName == NL)
    {
        isError("ERROR: Could not find named server.", 1);
    }

    serverAddress.sin_family = AF_INET;
    bcopy((char *)serverName->h_addr, (char *)&serverAddress.sin_addr.s_addr, serverName->h_length);
    serverAddress.sin_port = htons(port);

    if(connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) isError("ERROR: Could not connect to socket.", 1);

    if(DEBUG)
    {
        printf("Bound on port: %d\n", port);

    }

    dataSent = write(sockfd, buffer1, pTLength - 1);
    if(DEBUG) printf("Data Sent: %d\n", dataSent);

    if(dataSent < pTLength - 1) isError("ERROR: Could not send data to otp encoding daemon.", 2);

    if(DEBUG)
    {
        printf("Sending %d characters!", dataSent);
    }

    memset(buffer3, 0, 1);
    //assert(sizeof(buffer3) == 1);

    dataRcv = read(sockfd, buffer3, 1);
    if(dataRcv < 0) isError("ERROR: Did not receive acknowledgment from the encoder.", 2);

    if(DEBUG)
    {
        printf("ACK RCVD\n");
    }

    dataSent = write(sockfd, buffer2, keyLen - 1);
    if(dataSent < keyLen - 1) isError("ERROR: Problem sending key data.", 1);

    if(DEBUG)
        printf("Key sent. Awaiting response...\n");

    memset(buffer1, 0, BUFFER);

    do
    {
        dataRcv = read(sockfd, buffer1, pTLength);

    }while(dataRcv > 0);

    if(dataRcv < 0) isError("ERROR: Problem receiving cipher data.", 1);
    if(DEBUG) printf("DATA RCV'D\n");

     for(i = 0; i < pTLength; i++)
    {
        printf("%c", buffer1[i]);
    }


    printf("\n");

    close(sockfd);


return 0;


}
