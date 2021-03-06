/*
Programmer: Andrew M. Calhoun
Email: calhouna@oregonstate.edu
Due Date: 2/8/2016

Description: An old school adventure game, akin to Zork or Colossal Cave.

*/

#include <sys/types.h>  // for getpid()
#include <sys/stat.h>   // Creating directories properly
#include <dirent.h>     // for directory listing
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


// Defines are created in case I decide to come back to this assignment in the future for
// portfolio work, or even fun, and increase the complexity of the program. This will make it
// easier for me, since I don't have to run around and track down arrays and indices i.e. room[12] variables when I change them.

#define GAMESIZE 7 // Number of rooms
#define CONN 6      // Connections
#define SM_BUFF 20 // Buffer for 20 characters
#define BUFFER 32  // buffer for 32 characters
#define DESC 128   // buffer for room descriptions if they can be implemented


struct Rooms
{
    char name[SM_BUFF];
    int roomID;
    int totalConnections;
    int connections[CONN]; // each room has up 6 connections inclusive.
    char roomType[SM_BUFF]; // START_ROOM, MID_ROOM, END_ROOM;
    char roomDescription[DESC]; // to see if I can shoe-horn in room descriptions like the games of yore. Currently not working and causes bugs, crashes, and lost connections
                                // but leaving in because I want to get it to work potentially down the line.

};


// Function Prototypes

void createDir(char *dirName, int pid);
void createRooms();                                                       // create the rooms and writes them to file.
void interface(struct Rooms playGame[GAMESIZE]);                          // The gameplay loop, creates the back end of the 'playing' environment.
int connectionCheck(struct Rooms curRoom, struct Rooms connectedRooms);  // Make sure rooms do not loop back into each other
void readFile(struct Rooms game[GAMESIZE], char *dirName);               // Reads the rooms from file and back into the program, built based on https://www.cs.cf.ac.uk/Dave/C/node20.html and http://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
void display(struct Rooms curRoom, struct Rooms game[GAMESIZE]);        // Shows the room/gameplay interface. This is the front end of the 'playing' environment.
void createFile(struct Rooms game[GAMESIZE]);                             // Create the files and place them in the appropriate process ID deliniated directory.


// The Main Function -- this is needed to activate the program.
// It creates the random seed and then calls the various functions required to run the game itself.

int main(void)
{
    srand(time(NULL)); // seed the random generator

    struct Rooms playGame[GAMESIZE];

    //printf("Debugging:... getting PID\n");
    int pid = getpid(); // get the process ID.
    //printf("PID received: %d\n", pid);
    char dirName[50];

    //printf("SIZE OF dirname: %d\n", sizeof(dirName));

    //printf("Creating Directory...\n");
    createDir(dirName, pid);

    createRooms();
    readFile(playGame, dirName);
    interface(playGame);

    return 0;

}

/* Creates the directory where the maze data is stored.
** Function: createDir()
** Description: Creates directories and appends a process name.
** Parameters: the process ID variable, as well as the char *dirName.
** Pre:  Program starts
** Post: Directory to store files is created.
*/

void createDir(char *dirName, int pid)
{
   // printf("Creating directory...\n");
   // char *dirOutput = "calhouna.rooms";

    // Create the directory with input name:
    sprintf(dirName, "calhouna.rooms.%d", pid); // writes the string to the variable, dirName. Originally used snprintf, but found it too complicated and
                                                // clumsy for this program. http://www.cplusplus.com/reference/cstdio/sprintf/ - reference used, as well
                                                // as suggestions from classroom discussion board.
   // printf("Directory Created\n");

    if (mkdir(dirName, 0755) == -1)   // create the directory and make it readable and accessible by the program.
    {
            printf("Unable to create directory. Terminating program.");
            exit(1);
    }

    chdir(dirName); // Enter the directory.

     //  printf("Generating Rooms\n");


}

/* Reads the files stored in the maze directories.
** Function: readFile()
** Description: Processes and reads the files produced when the game is
                run and the files are created.
** Parameters: struct Rooms game[gamesize], char *dirName
** Pre:  Directory and files are created.
** Post: Files are read into the program and passed to the interface() and display() functions
        so you can play the game.
*/

void readFile(struct Rooms game[GAMESIZE], char *dirName)
{
    int i,j,k, idx = 0;
    char roomNames[GAMESIZE][SM_BUFF];  // originally had [DESC] as well, but strncpy it did not like a 3D array.

    chdir(".."); // Make sure we go back to the correct directory

    DIR *dir; // from dirent.h
    struct dirent *entry;


    // Check to ensure the directory exists and is accessible.
    if((dir = opendir(dirName)) != NULL)
    {
        while((entry = readdir(dir)) != NULL)
        {
          if((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..")) == 0)   // Make sure the directory has a . or .. in it.
          {
              continue;
          }
          else
          {
              strncpy(roomNames[idx], entry->d_name, SM_BUFF); // copy the room names to the index and increase the index for seven rooms.
              idx++;
          }
      }
      closedir(dir);
    }


    else
    {
        printf("Cannot open directory. Terminating.\n");    // Error catch, so if there is no directory, end the program.
        exit(1);
    }

    for(i=0; i < GAMESIZE; i++)
    {
        strncpy(game[i].name, roomNames[i], SM_BUFF);
        game[i].totalConnections = 0;
        game[i].roomID = i;
    }

    // After copying the rooms -- for description, try strcmp'ing for rooms (probably would require a switch statement, as more efficient.)
    // To add descriptions accurately. Previous versions of program would add descriptions randomly, rather than by room.

    chdir(dirName);

    FILE *fPtr;             // file pointers are pretty standard parts of file read/write, learned extensively in CS161/162/261.
    char fileName[BUFFER];

    for(i=0; i < GAMESIZE; i++)
    {
        idx=0;
        char string1[BUFFER], string2[BUFFER], string3[BUFFER]; // Create string buffers to hold the various room variables -- name, connections, and room type. Originally left as DESC, but nothing is longer than 32 characters, so changed to BUFFER
        char connNames[CONN][SM_BUFF];

        strncpy(fileName, roomNames[i], SM_BUFF);
        fPtr = fopen(fileName, "r");

        if(fPtr == NULL)
        {
            printf("Eerror opening file. Terminating.\n");
            exit(1);
        }

        while(fscanf(fPtr, "%s %s %s", string1, string2, string3) != EOF)   // Read data from files.
        {

            if(strcmp(string1, "CONNECTION") == 0)
            {
                strncpy(connNames[idx], string3, SM_BUFF);
                idx++;
            }

            if(strcmp(string3, "START_ROOM") == 0)
            {
                strncpy(game[i].roomType, "START_ROOM", SM_BUFF);
            }
            else if(strcmp(string3, "END_ROOM") == 0)
            {
                strncpy(game[i].roomType, "END_ROOM", SM_BUFF);
            }
            else
            {
                strncpy(game[i].roomType, "MID_ROOM", SM_BUFF);
            }
    }

    int numConn = 0;

    for(j=0; j < idx; j++)
    {
        for(k=0; k < GAMESIZE; k++)
        {
            if(strcmp(connNames[j], game[k].name) == 0)
            {
                game[i].connections[numConn] = game[k].roomID;
                game[i].totalConnections++;
                numConn++;

            }
        }
    }

        fclose(fPtr);
    }

}


/*
** Function: connectionCheck()
** Description: Checks to make sure the connections are valid and not returning to the same room.
** Parameters: struct Rooms curRoom, struct Rooms connectedRooms
** Pre:  Game has created rooms and needs the connections to be checked.
** Post: Checks if room connection is valid or non-self-referencing. Returns a true or false value.
*/

int connectionCheck(struct Rooms curRoom, struct Rooms connectedRooms)
{
    int i;

    for(i=0; i < connectedRooms.totalConnections; i++)
    {
        if(connectedRooms.connections[i] == curRoom.roomID)
            return 1;
    }

    return 0;
}


/* Creates Rooms
** Function: createRoom()
** Description: Processes and reads the files produced when the game is
                run and the files are created.
** Parameters: N/A
** Pre:  Directory is created.
** Post: Creates rooms and connections to be passed to files for creation.
*/
void createRooms()
{
    struct Rooms game[GAMESIZE];

    int i, j, k;
    int roomDuplicates; // set default to true
    int pathDuplicate;
    int startRoom;
    int endRoom;
    int differentRoom = 0;
    // printf("Room names being generated....\n");

    char *places[] = { "OutsideHouse", "GrueCave", "MtDoom", "Lothlorien", "LoudRoom", "Fizzbuzz", "Forest", "WeirdRoom", "ZotRoom", "Sluice" };
    // char *descriptions[] = { "You are outside a white house, in an open field. There is a mailbox here.", "It's dark and dank in here. You also feel like you just might get eaten by a grue.", "Smoldering heat and lava are all over the place. Plus a hobbit is fighting with Gollum for a shiny trinket.", "A beautiful Elven holdfast, a delicate and reassuring melody echoes through the air.", "IT'S REALLY LOUD IN HERE! EVERYTHING ECHOES ECHOES ECHOES!!!!", "Oh no! The classic computer science student program! Run!", "Light dapples the ground as the sun shines through pine trees. You hear a song bird in the distance.", "This is a really weird room. In fact, you are dizzy now.", "The holy artifact of Zot floats in the middle of the room.", "You are standing over the great sluice of the Great Underground Empire." };
    char gameRoom[GAMESIZE][BUFFER]; // variable for holding the place and description.
    char roomName[BUFFER];

    //   printf("\n123\n456\n879\n987\n654...\n");

    for(i=0; i < GAMESIZE; i++)
    {
        roomDuplicates = 1;
     //   printf("I: %d\n", i);
        while(roomDuplicates != 0)
        {

            int rdmName = rand() % 10; // pick a name outta the hat
            strncpy(roomName, places[rdmName], SM_BUFF);

              roomDuplicates = 0;
          //  printf("%d\n", rdmName);
            for(j=0; j < GAMESIZE; j++)
            {
                if(strcmp(gameRoom[j], roomName) == 0)
                {
                //    printf("J: %d\n", j);
                //    printf("Duplicate.\n");
                    roomDuplicates = 1;
                }
            }

            if(roomDuplicates != 1)
            {
             //   printf("Tracking error #1\n");
                strncpy(gameRoom[i], roomName, SM_BUFF);
              //  printf("Tracking error #2\n");
                strncpy(game[i].name, roomName, SM_BUFF);
              //  printf("#3 Tracking error\n");
              //  strncpy(game[i].roomDescription, descriptions[i], DESC);
                strncpy(game[i].roomType, "MID_ROOM", SM_BUFF);
              //  printf("#4 Tracking error\n");
                game[i].totalConnections = 0;
              //  printf("#5 Tracking error\n");
                game[i].roomID = i;
            }   //printf("If done.\n");
        } //printf("while loop done\n");
    } //printf("for loop %d times\n", i);

    startRoom = rand() % GAMESIZE;

    strncpy(game[startRoom].roomType, "START_ROOM", SM_BUFF); // set the start room.

    while(differentRoom != 1)
    {
        endRoom = rand() % GAMESIZE;

        if(startRoom != endRoom)
        {
            strncpy(game[endRoom].roomType, "END_ROOM", SM_BUFF);
            differentRoom = 1;
        }
    }
    printf("ROOM CREATION CLEAR!\n");

    /* CONNECTIONS */

    for(i=0; i<GAMESIZE; i++)
    {

        game[i].totalConnections = 3; // There must be at least three rooms connected at any given time.
        int usedRooms[GAMESIZE]; // local array for holding used rooms to prevent recursive connections and duplicates.
        int idx = 0; // shorthand for index.


        for(j=0; j<GAMESIZE; j++)
        {
            usedRooms[j] = 9;
        }

        usedRooms[idx] = i;
        idx++;

        for(j=0; j<game[i].totalConnections; j++)
        {
            int pathOK = 0;         // is the connection okay?
            while (pathOK != 1)
            {
                int route = rand() % GAMESIZE; // indicates a path to another room.
                pathDuplicate = 0; // boolean false

                for(k=0; k <  GAMESIZE; k++)
                {
                    if(route == usedRooms[k])
                    {
                        pathDuplicate = 1;
                    }
                }
                if(pathDuplicate == 0)
                {
                    game[i].connections[j] = route;
                    usedRooms[idx] = route;
                    idx++;
                    pathOK = 1;
                }

            }
        }
    }

    for(i = 0; i < GAMESIZE; i++)
    {
        int j;
        for(j=0; j<game[i].totalConnections; j++)
        {
            int pathCheck = game[i].connections[j]; // Make sure the connections are valid.
            if(connectionCheck(game[i], game[pathCheck]) == 0)
            {
                int pathIdx = game[pathCheck].totalConnections; // check the path indices

                game[pathCheck].connections[pathIdx] = i;
                game[pathCheck].totalConnections++;
            }
        }
    }

    // printf("Connection Creation Cleared. Creating files.\n"); // for debugging purposes.
    createFile(game);

}

/*
** Function: createFile
** Description: Writes the Rooms structures of the maze to a file.
** Parameters: struct Rooms game[GAMESIZE]
** Pre: Game has created rooms and prepared them to be passed into memory for file write.
** Post: Game files are created and passed to designated directory.
*/

void createFile(struct Rooms game[GAMESIZE])
{

    printf("Creating Game Files\n");
    printf("Initializing File Pointer\n");
    FILE *fPtr;

    int k, i, j;

    for(i = 0; i < GAMESIZE; i++)
    {
        char fileName[BUFFER];
        strncpy(fileName, game[i].name, SM_BUFF);
        fPtr = fopen(fileName, "w");
        fprintf(fPtr, "ROOM NAME: %s\n", game[i].name);
        // fprintf(fPtr, "ROOM DESCRIPTION: %s\n", game[i].roomDescription);

        k = 1;
        for(j = 0; j < game[i].totalConnections; j++)
        {
            fprintf(fPtr, "CONNECTION %d: ", k);
            fprintf(fPtr, "%s\n", game[game[i].connections[j]].name);
            k++;
        }

        fprintf(fPtr, "ROOM TYPE: %s\n", game[i].roomType);
        fclose(fPtr);
    }
}

/*
**  Function: display()
**  Description: prints the room and cconnections, as well as the travel prompt.
**  Parameters: the current room structure and the game structure for connections.
**  Pre: Player enters a room, interface sends call.
**  Post: Display the room information.
*/


void display(struct Rooms curRoom, struct Rooms playGame[GAMESIZE])
{
    int i; // loop index

    printf("\nCURRENT LOCATION: %s\n", curRoom.name);
    // printf("ROOM DESCRIPTION: %s\n", curRoom.roomDescription);
    printf("POSSIBLE CONNECTIONS: ");

    for(i=0; i <  curRoom.totalConnections; i++)
    {
        printf("%s", playGame[curRoom.connections[i]].name);

        if(i == curRoom.totalConnections - 1)
        {
            printf(".\n");
        }
        else
        {
            printf(", ");
        }

    }

    printf("WHERE TO? > ");

}

/*
** Function: interface()
** Description: Will display the game, starting with the START_ROOM
**              validate user input, and play the game until the user
**              wins or quits the program.
** Parameters:  The game state array.
** Pre: The urge to play a game.
** Post: You have played a game.
*/

void interface(struct Rooms playGame[GAMESIZE])
{
    int i, j, current, start, end, steps = 0, gameOver = 0;
    char path[100][20]; // array to hold number of turns taken by player.

    printf("\n * * * * * * * * * * * * * * * * * * *   \n");
    printf("*                                        * \n");
    printf("*      WELCOME TO THE INCOMPREHENSIBLE   * \n");
    printf("*               MAZE-THINGY              * \n");
    printf("*                                        * \n");
    printf(" * * * * * * * * * * * * * * * * * * * * \n");


    for(i=0; i < GAMESIZE; i++)
    {
        if(strcmp(playGame[i].roomType, "START_ROOM") == 0)
        {
            start = i;
        }

        if(strcmp(playGame[i].roomType, "END_ROOM") == 0)
        {
            end = i;
        }

    }

    // printf("Starting to Loop\n"); Debugging
    /* MAIN LOOP */

    current = start;
    while(gameOver != 1)
    {
        display(playGame[current], playGame);

        char line[DESC];

        if(fgets(line, sizeof(line), stdin)) // Input the room 'command'
        {
            char *position;
            if((position = strchr(line, '\n')) != NULL)
                *position = '\0';


            int validInput = 0;

            for(j=0; j < playGame[current].totalConnections; j++)
            {
                if(strcmp(line, playGame[playGame[current].connections[j]].name) == 0) // make sure user input is valid
                {
                    validInput = 1; // true
                    current = playGame[current].connections[j];
                    strncpy(path[steps], playGame[current].name, SM_BUFF);
                    steps++;
                    break;
                }
            }

            if(validInput != 1) // invalid means you get a fun TRY AGAIN! message.
            {
                printf("UH, WHAT? THAT'S NOT A VALID PATH OR ROOM! TRY AGAIN!\n");
            }
        }

        printf("\n");

        if(current == end)
        {
            printf("\nCONGRATULATIONS! YOU HAVE ESCAPED MY INCOMPREHENISBLE MAZE!\n");
            printf("YOUR PATH TO VICTORY TOOK %d STEPS! YOUR PATH WAS:\n", steps);
            for(i=0; i < steps; i++)
            {
              printf("%s\n", path[i]);
            }

            gameOver = 1;
        }
    }

}


