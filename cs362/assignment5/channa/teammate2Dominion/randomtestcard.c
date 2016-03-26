/*
 * Jonathan Lagrew
 * CS 362 Winter 2016 Assignment 4
 * 
 * Random testing Geat Hall
 *
 * The Great Hall is an action and victory card. This gives 1 plus card, 1 plus action and 1 Victory Point. 
 */
 
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "rngs.h"
#include <time.h>

#define MAX_TESTS 1000 

int main() {

	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};

	int i;
	int r = 0;
	int choice1 = 0, choice2 = 0, choice3 = 0;
	int numPlayers = 0, curPlayer = 0;
	int handPos = 0, discardCount = 0, numActions = 0;
	int counter = 0;
	int seed;

	srand(time(NULL));

	printf("Testing great_hall.\n\n");//print out to screen which card we are testing 

	for (i = 0; i < MAX_TESTS; i++) 
	{
		struct gameState * G = newGame();//creating gameState G for new games
		//random values to cardEffect arguments
		numPlayers = rand()%5; //gives max of 4 players per game instructions
		if(numPlayers == 0 || numPlayers == 1) numPlayers += 2; //need to have 2-4 players
		curPlayer = 0;//setting current player to 0
		seed = rand();//seed random
		choice1 = rand()%2;
		choice2 = rand()%2;
		choice3 = rand()%2;
		handPos = rand()%4;

		initializeGame(numPlayers, k, seed, G);	//initialize game with random number of players, cards, seed an new game 

		G->deckCount[curPlayer] = rand()%MAX_DECK; //random value for deckcount
		G->discardCount[curPlayer] = rand()%MAX_DECK; //rand value for discard count
		G->handCount[curPlayer] = rand()%MAX_HAND; //random value for hand count

		if (seed%2 == 0) G->deckCount[curPlayer] = 0;//making it to check for empty deck 50% of the time

		discardCount = G->discardCount[curPlayer]; //discard count should increase by 1
		numActions = G->numActions; //number of actions should increase by 1
		//this ensures that we can compare values before card was played and after

		r = cardEffect(great_hall, choice1, choice2, choice3, G, handPos, 0);

		assert(r == 0);	//ensure cardEffect worked okay

		if(discardCount != G->discardCount[curPlayer]-1 && G->discardCount[curPlayer] !=1) //checking discard count 
			{
				counter++;
				printf("discard count = %d\t\tG->discardCount = %d\n", discardCount, G->discardCount[curPlayer]);
			}
		if(numActions != G->numActions-1) //checking number of actions
		{
			printf("\tTest failed: num actions (i = %d)\n", i);
			printf("\t\tG->numActions = %d\t numActions = %d\n\n", G->numActions, numActions);
		}
		free(G);

	}
	printf("**Test complete.\n");

	return 0;
}