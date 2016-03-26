// Andrew M. Calhoun
// UNIT TEST - ASSIGNMENT 3
// unitTest1.c
// Unit Test for updateCoin function.

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

// updateCoins Code for Reference

/* int updateCoins(int player, struct gameState *state, int bonus)
{
  int i;

  //reset coin count
  state->coins = 0;

  //add coins for each Treasure card in player's hand
  for (i = 0; i < state->handCount[player]; i++)
    {
      if (state->hand[player][i] == copper)
	{
	  state->coins += 1;
	}
      else if (state->hand[player][i] == silver)
	{
	  state->coins += 2;
	}
      else if (state->hand[player][i] == gold)
	{
	  state->coins += 3;
	}
    }

  //add bonus
  state->coins += bonus;

  return 0;
}

*/

// Unit Prototype

int main()
{
    srand(time(NULL));
    int i, j; // counter variables
    int bonus;
    int testRun=0, testFailed=0, testPassed=0, randomTestFailed=0;// Test Counters
    struct gameState *state = malloc(sizeof(struct gameState));
    printf("STANDARDIZED TESTS:\nINITIALIZE GAME: 2 players... deck initialized.... state and memory allocated.\n");
    int k[10] = {adventurer, smithy, village, minion, council_room, feast, mine, remodel, baron, great_hall};
    initializeGame(2, k, 1, state);

    bonus = rand() % 10;
    int goldBinary, goldHold;

    int playerOneHand[10], playerTwoHand[10];
    memcpy(state->hand[0], playerOneHand, 0);
    memcpy(state->hand[0], playerTwoHand, 0);
    state->handCount[0] = 0;

    printf("State Coins: %d\n", state->coins);

    assert(state->coins == 4);
    assert(playerOneHand);
    assert(playerTwoHand);
    assert(bonus <= 10);

    // No Bonus, Empty Hand.

    // printf("Randomized Test Value: %d\n", randomTestFailed);

    printf("Empty Hand, No Bonus Cash.\n");
    updateCoins(0, state, 0);
    if(state->coins == 0) {
        printf("Test #1 Passed.\n");
        testRun++;
        testPassed++;
    }
    else
    {
        printf("Test #1 failed.\n Test found %d coins instead of 0\n.", state->coins);
        testRun++;
        testFailed++;
        randomTestFailed = 1;
    }

    printf("Empty hand, 10 coin bonus.\n");
    updateCoins(0, state, 10);
    if(state->coins==10)
    {
       printf("Test #2 Passed.\n");
       testRun++;
       testPassed++;
    }
    else
    {
        printf("Test #2 failed.\n Test found %d coins instead of 10.\n", state->coins);
        testRun++;
        testFailed++;
        randomTestFailed = 1;
    }

    printf("Empty hand, randomized bonus.\n");
    updateCoins(0, state, bonus);
    if(state->coins==bonus)
    {
        printf("Test #3 Passed.\n");
        testRun++;
        testPassed++;
    }
    else
    {
        printf("Test #3 failed.\n Test found %d instead of %d.\n", state->coins, bonus);
        testRun++;
        testFailed++;
        randomTestFailed=1;
    }

    printf("No coins, negative bonus.\n");
    updateCoins(0, state, -5);
    if(state->coins == -5)
    {
        printf("Test #4 Passed.\n");
        testRun++;
        testPassed++;
    }
    else
    {
        printf("Test #4 Failed.\n");
        testRun++;
        testFailed++;
        randomTestFailed=1;
    }

    printf("Two Coppers, Two Silver, No Bonus.\n");
    playerOneHand[0] = copper;
    playerOneHand[1] = copper;
    playerOneHand[2] = silver;
    playerOneHand[3] = silver;
    memcpy(state->hand[0], playerOneHand, 4 * sizeof(int));
    state->handCount[0] = 4;

    updateCoins(0, state, 0);
    if(state->coins == 6)
    {
        printf("Test #4 Passed!\n");
        testRun++;
        testPassed++;
    }
    else
    {
        printf("Test #4 Failed.\n Test found %d instead of 6 coins.\n", state->coins);
        testRun++;
        testFailed++;
        randomTestFailed=1;

    }

    printf("Same test, two bonus.\n");
    updateCoins(0, state, 2);
    if(state->coins == 8)
    {
        printf("Test #5 Passed!\n");
        testRun++;
        testPassed++;
    }
    else
    {
        printf("Test #5 Failed.\n");
        testRun++;
        testFailed++;
        randomTestFailed=1;
    }

    // printf("Randomized Test Value: %d\n", randomTestFailed);
    // Reset the Number of Coins Held.
    // state->coins = 0;

    printf("Fully Randomized Tests\n");
    for(i = 0; i < 10; i++)
    {
        state->coins = 0;
        for(j=0; j<10; j++)
        {
            goldBinary = rand() % 3;
            switch(goldBinary)
            {
                case 0:
                    playerOneHand[j] = copper;
                    break;
                case 1:
                    playerOneHand[j] = silver;
                    break;
                case 2:
                    playerOneHand[j] = gold;
                    break;
            }
        }
        for(j=0; j<10; j++)
        {
            if(state->hand[0][j] == copper)
            {
                state->coins += 1;
            }
            if(state->hand[0][j] == silver)
            {
                state->coins += 2;
            }
            if(state->hand[0][j] == gold)
            {
                state->coins += 3;
            }
        }

        goldHold = state->coins;
        bonus = rand() % 10;
        goldHold += bonus;
        updateCoins(0, state, bonus);

        if(state->coins == goldHold)
        {
            printf("State Gold (Expected): %d\n", state->coins);
            printf("Player Gold (Result): %d\n", goldHold);
            printf("Randomized Test Passed.\n");
            testRun++;
            testPassed++;
        }
        else
        {
            printf("Randomized Test Failed.\n");
            testRun++;
            testFailed++;
            randomTestFailed=1;
           // break;  // if any randomized tests fail, break the loop..
        }
    }

    // state->coins = 0;
    printf("Tests Run: %d\n", testRun);
    printf("Tests Passed: %d\n", testPassed);
    printf("Tests Failed: %d\n", testFailed);


    printf("Randomized Test Fail Value: %d\n", randomTestFailed);


    if(randomTestFailed == 0)
    {
        printf("All Random Tests Passed!\n\n");
    }
    else
    {
        printf("One or more random tests failed.\n\n");
    }
//    // 4000 Test Cases (20^2)
//        for(i = 0; i < 4000; i++)
//        {
//            for(i = 0; i < sizeof(struct gameState); i++)
//            {
//                result = unitTest(state);
//                if(result == 1) printf("FAIL!\n");
//                else printf("PASS!\n");
//            }
//        }
//
    return 0;
}



