/*
** Name: Andrew Michael Calhoun
** Date: 2/5/2014
** CS 362 - Assignment 4
** randomtestcard.c
**
** -- ALL FAIL! This is on purpose, as the discard function is not working properly in the village card.
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define TESTCARD "village"

/*
int playVillage(struct gameState *state, int currentPlayer, int handPos)
{
      //+1 Card
      drawCard(currentPlayer, state);

      //+2 Actions
      state->numActions = state->numActions + 2;

      //discard played card from hand
      // discardCard(handPos, currentPlayer, state, 0);
      return 0;
}
*/

int main()
{
	srand(time(NULL));
	int i;
	int numPlayers = 2, thisPlayer = 0, handPos = 0;
	int seed = rand() % 65535;
	int k[10] = { adventurer, smithy, village, great_hall, council_room, salvager, sea_hag, gardens, remodel, ambassador };
	int totalSuccesses, totalFailures, numTests = 1000;

	int successBool; // Assume all tests pass unless otherwise specified.

	totalSuccesses = numTests;

	struct gameState state, testState;

	initializeGame(numPlayers, k, seed, &state);

	totalFailures = 0;

	printf("GENERATING CARD TEST FOR %s\n", TESTCARD);

	for(i = 0; i < numTests; i++)
	{
		successBool = 1;
		printf("\nITERATION %d\n", i+1);


		memcpy(&testState, &state, sizeof(struct gameState));
		state.hand[thisPlayer][0] = rand() % 27;
		state.hand[thisPlayer][1] = rand() % 27;
		state.hand[thisPlayer][2] = rand() % 27;
		state.hand[thisPlayer][3] = rand() % 27;
		state.hand[thisPlayer][4] = rand() % 27;

		state.hand[thisPlayer+1][0] = rand() % 27;
		state.hand[thisPlayer+1][1] = rand() % 27;
		state.hand[thisPlayer+1][2] = rand() % 27;
		state.hand[thisPlayer+1][3] = rand() % 27;
		state.hand[thisPlayer+1][4] = rand() % 27;

		state.deck[thisPlayer][0] = rand() % 27;
		state.deck[thisPlayer][1] = rand() % 27;
		state.deck[thisPlayer][2] = rand() % 27;
		state.deck[thisPlayer][3] = rand() % 27;
		state.deck[thisPlayer][4] = rand() % 27;

		state.deck[thisPlayer+1][0] = rand() % 27;
		state.deck[thisPlayer+1][1] = rand() % 27;
		state.deck[thisPlayer+1][2] = rand() % 27;
		state.deck[thisPlayer+1][3] = rand() % 27;
		state.deck[thisPlayer+1][4] = rand() % 27;

		memcpy(&testState, &state, sizeof(struct gameState));

		//testState.hand[thisPlayer][0] = rand() % 27;
		//testState.hand[thisPlayer][1] = rand() % 27;
		//testState.hand[thisPlayer][2] = rand() % 27;
		//testState.hand[thisPlayer][3] = rand() % 27;
		//testState.hand[thisPlayer][4] = rand() % 27;



		// Make sure we have all of the victory cards are in order.
/*		if(testState.supplyCount[estate] != 8 || testState.supplyCount[duchy] != 8 || testState.supplyCount[province] != 8)
		{
			printf("\nFailed Test: Victory card supplies are incorrect.\n");
			successBool = 0;
		}

		if(testState.supplyCount[copper] != 46 || testState.supplyCount[silver] != 40 || testState.supplyCount[gold] != 30)
		{
			printf("\nFailed Test: Treasure Card supplies are not correct\n");
			successBool = 0;
		}
		if(testState.supplyCount[copper] + testState.supplyCount[silver] + testState.supplyCount[gold] != 116);
		{
			printf("\nFailed Test: Treasure Card Totals Do Not Equal 116\n");
			successBool = 0;
		}
*/
		playVillage(&testState, thisPlayer, handPos);

		if(testState.deckCount[thisPlayer] != state.deckCount[thisPlayer] - 1)
		{
			printf("\nFailed Test: Player did not pick up extra card properly.\n");
			printf("Deck Count: %d, Pre-Deck Count: %d\n", testState.deckCount[thisPlayer], state.deckCount[thisPlayer] - 1);
			totalFailures++;
			successBool = 0;
		}

		if(testState.numActions != state.numActions + 2)
		{
			printf("\nFailed Test: Number of actions did not increase properly for Player 1.\n");
			totalFailures++;
			successBool = 0;
		}

		if(testState.discardCount[thisPlayer] != state.discardCount[thisPlayer] + 1) // should fail
		{
			printf("\nFailed Test: Number of cards discarded did not increase properly for Player 1.\n");
			printf("Deck Count: %d, Pre-Deck Count: %d\n", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + 1);
			totalFailures++;
			successBool = 0;
		}

		endTurn(&testState);

		// Player 2

		thisPlayer = 1;

		playVillage(&testState, thisPlayer, handPos);

		if(testState.deckCount[thisPlayer] != state.deckCount[thisPlayer] - 1)
		{
			printf("\nFailed Test: Player 2 did not pick up extra card properly.\n");
			printf("Deck Count: %d, Pre-Deck Count: %d\n", testState.deckCount[thisPlayer], state.deckCount[thisPlayer] - 1);
			totalFailures++;
			successBool = 0;
		}

		if(testState.numActions != state.numActions + 2)
		{
			printf("\nFailed Test: Number of actions did not increase properly for Player 2.\n");
			totalFailures++;
			successBool = 0;
		}

		if(testState.discardCount[thisPlayer] != state.discardCount[thisPlayer] - 6)
		{
			printf("\nFailed Test: Number of cards discarded did not increase properly for Player 2.\n");
			printf("Deck Count: %d, Pre-Deck Count: %d\n", testState.discardCount[thisPlayer], state.discardCount[thisPlayer] + 1);
			totalFailures++;
			successBool = 0;

		}


	}

	printf("Testing complete for %s.\n", TESTCARD);
	printf("Total Tests: %d\n", numTests);
	printf("Total Successful: %d\n", (numTests * 6) - totalFailures);
	printf("Total Unsuccessful: %d\n", totalFailures);

	return 0;


}
