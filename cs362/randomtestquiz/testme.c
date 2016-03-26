// Andrew M. Calhoun
// calhouna@oregonstate.edu
// Date: 1/24/2016
// QUIZ 2 - Random Testing

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

char inputChar()
{
    char randomLetter[13] = {'[', ']', '(', ')', '{', '}', 'a', ' ', 'x', 'r', 'e', 's', 't'};  // A short hand forced test, with limited randomized parameters.
    char num = rand() % 13;
    return randomLetter[num];

//      Doing the test this way takes forever -- so I replaced it with only the necessary characters to make sure we accomplish it in a reasonable amount of time.
//    char randomLetter;
//    int randomNumber;
//    randomNumber = random() % 2;
//    if(randomNumber == 1)
//        randomLetter = 'A' + (random() % 31);
//    else
//        randomLetter = 'a' + (random() % 30);

    // TODO: rewrite this function

    // return randomLetter;
}

char *inputString()
{

    char *stringBuf = (char *)malloc(6);
    int i;

    for(i=0; i < 5; i++)
    {
        stringBuf[i] = inputChar();
    }

    stringBuf[5] = '\0';

    return stringBuf;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
