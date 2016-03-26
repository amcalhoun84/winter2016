/*
 * Author: Andrew M. Calhoun
 * OTP - CS3440 - 400 otp_enc.c
 * Description: This is the decryption client for the OTP encryption system.
                OTP_DEC will connect to a daemon by the name of otp_dec_d, and
                will request decryption of a cipher text file using a provided key.
                usage: otp_enc <ciphertext> <key> <port#>
 * Date: 3/6/2016
 */

 #include "otp.h"

#define FLAG 0 // determines that it is an decryption protocol. 1 == encrypt, 0 == decrypt
#define DEBUG 0

int main(int argc, char** argv)
{
    int sockfd, port;    
    struct sockaddr_in serverAddress;   // standard server-client socket stuff
    struct hostent *server;             // Learned about in CS372, Project 1 and 2

    struct stat cpStat, keyStat; // let's look at the stats for the cipher text and key.
    long cpSize, keySize;    // turns out one of the files is rather long, so we should use longs instead of ints here.
    int cpfd, keyfd, i;      // the file descriptors and looping index.
    int readText, readKey;   // readText, readKey, used to determine if the key and cipher files are good.

    int clientType = FLAG;   // encode/decode
    int serverType;          // server type boolean
    int sendBytes, readBytes;  // readBytes and sendBytes, send data, return data.

    char buffer[MAXBUFFER];     // for buffer transmission
    bzero(buffer, sizeof(buffer));  // create the memory space

    // determine if there is a sufficient number of arguments
    if(argc != 4)
    {
        clientUsage();          // gives information if used improperly
    }

    port = atoi(argv[3]);       // determine if the port is correct/an integer.

    verifyPort(port);

    stat(argv[1], &cpStat);        // get the cipher length
    cpSize = cpStat.st_size;       
    stat(argv[2], &keyStat);        // get the key length
    keySize = keyStat.st_size;

    if(keySize < cpSize) isError("Key is too short.");
    cpfd = open(argv[1], O_RDONLY);
    if(cpfd == -1) isError("ERROR: Could not open input cipher text file. Please try again or use a different file.");

    char cipherText[cpSize];
    bzero(cipherText, sizeof(cipherText));
    readText = read(cpfd, cipherText, cpSize);
    if(readText < 0) isError("ERROR: Cannot read the cipher text file. Please try again or use a different file.");

    // remove any '\n's -- might not be necessary if already done in file..
    cipherText[cpSize] = '\0';    // swap out an \n for an \0

   // printf("%s\n", cipherText);

    if(verifyInput(cipherText) != 1)
        isError("ERROR: Invalid character in the input text.");

    keyfd = open(argv[2], O_RDONLY);
    if(keyfd == -1) isError("ERROR: Could not open key file. Please try again or use a different key file.");

    char key[keySize];
    bzero(key, sizeof(key));
    readKey = read(keyfd, key, keySize);
    if(readKey < 0) isError("ERROR: Cannot read the key file. Please try again or use a different key file.");

    key[keySize-1] = '\0';
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
    if(clientType != serverType || serverType != 0 || clientType != 0)
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

    sendBytes = send(sockfd, &cpSize, sizeof(cpSize), 0);
    if(sendBytes < 0) isError("ERROR: Could not transmit input file to server.");

    sendBytes = send(sockfd, &keySize, sizeof(keySize), 0);
    if(sendBytes < 0) isError("ERROR: Could not transmit key to server.");

//    bzero(buffer, sizeof(buffer));
//    readBytes = recv(sockfd, buffer, sizeof(buffer), 0);
//    if(readBytes < 0) isError("Key Size Check Not ACK'D");
//
//    if(strcmp(buffer, "key accepted") != 0)
//    {
//        close(sockfd);
//        isError("ERROR: Key too short.");
//    }

    // Now we can begin transmitting data to the server.
    // send ciphertext buffer to server

    sendBytes = send(sockfd, cipherText, sizeof(cipherText), 0);
    if(sendBytes < 0)
        isError("ERROR: Could not transmit plaintext data to server.");


    sendBytes = send(sockfd, key, sizeof(key), 0);
    if(sendBytes < 0)
        isError("ERROR: Could not transmit key data to server.");

    // receive decrypted information from the server.

    char plainText[cpSize];
    bzero(plainText, sizeof(plainText));
    readBytes = recv(sockfd, plainText, sizeof(plainText), MSG_WAITALL); // send a wait signal.

    if(readBytes < 0)
        isError("Could not receive data.");

    // write to the file or prompt.

    for(i =0; i<cpSize; i++)
    {
        printf("%c", plainText[i]);
        close(sockfd);
    }

    return 0;

}
