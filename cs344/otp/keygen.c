/*
 * Author: Andrew M. Calhoun
 * OTP - CS3440 - 400 keygen.c
 * Description: Generates the keys for the OTP program.
 * Date: 3/6/2016
 */

#include "otp.h"

int main(int argc, char** argv)
{
    srand(time(NL));
    int i, randChar;
    int keyLen = atoi(argv[1]);

    if(argc != 2)   // if the keygen argument is less than 2.
    {
        isError("usage: keygen <key length> > <filename>");
    }

    if(keyLen < 0)
        isError("Key length must be greater than 0");

    // generate a key that is the length given + 1, per the grading script.
    for(i=0; i <= keyLen; i++)
    {
        randChar = rand() % 27 + 'A';
        if(randChar == 26 + 'A') randChar = 32;
        
        printf("%c", randChar);
    }

    // and alternative way to do this, would be to append a '\n' to the string
    // however, that felt like it may result in invalid characters after experimentation
    // so I decided to go with the way I did it. 

    return 0;

}
