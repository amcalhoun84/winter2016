// Andrew M. Calhoun
// CARD TEST - ASSIGNMENT 3
// cardTest3.c
// Unit Test for card function - village.

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>

#define DEBUG 0
#define NOISY_TEST 1
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
	int i, thisPlayer, seed = 65535, numPlayer = 2, kCardCount, allTestsPassed = 1;
	int k[10] = {adventurer, smithy, great_hall, council_room, salvager, gardens, mine, remodel, village, ambassador };
	struct gameState state, testState;


   // memset(&state, 0, sizeof(struct gameState)); // Ensure we have a clean slate.
    initializeGame(numPlayer, k, seed, &testState);

    memcpy(&state, &testState, sizeof(struct gameState));


    for(i=0; i < 10; i++)
    {
        testState.deck[thisPlayer][i] = rand() % 27;
    }


    for(i=0; i < 10; i++)
    {
        printf("Card: %d\n", testState.deck[thisPlayer][i]);

    }

    thisPlayer = 0;



    printf("TEST #1: Assert that each victory card has 8 cards in the supply, if not, abort program.\nPotential issue is base game is modified. Please reload base files if game has been altered.\n\n");
	assert(testState.supplyCount[estate] == 8);
	assert(testState.supplyCount[duchy] == 8);
	assert(testState.supplyCount[province] == 8);

	// Make sure kingdom cards are properly supplied.
	for(i = 0; i < 10; i++)
	{
		kCardCount = testState.supplyCount[k[i]];
		printf("k[%d] count: %d\n", i, kCardCount);
		// assert(kCardCount = testState.supplyCount[k[i]]);
		if(kCardCount != testState.supplyCount[k[i]])
        {
            allTestsPassed = 0;
        }
	}

	if(allTestsPassed)
        printf("Kingdom Card Supplies All Correct\n");
    else
        printf("Incorrect number of kingdom cards. Please correct or reload base game files.\n");


    playVillage(&testState, thisPlayer, 0);

	printf("TEST #2: Testing Num Action and Card Draw Increases...\n");
	if(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + 1)
    {
        printf("TEST #2: Cards Drawn Properly Increased.\n");
    }
	// assert(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + 1);
	else
    {
        printf("TEST #2: Cards Drawn Not Properly Increased.\n");
        printf("Expected: %d\n", state.handCount[thisPlayer] +1);
        printf("Result: %d\n\n", testState.handCount[thisPlayer]);
        allTestsPassed = 0;
    }

    if(testState.numActions == state.numActions + 2)
    {
        printf("TEST #2: Number of actions are properly increased.\n");
    }
    else
    {
        printf("TEST #2: Number of Actions not Properly Increased.\n");
        printf("Expected: %d\n", state.numActions + 2);
        printf("Result: %d\n\n", testState.numActions);
        allTestsPassed = 0;
    }

	// assert(testState.numActions == state.numActions + 2);

	printf("TEST #3: Discard Test\n");

	if(testState.discardCount[thisPlayer] == state.discardCount[thisPlayer] + 1)
    {
        printf("Test #3: Discard Test Passed.\n\n");
    }
    else
    {
        printf("TEST #3: Discard Test Failed!\nExpected: %d\n", state.discardCount[thisPlayer] + 1);
        printf("Result: %d\n\n", testState.discardCount[thisPlayer]);
        allTestsPassed = 0;
    }

    if(allTestsPassed) printf("All tests passed!");
    else printf("One or more tests failed, please review results and revise code.\n\n");

	return 0;

}
