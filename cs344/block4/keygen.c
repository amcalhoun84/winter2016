#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "onetimepad.h"

int randomNumberGen();

int main(int argc, char** argv)
{
	srand(time(NL));
	int i;

	if(argc != 2)
	{
		printf("usage: keygen <key length> > <key file name>\n");
		exit(0);
	}

	if(!atoi(argv[1]))
	{
		printf("Please use integers only for key generation.\n");
		exit(0);
	}

	if(atoi(argv[1]) < 0)
	{
		printf("usage: key must be 1 character or larger.\n");
		exit(0);
	}

	else
	{

		int keyLen = atoi(argv[1]);

		for(i=0; i<keyLen; i++)
		{
			printf("%c", convNum(randomNumberGen()));
		}
		printf("\n");
		
	}
	return 0;
}

int randomNumberGen(){	

		return rand() % 27;
}