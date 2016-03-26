// Andrew M. Calhoun
// UNIT TEST - ASSIGNMENT 3
// unitTest1.c
// Unit Test for card function -- ADVENTURER.
// cardtest1: cardtest1.c dominion.o rngs.o
//       gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)



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
#define TESTCARD "adventurer"

/*

int playAdventurer(struct gameState *state, int currentPlayer, int cardDrawn, int drawntreasure, int temphand[], int z)
{

      while(drawntreasure<2){
    if (state->deckCount[currentPlayer] <=1)
        {//if the deck is empty we need to shuffle discard and add to deck *** REFACTOR to <= will cause additional shuffling.
      shuffle(currentPlayer, state);
        }
        drawCard(currentPlayer, state);
        cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
      drawntreasure+=2; // The adventurer should not make off with your treasure. Originally had this put as --, but it actually caused the game to freeze. Opted to give the player more coins. Changes seeded outcomes.
    else{
      temphand[z]=cardDrawn;
      state->handCount[currentPlayer]++; //this should just remove the top card (the most recently drawn one).
      z++;
    }
      }
      while(z-1>=0){
    state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
    z=z-1;
      }
      return 0;
}

*/

int checkPlayAdventurer(struct gameState *state)
{
    int i, preDraw, failTest = 0;
      int k[10] = { adventurer, smithy, village, baron, great_hall, council_room, salvager, sea_hag, gardens, mine };
    // const char *cards[] = { "curse", "estate", "duchy", "province", "copper", "silver", "gold", "adventurer", "council_room", "feast", "gardens", "mine", "remodel", "smithy", "village", "baron",
    // "great_hall", "minion", "steward", "tribute", "ambassador", "cutpurse", "embargo", "outpost", "salvager", "sea_hag", "treasure_map" };
    int randomCard = 0; //rand() % 10;
    int currentPlayer = 0;
    int z = 0;
    int drawnTreasure = rand() % 3;
    int temphand[MAX_HAND];
    int cardDrawn = rand() % 27;

    for(i=0; i < MAX_HAND; i++)
    {
        if(i < 2)
        {
           temphand[i] = copper;
        }
        else
        {
            temphand[i] = k[i];
        }
    }

    // int x;
    struct gameState testState;
    memcpy(&testState, state, sizeof(struct gameState));

    printf("\nChecking Drawn Treasure.\nDrawn Treasure: %d\n", drawnTreasure);

    printf("Initiating randomized deckCount: 1-5\n");
    state->deckCount[currentPlayer] = rand() % 4;

    printf("Deck Count: %d\n", state->deckCount[currentPlayer]);

    playAdventurer(state);

    if(drawnTreasure>=2)
    {
        if(testState.handCount[currentPlayer] == state->handCount[currentPlayer]++)
         printf("Deck Draw Test Passed.\n");
        else
        {
            printf("Deck draw tests failed.\n");
            failTest = 1;
        }
    }

    if(testState.coins == state->coins++)
    {
        printf("Coin test passed.\n");
    }
    else
    {
        printf("Test failed. Expected Coins: %d   Coins Held: %d\n", state->coins++, testState.coins);
        failTest = 1;
    }

    printf("Previous Hand Count: %d\n", state->handCount[currentPlayer]);
    printf("Current Hand Count: %d\n", testState.handCount[currentPlayer]);

    return failTest;
}

int main()
{
    // Set up the test harness.

    srand(time(NULL));
    int k[10] = { adventurer, smithy, village, baron, great_hall, council_room, salvager, sea_hag, gardens, mine };
    int x, i, testSuiteSuccess = 0, testSuiteFailure = 0, testsRun = 0;
    int seed = 1000; // Perhaps having a set seed would standardize testing, but we need ALL situations tested, even those we don't anticipate.

    struct gameState *post = malloc(sizeof(struct gameState));
    initializeGame(2, k, seed, post);

    printf("Testing Adventurer!\n");
    for(i=0; i < 20; i++)
    {
        x = checkPlayAdventurer(post);
        if(x)
        {
            printf("One or More Assertion and Gameplay Tests Failed.\n");
            testSuiteFailure++;
            testsRun++;
        }
        else
        {
            printf("All Assertion and Gameplay Tests Passed!\n");
            testSuiteSuccess++;
            testsRun++;
        }
    }

    printf("Successes / Tests Run: %d / %d\n", testSuiteSuccess, testsRun);
    printf("Failures / Tests Run: %d / %d\n", testSuiteFailure, testsRun);

    free(post);
    return 0;
}
