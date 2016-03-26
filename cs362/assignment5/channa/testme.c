#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function

    // array of characters from which we select a single one to return
    char array[] = {'@', '!', '[', '~', '(', '^', '{', '*', ' ', '%', '$', 'a', '#', 'x', '}', '&', ')', ']'};
    // number of elements in the array
    int size = sizeof(array)/sizeof(array[0]);
    // randomly select an index
    int index = rand() % size;

    char toReturn = array[index];

    return toReturn;
}

char *inputString()
{
    // TODO: rewrite this function

    // message that will cause the error message to print
    char message[] = "reset";
    int messageLength = strlen(message) + 1;
    
    // array of characters which we will use to construct a string
    char array[] = {'a', 'b', 'c', 'r', 'd', 'e', 'f', 'g', 's', 'e', 't'};
    // number of elements in the array
    int size = sizeof(array)/sizeof(array[0]);
    // length of the string is the full length of the message or shorter
    int stringLength = rand() % messageLength;
    int i, index;

    // allocate memory for string to be returned
    char *string = malloc(sizeof(char) * (stringLength + 1));

    // construct a random string of random length
    for (i = 0; i < stringLength; i++) {
      // randomly select an index
      index = rand() % size;
      string[i] = array[index];
    }

    // string is null-terminated
    string[stringLength] = '\0';

    return string;
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
