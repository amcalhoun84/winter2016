/*
** Name: Andrew Michael Calhoun
** Assignment: Block 3, Small Shell - CS344-400
** Date: 2/15/2016
**
** Description: Creating a small, shell-like environment in C to do a variety
** of tasks, such as forking, multitasking/multiprocess management, and more.
** There are three basic commands to use, exit, cd, and status.
** I successfully finished the program and then curtailed the names of the
** functions and variables as a management challenge. Eventually I hope to try to
** "tiny" code this down to half it's current size.

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

#define DLM " \t\r\n\a"
#define CHRLM 2048
#define ARGLM 512
#define BUFFSIZE 64
#define DBG 0
#define NL NULL


// Create a linked to match up the children and have some flexibility to shut down background processes.
// based off patterns learned in CS 261
typedef struct bgChld {
	int bgPID; // child process ID
	struct bgChld *nxt;

}bgChld;

/* PROTOTYPES */
char *rdLn(); // reads user input
char **parse(char *input, int *numArgs);
int rdctSh(int inFlg, int oFlg);
int lnchSh(char **args, int numArgs, int bgP, int *sglFlg, int *trmSgl, bgChld *head);
int execSh(char **args, int numArgs, int *extFlg, int *sglFlg, int *trmSgl, bgChld *head);
int shExt(char **args);
void shSt();
void shBGLstInsrt(bgChld *ptr, int bgPID);

/*
** Function: status()
** Description: Checks the status of the process IDs and background process IDs.
** Parameters: None
** Pre/Post: Process IDs exist and we need to be notified about them.
*/

void shSt() {
	int pid, bgS; // process id and background stats

	// check for background processes. Research based on http://stackoverflow.com/questions/13482416/linux-waitpid-wnohang-and-zombies.
	// For the record? I hate zombies. My programs will NOT be the cause of the zombie apocalypse.
	do {
		pid = waitpid(-1, &bgS, WNOHANG);
		if(pid > 0) {// uh-oh... something died... but will it stay DEAD?!
    		printf("Background PID %d has terminated: ", pid);
			if(WIFEXITED(bgS)) printf("Exited value: %d\n", WEXITSTATUS(bgS));  // Exited normally
			else if (WIFSIGNALED(bgS) != 0) printf("Terminated by signal %d\n", WTERMSIG(bgS)); // it was forcefully or otherwise terminated.
		}
    }while(pid > 0);
}

/*
** Function: char readLine()
** Description: Function will get the user input and allocate it to the buffer via getline.
** Credit: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
** Params: None
*/

char *rdLn() {
	char *bff = NL;
	size_t bffSz = 0;
	getline(&bff, &bffSz, stdin);
	return bff;
}

/* Function: char **prsLn
** Description: Based on stephen brennan's version of the function, it will take, store, and interpret
				the command line arguments and set the variable for the number of arguemnts.
				Credit: http://stephen-brennan.com/2015/01/16/write-a-shell-in-c/
	Paramaters: char *userInput, int *numArgs -- data is held in the memory, given we have
					multiple processes potentially running.
*/

char **prsLn(char *input, int *numArgs) {
	int bffSz = BUFFSIZE;
	int pos = 0;
	char *tkn, **tkns = malloc(bffSz * sizeof(char*));	// token and tokens,
								// tkn is for individual characters, tokens is for the
								// string.

	tkn = strtok(input, DLM);	// tokenize the string. Compares whatever is in the input
											// and the delimiters, and tokenizes it versus the delimiters.
											// remove \t or \n or \r or \a

	while(tkn!=NL) { // NULL works -- tried '\0' and ran into a few weird problems.
		tkns[pos] = tkn;	// sets the tkns[pos] to whatever the character is. Helps recreate the string.
		pos++;
		if(pos>=bffSz) {
			bffSz*=2;
			tkns = realloc(tkns, bffSz * sizeof(char*));       // If our position overflows the buffer, we will go ahead and enlarge it.

        if(pos>=2147483647)            // Just to prevent stack overflow, given that a recent article came out about DNS overflows being a cause of malware attacks
        {
            printf("System stack overflow, aborting.");
            exit(1);
        }

        }

		tkn = strtok(NL, DLM);
	}
	tkns[pos] = NL;
	*numArgs = pos;
	return tkns;
}

// LAUNCH SHELL
/* FUNCTION: lnchSh
** Description: Launches the not-otherwise-specified/hard coded processes and branches and forks as necessary.
** Allows us to do file inputs and outputs, multiple processes at once, and put processes in the background.
** Parameters: char **args, int numArgs, int *extFlg, int *sglFlg, int *trmSgl, struc bgChld *head
** Pre: The shell is given a command to run, such as cd, status, or exit. Interpreted by the execute shell function.
** Post: Command is run, and either a termination signal is called, or the user is prompted to enter another command.
*/
int lnchSh(char **args, int numArgs, int bgP, int *sglFlg, int *trmSgl, bgChld *head) {
	pid_t pid, wpid;
	int st;
	pid = fork();
	// Generate the Children
	if(pid == 0) {
		// researched sigaction -- http://man7.org/linux/man-pages/man2/sigaction.2.html
		struct sigaction a;
		a.sa_handler = SIG_DFL;
		a.sa_flags = 0;
		sigaction(SIGINT, &a, 0);
		int fd, inFlg = -1, oFlg = -1;
		// Counter for preventing a fault in case of background commands or other processes running.
		int cnt = numArgs;
		if(bgP==1) cnt=(numArgs-1);

		int i;
		for(i=0;i<cnt;i++) {
			if(strcmp(args[i], "<") == 0) inFlg = i;
			else if(strcmp(args[i], ">") == 0) oFlg = i;
		}
		if((inFlg > -1) || (oFlg > -1))	{
			if(rdctSh(inFlg, oFlg) == 0) {	// based off of Pipes and Redirection - Lecture 12
				fd=open(args[inFlg + 1], O_RDONLY, 0644); // open file
				if(fd==-1) {
					printf("Cannot open %s\n", args[inFlg+1]);
					exit(1); // exit the child process
				}
				else {
					if(dup2(fd,0)==-1) {
						printf("Could not redirect pipe to designated input file.\n");
						exit(1); // exit the child process
					}
					args[inFlg] = NL;
					close(fd);
				}
			}
			else { // perform redirect for output
				fd = open(args[oFlg+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if(fd == -1) {
					printf("smallsh: cannot open %s for output.\n", args[oFlg+1]);
					exit(1); // exit the child process
				}
				else {
					if(dup2(fd, 1) == -1) {
						printf("Cannot redirect stdout for output\n");
						exit(1); // exit the child process
					}
					args[oFlg] = NL;    // My oldest foe. I =='d this ans it was driving me nuts, but it's fixed and we are good.
					close(fd);          // closing the file is good general practice.
				}
			}
	}
	else if((bgP==1)&&(inFlg<0)){      // If we have a Background Process, but nowhere to input.
		fd = open("/dev/null", O_RDONLY, 0644);
		if(fd == -1) {
			printf("smallsh: cannot open \"/dev/null\"\n");
			exit(1); // exit child process
		}
		else {
			if(dup2(fd, 0) == -1) {
				printf("Could not redirect the input for \"/dev/null\".\n");
				exit(1); // exit child process
			}
			close(fd);
			}
		}
		if(execvp(args[0], args) == -1 ) {          // If we can't find the file or directory, the whole thing stops.
				printf("smallsh: no such file or directory\n");
				exit(1);
		}
	}
	// Error forking
	else if (pid < 0) {
		perror("error forking");  // perror.
		// return 1;
	}
	// Parent Processes
	else {
		do {
			if(bgP == 1) {
				shBGLstInsrt(head, pid);
				printf("BG process %d initiated.\n", pid);
				fflush(stdout); // flush the buffer
            }
			else wpid = waitpid(pid, &st, WUNTRACED);
		}while(!WIFEXITED(st) && !WIFSIGNALED(st)); // If the program is NOT terminated by signal or exited, we can keep going.
		if(WIFEXITED(st)) {
			*sglFlg = 0;    // if we want to exit, we set the signal flag to 0 to indicate to the program that we want to quit.
			return WEXITSTATUS(st);
		}
		else if(WIFSIGNALED(st)) {
			*sglFlg = 1;
			*trmSgl = WTERMSIG(st); // If there is termination signal, we return 1, because we want to terminate a process..
			return 1;
		}
	} return 1;
}

/* FUNCTION: EXECUTE SHELL
** Description: The processing function, before launching the shell command.
** Parameters: char **args, int numArgs, int *extFlg, int *sglFlg, int *trmSgl, struc bgChld *head
** Pre: The shell is given a command to run, such as cd, status, or exit.
** Post: Passes it to the shell launcher, where it is processed and the command is executed. */
int execSh(char **args, int numArgs, int *extFlg, int *sglFlg, int *trmSgl, bgChld *head) {
	int bgP=0;
	// Make sure there is nothing else running.
	if((args[0]==NL)||strcmp(args[0],"#")==0)return 1; // if command is empty
	else if(strcmp(args[0], "cd") == 0) {
		if(args[1]==NL)	{
			char* hmPth;
			hmPth = getenv("HOME");
			if(hmPth!=NL) chdir(hmPth);
		}
		else {
			if(chdir(args[1]) != 0)	{
				printf("smallsh: could not find the directory.\n");
				*extFlg = 1;
			}
		}
    return 1; // continue loop.
	}
	else if(strcmp(args[0], "status") == 0)	{ // Check Status
		if(*sglFlg == 0) printf("smallsh: exit value: %d\n", *extFlg); // Set the sign
		else printf("smallsh: terminated by signal: %d\n", *trmSgl);
		return 1; // otherwise program will fall through
	}
	else if(strcmp(args[0], "exit") == 0) {
		return 0;	// end the loop. 0 = end, 1 = continue, as shStatus is a boolean int
	}
	if(strcmp(args[numArgs - 1], "&") == 0)	{
		bgP = 1;
		args[numArgs - 1] = NL;
	}
	*extFlg = lnchSh(args, numArgs, bgP, sglFlg, trmSgl, head);
	return 1;
}

/* FUNCTION: rdctSh
** Redirect Shell
** Description: Redirects pipes to the needed process, so that we can have multiple processing running and waiting.
** Parameters: int inFlg, int oFlg
** Pre: There is an input/output command or another process is running/waiting and needs to be reached.
** Post: The process is redirected or called up.
*/

int rdctSh(int inFlg, int oFlg) {
	if((inFlg > -1) && (oFlg > -1)) {
		if(inFlg < oFlg) return 0;
		else return 1;
	}
	else if((inFlg>-1)&&(oFlg==-1)) return 0;
	else return 1;
}

/* Allows user to add or remove process IDs. Working on potentially
** advanced version that will allow them to kill / terminate speciic ones.
*/

// Linked List for processes to make them easier to find and eliminate when they are no longer needed.
// Inputs are the struct bgChld ptr, and the bgPID. When we need to either insert or remove it, we call this
// function to

void shBGLstInsrt(bgChld *ptr, int bgPID) {
	assert(ptr != NL);
    while(ptr->nxt != NL) ptr = ptr->nxt;
	ptr->nxt = (bgChld *)malloc(sizeof(bgChld));
	ptr = ptr->nxt;
	ptr->bgPID = bgPID;
	ptr->nxt = NL;
}

/*
** Function: int main()
** Description: The primary executive function of the program, contains the loops and calls
** to run the various loops.
** Pre/Post: Execute program and exit.
*/

int main() {
	bgChld *head, *cur;
	head = (bgChld *)malloc(sizeof(bgChld));
	cur = head;
	cur->bgPID = -1; // automatically default to non-extant.
	cur->nxt = NL;

	char *input, **args;
	int numArgs, shS, extFlg = 0,
		sglFlg = 0, trmSgl;

	// Signal Handler for SIGINT.
	struct sigaction a;
	a.sa_flags = 0;
	a.sa_handler = SIG_IGN; // signal ignore
	sigaction(SIGINT, &a, NL);

	do { // tried with a while loop originally, didn't work out too well. Just went
		// straight back to the command prompt.
	    // check for background processes
		shSt();
		printf(": ");
		fflush(stdout); // flush the buffer and prompt, per directions

		input = rdLn(); // read input
		args = prsLn(input, &numArgs);
        shS = execSh(args, numArgs, &extFlg, &sglFlg, &trmSgl, head);
		free(input);
		free(args);
	 } while(shS);
    // kill any background processes to prevent overwrite or orphans.
	cur = head;
	while (cur != NL) {
		if(cur->bgPID != -1) {
			kill(cur->bgPID, SIGKILL); // Kill that ZOMBIE
		}
		cur = cur->nxt;
	}
	while((cur = head) != NL) {
		head = head->nxt;
		free(cur);
	}
	return 0;
}
