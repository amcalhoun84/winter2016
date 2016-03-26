/* Andrew M. Calhoun
calhouna@oregonstate.edu
Due Date: 2/7/2016
Project I
chatclient.c

// Some Patterns are based off of Beej's guide:
// http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleserver

Multithreading based off of:
// http://www.cplusplus.com/forum/unices/116977/

This project includes multithreading, and allows up to 5 connections.
It also allows the user to change their nick on the fly with the \\nick command.

Chat Server Defines and Includes */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

#define MSGBUFFER 500 // Limit to 500 characters.
#define DEBUG 0

// Global Variables - We don't have classes in this program, and while not
// ideal, what we are doing is simple enough.

struct sockaddr_in serverAddress;
struct hostent *server;
char userName[40];

// Prototypes

int openSocket(const char *hostName, int portNum);
void connectSocket(int sockfd);
int rcvMsg(int sockfd);
int sMsg(int sockfd);
void errorMsg(const char *msg);

/*
** Function: errorMsg()
** Description: If there is an error, the program aborts and let's the user know what the error is.
** Parameters: const char *msg
** Pre: An exception or error is hit.
** Post: Program exits and lets the user know what went wrong.
*/

void errorMsg(const char *msg)
{
    printf("Something went wrong.\n");
    printf("%s\n", msg);
    exit(1);
}

/*
** Function: userNameAssign()
** Description: Allows the user to input their name. Also, will be called again if \\nick is used,
    which can be used to change the user's handle.
** Parameters: N/A
** Pre: Successful connection or \\nick is input in an outgoing message.
** Post: Name is assigned to variable userName[40]
*/

char userNameAssign()
{
    int goodName = 0;

    printf("Please enter your user name (one-word) >> ");
    while(goodName != 1)
    {
        fgets(userName, 40, stdin);
        if(strlen(userName) < 40 && strlen(userName) > 0)
        {
            goodName = 1;
            strcat(userName, " >> ");
            size_t length = strlen(userName) - 1;
            if(userName[length] == '\n')
                userName[length] == '\0';

            break;
        }
        else
            printf("Name invalid. Please try again. >> ");
    }
}

/*
Function: openSocket()
Description: Opens and connects the socket. Starts listening.
Parameters: const char *hostName, int portNum
Pre: Program is started and there is a server to connect too.
Post: Socket is opening and tries to attach to the server socket in connectSocket.
*/

int openSocket(const char *hostName, int portNum)
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        errorMsg("There was a problem opening client socket.");

    server = gethostbyname(hostName);
    if(server == NULL)
        errorMsg("No such host.");


    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = PF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(portNum);

    return sockfd;

}

/*
** Function: errorMsg()
** Description: If there is an error, the program aborts and let's the user know what the error is.
** Parameters: const char *msg
** Pre: An exception or error is hit.
** Post: Program exits and lets the user know what went wrong.
*/

void connectSocket(int sockfd)
{
    int status = 0, m;
    // printf("%d\n", status);

    if(connect(sockfd,(struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        errorMsg("Error Connecting to server.");

    m = write(sockfd, "PORTNUM",7);
    if(m < 0)
        errorMsg("Problem writing to socket.");

    userNameAssign();
    // printf("%s", userName);


    while(status != 1)
    {

        // build the message for transmit.

        status = sMsg(sockfd);
        if(status == 1)
        {
            break;
        }
        status = rcvMsg(sockfd);
        if(status == 1)
        {
            break;
        }
    }
    close(sockfd);
}

/*
** Function: sMsg()
** Description: If there is an error, the program aborts and let's the user know what the error is.
** Parameters: int sockfd
** Pre: Requires connection to a server.
** Post: Message Sent to Server
*/

int sMsg(int sockfd)
{

    int m, i = 0, status;
    char buffer[MSGBUFFER];
    char outMessage[MSGBUFFER];
    size_t messageLength;
    char *quit = (char*)malloc(4);
    char *nick = (char*)malloc(4);

        printf("%s", userName);
        bzero(buffer, 500);
        fgets(buffer, 500, stdin);

        // if(DEBUG == 1)
        //    printf("%s\n", buffer);
        //    x = strcmp(buffer, "\\quit");
        //    printf("String Comparison: %d\n", &x);

        messageLength = strlen(outMessage) - 1;
            if(buffer[messageLength] == '\n')
                buffer[messageLength] == '\0';

        bzero(outMessage, 500);
        strcat(outMessage, userName);
        strcat(outMessage, " ");
        strcat(outMessage, buffer);
        printf("Awaiting Server Response...\n\n"); // Just letting the user know that they have to wait.

        // upload the message to the server
        write(sockfd, outMessage, 500);
        if(m < 0)
            errorMsg("Problem writing to client socket.");

        messageLength = strlen(outMessage);
        for(i=0; i < messageLength; i++)
        {
            if(outMessage[i] == '>')
                break;
        }
        i = i + 5;
        if(DEBUG == 1)
        {
            printf("Message Length: %d\n", messageLength);
            printf("String Length: %d\n", i);
        }

        bzero(quit, 5);
        bzero(nick, 5);
        strncpy(quit, outMessage + i, 5);

        if(DEBUG == 1)
        {
            printf("i: %d\n", i);
            int r = strlen(outMessage + i);
            int s = strlen(quit);
            printf("Outmessage Length: %d\n", r);
            printf("Quit Length: %d\n", s);
            printf("Quit message: %s\n", quit);
        }

        strncpy(nick, outMessage + i, 5);
        if(strcmp(quit, "\\quit") == 0)
        {
            printf("Client has sent a quit request. Terminating connection.\n\n");
            status = 1;
            free(quit);
            return status;
        }

        if(strcmp(nick, "\\nick") == 0)
        {
            userName[40] = userNameAssign();
            printf("You have changed your name to %s\n", userName);
            free(nick);
        }

    //free(quit);
    //free(nick);

    return status;
}
/*
** Function: rcvMsg()
** Description: Receives messages from the server and reads them to the chat client.
** Parameters: int sockfd
** Pre: Requires connection to a server.
** Post: Message Sent to Server
*/


int rcvMsg(int sockfd)
{

    int m, status;
    int i;
    char buffer[MSGBUFFER];
    char rcvMsg[MSGBUFFER];
    size_t messageLength;
    char *quit = (char*)malloc(4);
    // char *nick = (char*)malloc(4); only needed in sMsg()

    bzero(buffer, 500);
    bzero(quit, 5);
    m = read(sockfd, buffer, 500);
    if(m < 0)
        errorMsg("Error reading from the socket.");

    strncpy(quit, buffer + 13, 5);

    //if(DEBUG == 1)
    //    printf("%s\n", quit);

    if(strcmp(quit, "\\quit") == 0)
    {
        printf("Server sent quit message! Quiting now...\n");
        status = 1;
        return status;
    }
    printf("%s\n", buffer);
    free(quit);

    return status;


}


int main(int argc, char *argv[])
{
    char hostName[80];
    int sockfd, portNum;

    if(argc < 3)
    {
        printf("$ usage ./%s <hostname> <port>\n\n", argv[0]);
        exit(0);
    }

    sprintf(hostName, "%s", argv[1]);
    portNum = atoi(argv[2]);

    sockfd = openSocket(hostName, portNum);
    connectSocket(sockfd);

    return 0;
}
