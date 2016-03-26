// Andrew M. Calhoun
// CARD TEST - ASSIGNMENT 3
// CardTest4.c
// Unit Test for card function - MINION.

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
#include <string.h>

#define DEBUG 0
#define NOISY_TEST 1
#define TESTCARD "minion"

/*-------------------------------
// PLAY MINION
//-------------------------------

int playMinion(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos)
{

    int i, j;


      //+1 action
      state->numActions--;

      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);

      if (choice1)		//+2 coins *** REFACTORED: -2 coins.
	{
	  state->coins = state->coins - 2;
	}

      else if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{

//	  //discard hand ** COMMENTED OUT FOR REFACTORING
//	  while(numHandCards(state) > 0)
//	    {
//	      discardCard(handPos, currentPlayer, state, 0);
//	    }

	  //draw 4 *** REFACTORED TO DRAW 3
	  for (i = 0; i < 3; i++)
	    {
	      drawCard(currentPlayer, state);
	    }

	  //other players discard hand and redraw if hand size > 4 *** REFACTOR: They not only get to keep their cards, they get 3 MORE!
	  for (i = 0; i < state->numPlayers; i++)
	    {
	      if (i != currentPlayer)
		{
		  if ( state->handCount[i] > 4 )
		    {
		    //  //discard hand
		    //  while( state->handCount[i] > 0 )
			// {
			//  discardCard(handPos, i, state, 0);
			// }

		      //draw 4 ** REFACTOR: 3
		      for (j = 0; j < 3; j++)
			{
			  drawCard(i, state);
			}
		    }
		}
	    }

	}
      return 0;

}

*/

int main()
{
    // The minion card has been changed drastically, thus it should fail on most accounts -- tests will find failure points and point them out.

   int i, thisPlayer, handPos = 0, seed = 65535, numPlayer = 2, kCardCount, choice1, choice2, allTestsPassed = 1;
	int k[10] = {adventurer, smithy, great_hall, council_room, salvager, gardens, mine, remodel, village, ambassador };
	struct gameState state, testState;


   // memset(&state, 0, sizeof(struct gameState)); // Ensure we have a clean slate.
    initializeGame(numPlayer, k, seed, &testState);

    memcpy(&state, &testState, sizeof(struct gameState));

    testState.coins = 10;
    state.coins = 10;
    testState.numActions = 1;

    thisPlayer = 0;
    // playMinion(&testState, 0, 0, thisPlayer, 0);

    // assert(state.supplyCount[duchy] == 8);  // Making sure the supplies are proper, if not,
                                            // please reload original or refactored code
//                                            // that does not adjust values...
//
//    printf("TEST #1: Checking card supplies...\n");
//    for(i = 0; i < 26; i++)
//    {
//        kCardCount = testState.supplyCount[i];
//        printf("Card: %d\n", testState.supplyCount[i]);
//        assert(testState.supplyCount[i] == kCardCount);
//
//    }
//
//    printf("All kingdom and victory cards check out.\n");

    printf("TEST #1: Minion Card - choice1 = 1, choice 2 = 0\n");
    choice1 = 1;
    choice2 = 0;

    playMinion(&testState, choice1, choice2, thisPlayer, handPos);

    printf("Checking to see if action was added...");
    if(testState.numActions == state.numActions + 1)
    {
        printf("TEST #1: Action Test passed!\n\n");
    }
    else
    {
        printf("TEST #1: Action Test failed!\n");
        printf("Expected: %d\n", state.numActions + 1);
        printf("Result: %d\n\n", testState.numActions);
        allTestsPassed = 0;
       // assert(testState.numActions == state.numActions + 1); // This will break.
    }

    printf("Checking to see if 2 coins were added...\n");
    if(testState.coins == state.coins + 2)
    {
        printf("TEST #1: Coin Test passed!\n");
    }
    else
    {
        printf("TEST #1: Coin Test failed!\n That dirty minion stole or forgot to give us coins!\n");
        printf("Expected: %d\n", state.coins + 2);
        printf("Result: %d\n\n", testState.coins);
        allTestsPassed = 0;
    }

    /* printf("Discard Count: %d\n", testState.discardCount[thisPlayer]);
    printf("Discards: %d\n", testState.discard[thisPlayer][i]);

    if(testState.discardCount[thisPlayer] != state.discardCount[thisPlayer])
    {
        printf("TEST #1: Discard Test Failed.\n");
        allTestsPassed = 0;
    }
    else
    {
        printf("TEST #1: Discard Test Passed.\n");
    }
    */


    printf("TEST #1: Previous Hand Count: %d\n", state.handCount[thisPlayer]);
    printf("TEST #1: Current Hand Count: %d\n", testState.handCount[thisPlayer]);

    if(state.handCount[thisPlayer] == testState.handCount[thisPlayer] + 1)
    {
        printf("TEST #1: Hand Count Test passed.\n\n");
    }
    else
    {
        printf("TEST #1: Hand Count Test failed.\n\n");
        allTestsPassed = 0;
    }

    printf("Checking to see if other players drew cards. (Should Not if Choice 1 was Picked)\n");

    for(i=1; i<numPlayer; i++)
    {
        printf("Number of Cards in Hand for Player %d: %d\n", (i+1), testState.handCount[i]);
        if(testState.handCount[i] > state.handCount[i])
        {
            printf("TEST #1: Player Draw Test failed.\n\n");
            allTestsPassed = 0;
        }
        else
        {
            printf("TEST #1: Player Draw Test passed.\n\n");
        }
    }

    printf("Resetting values...\n\n");

    memcpy(&testState, &state, sizeof(struct gameState));



    testState.coins = state.coins = 10;
    testState.numActions = state.numActions = 1;
    choice1 = 0;
    choice2 = 1;

    state.handCount[1] = 5;
    testState.handCount[1] = 5;

    printf("MINION TEST #2: Checking Choice 2...\n");
    playMinion(&testState, choice1, choice2, thisPlayer, handPos);

    printf("Checking to see if action was added...\n");

    if(testState.numActions == state.numActions + 1)
    {
        printf("TEST #2: Action Test Passed!\n\n");
    }
    else
    {
        printf("TEST #2: Test failed!\n");
        printf("Expected: %d\n", state.numActions + 1);
        printf("Result: %d\n\n", testState.numActions);
        allTestsPassed = 0;
       // assert(testState.numActions == state.numActions + 1); // This will break.
    }

    // endTurn(&testState);

    printf("Testing to see if 2 coins were added... (UPDATED FOR CORRECTNESS)\n");

    if(state.coins == testState.coins)
    {
        printf("TEST #2: Coin Test passed!\n\n");
    }
    else
    {
        printf("TEST #2: Test failed!\n");
        printf("Expected: %d\n", state.coins + 2);
        printf("Result: %d\n\n", testState.coins);
        allTestsPassed = 0;
    }

    /* printf("TEST #2: Checking discard. Should be greater than 0.\n");
     if(testState.discardCount[thisPlayer] > state.discardCount[thisPlayer])
    {
        printf("TEST #2: Discard Test passed.\n\n");
    }
    else
    {
        printf("TEST #2: Test failed.\n\n");
        allTestsPassed = 0;
    }

    */

    printf("TEST #2: Checking to see that 4 cards were drawn for player 1\n\n");
    if(testState.handCount[thisPlayer] <= state.handCount[thisPlayer])
    {
        printf("TEST #2: Draw Test passed!\n\n");
    }
    else
    {
        printf("TEST #2: Draw Test failed!\n");
        printf("Expected: %d\n", state.handCount[thisPlayer] -1);
        printf("Result: %d\n\n", testState.handCount[thisPlayer]);
        allTestsPassed = 0;
    }

    printf("TEST #2: Checking Hand Counts for other Players.\n");
    for(i = 1; i < numPlayer; i++)
    {
        printf("Player %d: Hand Count: %d\n", 2, testState.handCount[1]);
        if(testState.handCount[1] > 4)
        {
            printf("TEST #2: Other Player Hand Count Test failed!\n");
            printf("Expected: 4\n");
            printf("Result %d\n\n", testState.handCount[1]);
        }
        else
        {
            printf("TEST #2: Other Player Hand Count Test Passed.\n\n");
            printf("Expected: 4\n");
            printf("Result: %d\n", testState.handCount[1]);

        }
    }

    // refactored bug should increase number of cards to 6/7, but it does not.


    if(allTestsPassed)
    {
        printf("All tests passed. Function works perfectly!\n");
    }
    else
    {
        printf("One or more tests failed! Please review results and revise code.\n");
    }

	return 0;
}
