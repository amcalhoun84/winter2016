/*
 * Author: Andrew M. Calhoun
 * calhouna@oregonstate.edu
 * file: otp.h
 *
 * This file is used to contain all of the common libraries, functions, and other information that will be used across
 * all of the OTP files.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAXBUFFER 262144
#define MSGBUFFER 256
#define NL NULL

// Prototypes:

void isError(char *msg);
void verifyPort(int port);
int verifyInput(char *str);
void encrypt(char *plaintext, char *key, char *cipher);
void decrypt(char *cipher, char *key, char *plaintext);
void serverUsage();
void clientUsage();

// Function definitions

/*
 * Function: void isError()
 * Description: Let's the user know there is an error and exits the program.
 * Only used if there is a fatal error.
 * Parameters: char *msg
 * Precondition: Something went wrong.
 * Postcondition: Brief description of the issue is given and program exits.
 */

 void isError(char *msg)
 {
     fprintf(stderr, "Something went wrong.\n", 0);
     fprintf(stderr, msg, 0);
     fprintf(stderr, "\n");
     exit(1);
 }

/*
 * Function: void verifyPort()
 * Description: If the port is out of range, it will let the user know and what the correct port is.
 * Parameters: int port
 * Pre: An incorrect port is given.
 * Post: A message is delivered explaining which ports to use.
 *
 */

void verifyPort(int port)
{
    if(port < 0 || port > 65355)
    {
        printf("Port number out of range. Please use ports [0..65355]\n");
        exit(0);
    }
}

/* Function: int verifyInput()
 * Description: Makes sure the input, whether plaintext or cipher is good.
 * Params: char *str
 * Pre: Receives a string or character array.
 * Post: Returns 0 if false, returns 1 if good.
 */

int verifyInput(char *str)
{
    int i;
    int length = strlen(str);

    for(i=0; i < length; i++)
    {
        if(str[i] > 90 || (str[i] < 65 && str[i] != 32))
        {
            return 0;
        }
    }

    return 1;
}

/* Function: void encrypt()
 * Description: Encrypt a string buffer fed to the server.
 * Params: char *plaintext, char *key, char *cipher
 * Pre: Receives a string or character array.
 * Post: Reads the plaintext, compares to the key, and returns the cipher.
 */

void encrypt(char *plaintext, char *key, char *cipher)
{
    int i, x, y, z;
    long ptSize = strlen(plaintext);

    char valCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    for(i=0; i < ptSize; i++)
    {
        if(plaintext[i] == ' ')
            x = 26;

        else
            x = plaintext[i] - 65;

        if(key[i] == ' ')
            y = 26;
        else
            y = key[i] - 65;

        z = x + y;
        z %= 27;

        cipher[i] = valCharacters[z];
    }
}

/* Function: void decrypt()
 * Description: Decrypt a string buffer fed to the server.
 * Params: char *cipher, char *key, char *plaintext
 * Pre: Receives a string or character array.
 * Post: Reads the cipher, compares to the key, and returns the plain text.
 */

void decrypt(char *cipher, char *key, char *plaintext)
{
    int i, x, y, z;
    long ciphSize = strlen(cipher);

    char valCharacters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

    for(i=0; i < ciphSize; i++)
    {
        if(cipher[i] == ' ') x = 26;

        else x = cipher[i] - 65;

        if(key[i] == ' ') y = 26;
        else y = key[i] - 65;


        z = x - y;
        if(z<0)
        {
            z+=27;
        }

        z %= 27;
        plaintext[i] = valCharacters[z];
    }
}


// just prints out the instructions for usage of the server.

void serverUsage()
{
   printf("Server usage: otp_enc_d/otp_dec_d <port #>\n");
   exit(0);
}

// just prints out the instructions for usage of the client.

void clientUsage()
{
    printf("Client usage: otp_enc_d/otp_dec_d <filename> <key> <port #>\n");
    exit(0);
}
