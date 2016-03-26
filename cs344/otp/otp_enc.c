/*
 * Author: Andrew M. Calhoun
 * OTP - CS3440 - 400 otp_enc.c
 * Description: This is the encryption client for the OTP encryption system.
                OTP_ENC will connect to a daemon by the name of otp_enc_d, and
                will request encryption of a plain text file using a provided key.
                usage: otp_enc <plaintext> <key> <port#>
 * Date: 3/6/2016
 */

 #include "otp.h"

#define FLAG 1 // determines that it is an encryption protocol. 1 == encrypt, 0 == decrypt
#define DEBUG 0

int main(int argc, char** argv)
{
    int sockfd, port;    
    struct sockaddr_in serverAddress;   // standard server-client socket stuff
    struct hostent *server;             // Learned about in CS372, Project 1 and 2

    struct stat ptStat, keyStat; // let's look at the stats for the plain text and key.
    long ptSize, keySize;    // turns out one of the files is rather long, so we should use longs instead of ints here.
    int ptfd, keyfd, i;     // the file descriptors and looping index
    int readText, readKey;  // readText, keyText, and used to determine if the files are good

    int clientType = FLAG;      // encode/decode, this case, it is encoding 
    int serverType;             // serverType boolean
    int sendBytes, readBytes;   // readBytes and sendBytes, send data, return data.

    char buffer[MAXBUFFER];     // for buffer transmission
    bzero(buffer, sizeof(buffer));  // create the memory space

    // determine if there is a sufficient number of arguments
    if(argc != 4)
    {
        clientUsage();   // gives information if used improperly
    }

    port = atoi(argv[3]); // determine if the port is correct/an i

    verifyPort(port);     

    stat(argv[1], &ptStat);     // get the plaintext length
    ptSize = ptStat.st_size;
    stat(argv[2], &keyStat);    // get the key length
    keySize = keyStat.st_size;

    if(keySize < ptSize) isError("Key is too short.");

    ptfd = open(argv[1], O_RDONLY);
    if(ptfd == -1) isError("ERROR: Could not open input plain text file. Please try again or use a different file.");

    char plainText[ptSize];
    bzero(plainText, sizeof(plainText));
    readText = read(ptfd, plainText, ptSize);
    if(readText < 0) isError("ERROR: Cannot read the plain text file. Please try again or use a different file.");

/* Testing */
//    for(i=0; i < ptSize; i++)
//    {
//        printf("%c", plainText[i]);
//    }
//
//    printf("\n");
//
//    for(i=0; i < ptSize; i++)
//    {
//        printf("%d ", plainText[i]);
//    }
//
//    printf("\n%s\n", plainText);

    // remove any '\n's
    //if(ptSize <= 512 || ptSize >= 10000)
    plainText[ptSize] = '\0';
    //else plainText[ptSize-1] = '\0';

    if(plainText[ptSize-1] == '\n') plainText[ptSize-1] = '\0';

    if(verifyInput(plainText) != 1)
        isError("ERROR: Invalid character in the input text.");

/* Testing */
//    for(i=0; i < ptSize; i++)
//    {
//        printf("%c", plainText[i]);
//    }
//
//    printf("\n");
//
//    for(i=0; i < ptSize; i++)
//    {
//        printf("%d ", plainText[i]);
//    }
//
//    printf("\n%s\n", plainText);

    keyfd = open(argv[2], O_RDONLY);
    if(keyfd == -1) isError("ERROR: Could not open key file. Please try again or use a different key file.");

    char key[keySize];
    bzero(key, sizeof(key));
    readKey = read(keyfd, key, keySize);
    if(readKey < 0) isError("ERROR: Cannot read the key file. Please try again or use a different key file.");

    key[keySize - 1] = '\0';
    if(verifyInput(key) != 1) isError("ERROR: Invalid character in the key file.");

    // create the socket

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) isError("ERROR: Socket creation failed!");

    server = gethostbyname("localhost");
    if(server == NL) isError("ERROR: Could not get or resolve named server.");

    // set up the client socket

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    memcpy(&serverAddress.sin_addr, server->h_addr_list[0], server->h_length);

    // connect to the server

    if(connect(sockfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
    {
        if(DEBUG) printf("Designated Port: %d\n", port);
        isError("ERROR: Could not contact otp_enc daemon on selected port.");
    }

    // check to see if connection can be read and written to server
    sendBytes = send(sockfd, &clientType, sizeof(clientType),0);
    if(sendBytes < 0) isError("ERROR: Could not send to otp_enc_d server.");

    readBytes = recv(sockfd, &serverType, sizeof(serverType), 0);
    if(readBytes < 0)
        isError("ERROR: Could not receive acknowledgment from otp_enc_d server.");

    // if there is a bad connection/client-server match.
    if(clientType != serverType || serverType != 1 || clientType != 1)
    {
        close(sockfd);
        isError("ERROR: Incorrect client to server relationship.");
    }

    // get ACK from the server
    bzero(buffer, sizeof(buffer));
    readBytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if(readBytes < 0) isError("Could not receive data from the server.");

    // if everything is good, the buffer should read OK
    if(strcmp(buffer, "OK") != 0)
    {
        close(sockfd);
        isError("ERROR: Cannot find port.");
    }

    // send data to server

    sendBytes = send(sockfd, &ptSize, sizeof(ptSize), 0);
    if(sendBytes < 0) isError("ERROR: Could not transmit input file to server.");

    sendBytes = send(sockfd, &keySize, sizeof(keySize), 0);
    if(sendBytes < 0) isError("ERROR: Could not transmit key to server.");

    bzero(buffer, sizeof(buffer));
    readBytes = recv(sockfd, buffer, sizeof(buffer), 0);
    if(readBytes < 0) isError("Key Size Check Not ACK'D");

    if(strcmp(buffer, "key accepted") != 0)
    {
        close(sockfd);
        isError("ERROR: Key too short.");
    }

    // Now we can begin transmitting data to the server.
    // send plaintext buffer to server

    sendBytes = send(sockfd, plainText, sizeof(plainText), 0);
    if(sendBytes < 0)
        isError("ERROR: Could not transmit plaintext data to server.");


    sendBytes = send(sockfd, key, sizeof(key), 0);
    if(sendBytes < 0)
        isError("ERROR: Could not transmit key data to server.");

    char cipherText[ptSize];
    bzero(cipherText, sizeof(cipherText));
    readBytes = recv(sockfd, cipherText, sizeof(cipherText), MSG_WAITALL); // send a wait signal.

    if(readBytes < 0)
        isError("Could not receive data.");

   // if(ptSize > 256) cipherText[ptSize] = '\0'; //-- breaks the cipher???
    
    // DEBUGGING: printf("%d\n", ptSize);
    for(i=0; i < ptSize; i++)
    printf("%c", cipherText[i]);
    
    cipherText[ptSize] = 0;

    close(sockfd);

    return 0;

}
