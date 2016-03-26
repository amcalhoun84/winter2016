/*
** Name: Andrew Michael Calhoun
** Assignment: Block 3, Small Shell - CS344-400
** Date: 2/15/2016
**
** Description: Creating a small, shell-like environment in C to do a variety
** of tasks, such as forking, multitasking/multiprocess management, and more.
** There are three basic commands to use, exit, cd, and status.
*/

#include <unistd.h>  // unix shell commands, exec(), execvp(), fork(), et al.
#include <stdlib.h>	// getenv()
#include <stdio.h>
#include <assert.h>
#include <string.h>	// strcmp(), strtok() -- all string related functions
#include <signal.h>
#include <fcntl.h>  // file control (from Lecture #12)
#include <signal.h> // from CS372, lecture #10
#include <sys/types.h>  // getpid()
#include <sys/wait.h>	// waitpid()
#include <sys/stat.h>

#define DELIMITERS " \t\r\n\a"
#define MAXCHARLIM 2048
#define MAXARGLIMT 512
#define BUFFSIZE 64


// Create a doubly linked list to match up the children and have some flexibility to shut down background processes. 
// based off patterns learned in CS 261
typedef struct bgChild
{
	int bgPID;
	// struct bgChild *sentinel;
	struct bgChild *next;
	// struct bgChild *prev;

}bgChild;

/* PROTOTYPES */
void cd(char *dirName);
pid_t getPID(void);
int cdShell(char **args);
char *readLine(); // reads user input
char **parse(char *userInput, int *numArgs);
int redirectShell(int inFlag, int outFlag);
int launchShell(char **args, int numArgs, int bgProc, int *signalFlg, int *termSgnl, bgChild *head);
int executeShell(char **args, int numArgs, int *exitFlag, int *signalFlg, int *termSgnl, bgChild *head);
char **splitLineShell(char *line);
int shellExit(char **args);
void shellStatus();
void shellBGListInsert(bgChild *pointer, int bgPID);
// int shellHelp(char **args);


/*
** Function: status()
** Description: Checks the status of the process IDs and background process IDs.
** Parameters: None
** Pre/Post: Process IDs exist and we need to be notified about them.
*/

void shellStatus()
{
	int pid, bgStatus;

	// check for background processes. Research based on http://stackoverflow.com/questions/13482416/linux-waitpid-wnohang-and-zombies.
	// For the record? I hate zombies. My programs will NOT be the cause of the zombie apocalypse.

	do
	{
		pid = waitpid(-1, &bgStatus, WNOHANG);

		if(pid > 0) // uh-oh... something died... but will it stay DEAD?!
		{
			printf("Background PID %d has terminated: ", pid);

			if(WIFEXITED(bgStatus)) // Exited normally
			{
				printf("Exited value: %d\n", WEXITSTATUS(bgStatus));
			}
			else if (WIFSIGNALED(bgStatus) != 0) // it was forcefully or otherwise terminated.
			{
				printf("Terminated by signal %d\n", WTERMSIG(bgStatus));
			}
		}
	} while(pid > 0);
		
}


/*
** Function: int cdShell()
** Description: Changes the active directory. 
** Parameters: char **args
** Pre: Input the change of directory.
** Post: Directory changed, whether to name of dir or to level above and root.
** Credit: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
*/ 

int cdShell(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "Expected argument to \'cd\'\n");
	}
	else
	{
		if(chdir(args[1]) != 0)
		{
			printf("Error: Directory never found.\n");
		}
	}
	return 1;
}

/*
** Function: char readLine()
** Description: Function will get the user input and allocate it to the buffer via getline.
** Params: None
*/

char *readLine()
{
	char *buffer = NULL;
	size_t bufferSize = 0;
	getline(&buffer, &bufferSize, stdin);
	return buffer;
}

/* Function: char **parseLine
** Description: Based on stephen brennan's version of the function, it will take, store, and interpret
				the command line arguments and set the variable for the number of arguemnts.
				Credit: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
	Paramaters: char *userInput, int *numArgs -- data is held in the memory, given we have
					multiple processes potentially running.
*/


char **parseLine(char *userInput, int *numArgs)
{
	int bufferSize = BUFFSIZE;
	int pos = 0;
	char *tkn, **tkns = malloc(bufferSize * sizeof(char*));	// token and tokens, 
								// tkn is for individual characters, tokens is for the
								// string. 

	tkn = strtok(userInput, DELIMITERS);	// tokenize the string. Compares whatever is in the input
											// and the delimiters, and tokenizes it versus the delimiters.
											// remove \t or \n or \r or \a

	while(tkn != '\0')	// NULL also works.
	{
		tkns[pos] = tkn;	// sets the tkns[pos] to whatever the character is. Helps recreate the string. 
		pos++;

		if(pos >= bufferSize)
		{
			bufferSize *= 2; 
			tkns = realloc(tkns, bufferSize * sizeof(char*));
		}

		tkn = strtok(NULL, DELIMITERS);
	}

	tkns[pos] = NULL;
	*numArgs = pos;

	return tkns;
}

// LAUNCH SHELL

int launchShell(char **args, int numArgs, int bgProc, int *signalFlg, int *termSgnl, bgChild *head)
{

	pid_t pid, wpid;
	int status;

	pid = fork();
	// Generate the Children

	if(pid == 0)
	{
		// researched sigaction -- http://man7.org/linux/man-pages/man2/sigaction.2.html
		struct sigaction action;
		action.sa_handler = SIG_DFL;
		action.sa_flags = 0;
		sigaction(SIGINT, &action, 0);
		int fd, inFlag = -1, outFlag = -1;

		// Counter for preventing a fault in case of background commands or other processes running.
		int counter = numArgs;
		if(bgProc == 1)
		{
			counter = (numArgs - 1);
		}

		int i;
		for(i=0; i<counter; i++)
		{
			if(strcmp(args[i], "<") == 0)
			{
				inFlag = i;
			}
			else if(strcmp(args[i], ">") == 0)
			{
				outFlag = i;
			}
		}

		if((inFlag > -1) || (outFlag > -1))
		{
			if(redirectShell(inFlag, outFlag) == 0)	// based off of Pipes and Redirection - Lecture 12
			{
				fd = open(args[inFlag + 1], O_RDONLY, 0644); // open file
				if(fd == -1)
				{
					printf("Cannot open %s\n", args[inFlag + 1]);
					exit(1); // exit the child process
				}
				else
				{
					if(dup2(fd, 0) == -1)
					{
						printf("Could not redirect pipe to designated input file.\n");
						exit(1); // exit the child process
					}

					args[inFlag] = NULL;
					close(fd);
				}

			}
			else // perform redirect for output
			{
				fd = open(args[outFlag + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if(fd == -1)
				{
					printf("smallsh: cannot open %s for output.\n", args[outFlag + 1]);
					exit(1); // exit the child process
				}
				else
				{
					if(dup2(fd, 1) == -1)
					{
						printf("Cannot redirect stdout for output\n");
						exit(1); // exit the child process
					}

					args[outFlag] == NULL;
					close(fd);
			
				}			
			}
	}
	
	else if((bgProc == 1) && (inFlag < 0))
	{
		fd = open("/dev/null", O_RDONLY, 0644);

		if(fd == -1)
		{
			printf("smallsh: cannot open \"/dev/null\"\n");
			exit(1); // exit child process
		}
		else
		{
			if(dup2(fd, 0) == -1)
			{
				printf("Could not redirect the input for \"/dev/null\".\n");
				exit(1); // exit child process
			}

			close(fd);
			}

		}

		if(execvp(args[0], args) == -1 ) 
		{
				printf("smallsh: no such file or directory\n");
				exit(1);
		}
	}

	// Error forking

	else if (pid < 0)
	{
		perror("error forking");
		return 1;
	}

	// Parent Processes
	else
	{
		do
		{
			if(bgProc == 1)
			{
				shellBGListInsert(head, pid);
				printf("BG process %d initiated.\n", pid);
				fflush(stdout); // flush the buffer
			}
			else
			{
				wpid = waitpid(pid, &status, WUNTRACED);
			}
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));

		if(WIFEXITED(status))
		{
			*signalFlg = 0;
			return WEXITSTATUS(status);
		}

		else if(WIFSIGNALED(status))
		{
			*signalFlg = 1; 
			*termSgnl = WTERMSIG(status);
			return 1;
		}
	}
}	

// EXECUTE SHELL

int executeShell(char **args, int numArgs, int *exitFlag, int *signalFlg, int *termSgnl, bgChild *head)
{
	int bgProc = 0;
	// Make sure there is nothing else running.
	if((args[0] == NULL) || strcmp(args[0], "#") == 0) return 1; // if command is empty
	
	else if(strcmp(args[0], "cd") == 0)
	{
		if(args[1] == NULL)
		{
			char* homePath;
			homePath = getenv("HOME");

			if(homePath != NULL) chdir(homePath);
		}
	
		else
		{
			if(chdir(args[1]) != 0)
			{
				printf("smallsh: could not find the directory.\n");
				*exitFlag = 1;
			}
		}

	return 1; // continue loop.
	
	}
	
	else if(strcmp(args[0], "status") == 0)	// Check Status
	{
		if(*signalFlg == 0) // Set the sign
		{
			printf("smallsh: exit value: %d\n", *exitFlag);
		}
		else
		{
			printf("smallsh: terminated by signal: %d\n", *termSgnl);
		}
		return 1; // otherwise program will fall through
	}	

	else if(strcmp(args[0], "help") == 0)
	{
		shellHelp(args);
		printf("smallsh: functionality not yet fully implemented.\n");
		return 1;
	}

	else if(strcmp(args[0], "exit") == 0)
	{
		return 0;	// end the loop. 0 = end, 1 = continue, as shStatus is a boolean int
		//exit(0);
	}

	if(strcmp(args[numArgs - 1], "&") == 0)
	{
		bgProc = 1;
		args[numArgs - 1] = NULL;
	}

	*exitFlag = launchShell(args, numArgs, bgProc, signalFlg, termSgnl, head);

	return 1;
}

// SHELL HELP

int shellHelp(char **args)
{
	int i;
	printf("Small Shell Help\n");
	printf("Type program names and arguments, hit enter.\n");
	printf("The following functions are built in:\n");

	printf("cd, exit, status\n\n");
	return 1;
}

// REDIRECT SHELL

int redirectShell(int inFlag, int outFlag)
{
	if((inFlag > -1) && (outFlag > -1))
	{
		if(inFlag < outFlag) return 0;
		else return 1;
	}
	else if((inFlag > -1) && (outFlag == -1)) return 0;
	else return 1;
}

/* Allows user to add or remove process IDs. Working on potentially 
** advanced version that will allow them to kill / terminate speciic ones. 
*/


void shellBGListInsert(bgChild *ptr, int bgPID)
{
	assert(ptr != NULL);

	// ptr->sentinel = (struct bgChild* )malloc(sizeof(struct bgChild));

	// while(ptr->next != ptr->sentinel)
	while(ptr->next != NULL)
	{
		ptr = ptr->next;
		// ptr->prev = ptr->next->prev;
	}

	ptr->next = (bgChild *)malloc(sizeof(bgChild));
	ptr = ptr->next;
	ptr->bgPID = bgPID;
	ptr->next = NULL;

	//ptr->prev = ptr;
	//ptr->next = ptr->sentinel;

}


/*
** Function: int main()
** Description: The primary executive function of the program, contains the loops and calls
** to run the various loops.
** Pre/Post: Execute program and exit.
*/


int main()
{
	bgChild *head, *cur;
	head = (bgChild *)malloc(sizeof(bgChild));
	cur = head; 
	cur->bgPID = -1; // automatically default to non-extant.
	cur->next = NULL;

	char *userInput, **args;
	int numArgs, shStatus, exitFlag = 0, signalFlg = 0, termSgnl;


	struct sigaction action;
	action.sa_flags = 0;
	action.sa_handler = SIG_IGN; // signal ignore
	sigaction(SIGINT, &action, NULL);

	do  // tried with a while loop originally, didn't work out too well. Just went
		// straight back to the command prompt.
	{
		// check for background processes 
		shellStatus();

		printf(": ");
		fflush(stdout); // flush the buffer and prompt, per directions

		// read input
		userInput = readLine();

		args = parseLine(userInput, &numArgs);
		shStatus = executeShell(args, numArgs, &exitFlag, &signalFlg, &termSgnl, head);

		free(userInput);
		free(args);
	

	 } while(shStatus); 

    // kill any background processes to prevent overwrite or orphans.
	cur = head;
	while (cur != NULL)
	{
		if(cur->bgPID != -1)
		{
			kill(cur->bgPID, SIGKILL); // Kill that ZOMBIE
		}

		cur = cur->next;
	}

	while((cur = head) != NULL)
	{
		head = head->next;
		free(cur);
	}

	return 0;
}