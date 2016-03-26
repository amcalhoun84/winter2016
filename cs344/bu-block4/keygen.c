#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Prototypes */
int randomNumGen(int min, int max);

int randomNumGen(int min, int max)
{
	int randomNumber;

	randomNumber = rand() % ((max - min) + 1) + min;
	return randomNumber;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	int keyLength, i;
	char keybasis;

	if(argc < 2)
    {
        printf("No length given or key file. Please try again.\n");
        exit(1);
    }

	sscanf(argv[1], "%d", &keyLength);

	if(keyLength < 1)
	{
		printf("That is an invalid key length. Please try again.\n");
		exit(1);
	}

	for(i=0; i < keyLength; i++)
	{
		keybasis = (char) randomNumGen(64, 90);
		// if(keybasis == "[@*,\']") keybasis == '';
		if(keybasis == '@')
			keybasis = ' ';

		printf("%c", keybasis);
	}

	printf("\n");
	return 0;

}
