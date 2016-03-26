// Andrew M. Calhoun
// CARD TEST - ASSIGNMENT 3
// CardTest2.c
// Unit Test for card function - SMITHY.

// cardtest2: cardtest2.c dominion.o rngs.o
//       gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)


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
#define TESTCARD "smithy"

/*

int playSmithy(struct gameState *state, int currentPlayer, int handPos)
{

    int i;

//+3 Cards  *** REFACTOR changes i++ to ++i, breaks the intent of the card.
      for (i = 0; i < 3; ++i)
	{
	  drawCard(currentPlayer, state);
	}
      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);
      return 0;
}

*/

int main()
{
    int i, seed = 65535, numPlayer = 2, thisPlayer, index=0, allTestsPassed = 0, smithyFail = 0;
    int k[10] = {adventurer, smithy, great_hall, council_room, salvager, gardens, mine, remodel, village, ambassador };

    // Checking for victory conditions:
    int victoryPoints; // kCardCount;
    int points[13];

    struct gameState state, testState;
    //int count;


    memset(&state, 0, sizeof(struct gameState)); // Ensure we have a clean slate.
    initializeGame(numPlayer, k, seed, &testState);

    memcpy(&state, &testState, sizeof(struct gameState));

//    // DEBUG CHECK: MAKE SURE THAT ALL THINGS ARE IN PROPER ORDER!
//    printf("Supply of Estates: %d\n", testState.supplyCount[estate]);
//    assert(testState.supplyCount[estate] == 8);
//    printf("Supply of Duchies: %d\n", testState.supplyCount[duchy]);
//    assert(testState.supplyCount[duchy] == 8);
//    printf("Supply of Provinces: %d\n", testState.supplyCount[province]);
//    assert(testState.supplyCount[province] == 8);

    // Treasure cards are variable, cards that assist in victory conditions such as the Land Parcels, Great Hall and Gardens should be 8, all Kingdom Cards should be 10. For the
    // purposes of this test, we need to make sure that the supply of land parcels is proper.

    printf("Testing Smithy!\n");

    victoryPoints = testState.supplyCount[estate];
    points[index] = victoryPoints;
    index++;

//    printf("Total Estate Victory Points: %d\n", victoryPoints);
    assert(victoryPoints == 8);


    victoryPoints = testState.supplyCount[duchy] * 3;
    points[index] = victoryPoints;
    index++;

   // printf("Total Duchy Victory Points: %d\n", victoryPoints);
    assert(victoryPoints == 24);

    victoryPoints = testState.supplyCount[province] * 6;
    points[index] = victoryPoints;
    index++;

//    printf("Total Province Victory Points: %d\n", victoryPoints);
    assert(victoryPoints == 48);
//    assert(points[0] == 8 && points[1] == 24 && points[2] == 48); // Make sure all the points match up.

//    for(i = 0; i < 10; i++)
//    {
//        kCardCount = testState.supplyCount[k[i]];
//        points[index] = kCardCount;
//        index++;
//        printf("k[%d] count: %d\n", i, kCardCount);
//        if(kCardCount != testState.supplyCount[k[i]]);
//        {
//            allTestsPassed=0;
//        }
//    }
//    if(!allTestsPassed) printf("Some cards do not have the proper supply. Please see reference materials or restore base game files for proper output.\n");

    thisPlayer=0;
    printf("Player 1 Test -- GIVING SMITHY");

    testState.hand[thisPlayer][testState.handCount[thisPlayer]] = smithy;
    testState.handCount[thisPlayer]++;
    if(state.handCount[thisPlayer] + 1 == testState.handCount[thisPlayer])
    {
        printf("Giving Smithy Test Passed.\n");
    }
    else
    {
        printf("Unable to give Smithy. Aborting.\n\n");
        allTestsPassed = 0;
        exit(0);
    }

    for(i=0; i < testState.deckCount[thisPlayer]; i++)
    {
        printf("Position %d, Card: %d\n", i, testState.deck[thisPlayer][i]);
    }

    printf("DECK COUNTED\n");
    printf("DISCARD COUNT -- SHOULD BE 0\n");

    if(testState.discardCount[thisPlayer] == 0)
    {
        printf("Discard Successful. Count successful.\n");
    }
    else
    {
        printf("Discard Count Unsuccessful.\n");
        allTestsPassed = 0;
    }

    // EXPECTED: 1, 4, 1, 4, 1, 13;
    int handCountExpect[] = { 1, 4, 1, 4, 1, 13 }; // 13 is smithy. MAKE SURE IT'S SMITHY!!!!
    assert(testState.hand[thisPlayer][5] == smithy);
    for(i=0; i < testState.handCount[thisPlayer]; i++)
    {
        printf("Position %d, card: %d\n", i, testState.hand[thisPlayer][i]);
        if(handCountExpect[i] != testState.hand[thisPlayer][i]);
        {
            allTestsPassed=0;
        }
    }

    // Add 3 more cards to deck.
    testState.deck[thisPlayer][testState.deckCount[thisPlayer] ] = gardens; // 10
    testState.deckCount[thisPlayer]++;
    testState.deck[thisPlayer][testState.deckCount[thisPlayer] ] = salvager; // 24
    testState.deckCount[thisPlayer]++;
    testState.deck[thisPlayer][testState.deckCount[thisPlayer] ] = ambassador; // 20
    testState.deckCount[thisPlayer]++;

    // TEST THE SMITHY

    printf("Smithy Granted Hand Count: %d\n", testState.handCount[thisPlayer]);
    printf("Pre-Smithy Granted Hand Count: %d\n", state.handCount[thisPlayer]);

    playSmithy(thisPlayer, &testState, 0);

    for(i=0; i < testState.handCount[thisPlayer]; i++)
    {
        printf("Position %d, card: %d\n", i, testState.hand[thisPlayer][i]);

    }

    printf("\n\n");

    if(testState.handCount[thisPlayer] == state.handCount[thisPlayer] + 4) // Should fail, bug gives only two cards. Test also factors in the fact that Smithy Card is granted.
    {
        printf("Smithy added 3 cards successfully. Test Passed.\n");
    }
    else
    {
        printf("Smithy did not add expected number of cards.\n");
        printf("Expected Hand Count: %d\n", state.handCount[thisPlayer] + 4);
        printf("Result: %d\n", testState.handCount[thisPlayer]);
        allTestsPassed = 0;
        smithyFail = 1;
    }


    printf("Testing Card Discard.\n");

    for(i=0; i < testState.discardCount[thisPlayer]; i++)
    {
        printf("Discard Card: %d\n", testState.discard[thisPlayer][i]);
    }

    if(testState.deckCount[thisPlayer] == state.deckCount[thisPlayer]) // Should be even again.
    {
        printf("Discard test and check passed.\n");
    }
    else
    {
        printf("Discard test and check failed.\n");
        allTestsPassed=0;
    }

    printf("Checking hand and seed state.\n");

    int seedHandExpect[] = {10, 4, 1, 4, 1, 13, 20, 24};
    int seedNameExpect[] = { gardens, copper, estate, copper, estate, smithy, ambassador, salvager };
    for(i=0; i < testState.handCount[thisPlayer]; i++)
    {
        if(seedHandExpect[i] != testState.hand[thisPlayer][i])
        {
            allTestsPassed = 0;
        }
        if(seedHandExpect[i] != seedNameExpect[i])
        {
            allTestsPassed = 0;
        }
        //printf("Position %d Hand Card: %d\n", i, testState.hand[thisPlayer][i]);
    }

    printf("Test Passed.\n");

    thisPlayer = 1;
    // Do the tests again.
    printf("\nPlayer 2 Tests ----- Making sure everything looks good for their turn.\n\n");

    int copperCount = 0;
    int estateCount = 0;

    printf("DECK RECOUNT\n");
    for(i = 0; i < testState.deckCount[thisPlayer]; i++)
    {
        printf("Position %d, Card: %d\n", i, testState.deck[thisPlayer][i]);
        if(testState.deck[thisPlayer][i] == copper)
        {
            copperCount++;
        }
        if(testState.deck[thisPlayer][i] == estate)
        {
            estateCount++;
        }
    }

    if(copperCount != 7)
    {
        printf("Improper copper amount. Should be 7. Result: %d\n", copperCount);
        allTestsPassed = 0;
    }
    if(estateCount != 3)
    {
        printf("Improper estate amount. Should be 3. Result: %d\n", estateCount);
        allTestsPassed = 0;
    }


    printf("Player has 10 card count Test.\n");

    if(testState.deckCount[thisPlayer] == 10)
    {
        printf("Player has proper number of cards.\n");
    }
    else
    {
        printf("Player has improper number of cards. Should have 10.\nPlayer Has: %d\n", testState.deckCount[thisPlayer]);
        allTestsPassed = 0;
    }

    testState.hand[thisPlayer][testState.handCount[thisPlayer]] = smithy;
    testState.handCount[thisPlayer]++;
    if(state.handCount[thisPlayer] + 1 == testState.handCount[thisPlayer])
    {
        printf("Smithy properly granted to player and player card count incremented.\n");
    }
    else
    {
        printf("Smithy was either improperly granted or card count not incremented.\n");
        allTestsPassed=0;
    }

    // Ensure the victory card count is the same.

    assert(testState.supplyCount[estate] == 8);
    assert(testState.supplyCount[estate] == 8);
    assert(testState.supplyCount[estate] == 8);
    assert(points[0] == 8);
    assert(points[1] == 24);
    assert(points[2] == 48);

    if(allTestsPassed) printf("No state changes, all tests passed.\n\n");
    else printf("One or more tests failed. Please review outputs and revise code.\n");
    if(smithyFail)
    {
        printf("Smithy card did not work properly. Please revise code.\n");
        printf("Smithy gave: %d\n", testState.handCount[0]);
        printf("Expected: %d\n", state.handCount[0] + 4);
    }

    return 0;
}
