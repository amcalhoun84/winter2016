// Andrew M. Calhoun
// UNIT TEST - ASSIGNMENT 3
// unitTest3.c
// Unit Test for buyCard function.

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

// Ensures that buyCard function works properly and asserts properly.


/* function for reference

int buyCard(int supplyPos, struct gameState *state) {
  int who;
  if (DEBUG){
    printf("Entering buyCard...\n");
  }

  // I don't know what to do about the phase thing.

  who = state->whoseTurn;

  if (state->numBuys < 1){
    if (DEBUG)
      printf("You do not have any buys left\n");
    return -1;
  } else if (supplyCount(supplyPos, state) <1){
    if (DEBUG)
      printf("There are not any of that type of card left\n");
    return -1;
  } else if (state->coins < getCost(supplyPos)){
    if (DEBUG)
      printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
    return -1;
  } else {
    state->phase=1;
    //state->supplyCount[supplyPos]--;
    gainCard(supplyPos, state, 0, who); //card goes in discard, this might be wrong.. (2 means goes into hand, 0 goes into discard)

    state->coins = (state->coins) - (getCost(supplyPos));
    state->numBuys--;
    if (DEBUG)
      printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos, getCost(supplyPos), state->numBuys, state->coins);
  }

  //state->discard[who][state->discardCount[who]] = supplyPos;
  //state->discardCount[who]++;

  return 0;
} */
int checkBuyCard(int supplyPos, struct gameState *state)
{
    int who;
    // if needed
    struct gameState pre;
    memcpy(&pre, state, sizeof(struct gameState));
    //
    printf("Testing Buy Card Functionality\n\n");

    who = state->whoseTurn;

    if(state->numBuys < 1)
    {
        assert(state->numBuys > -1); // Make sure it doesn't go below 0.
        printf("You have no buys left. 0 Buy Test Passed.\n\n");
        return 0;
    }
    else
    {
        assert(state->phase=1); // Ensure that the game is in buy phase.
        assert(state->numBuys >= 1);
        printf("One or more buys available test passed.\n");
        if(supplyCount(supplyPos, state) < 1)
        {
            printf("Out of that particular card!\n");
            return 0;
        }
        printf("Card supply available.\n");
        if (state->coins >= getCost(supplyPos))
        {
            printf("Can buy card with available gold. Yes!\n");
        }
        else
        {
            printf("Thou hast not enough gold!\n");
        }
        buyCard(supplyPos, state);
        assert(state->numBuys == pre.numBuys - 1);
        assert(state->numBuys >= 0);
        assert(state->coins == (pre.coins - getCost(supplyPos)));

        return 0;
    }

}

int main()
{
    srand(time(NULL));
    int seed = rand() % 65535;
    int coinage, buyage;
    int supplyPos;
    int i;
    int k[10] = { adventurer, smithy, village, baron, salvager, sea_hag, great_hall, minion, council_room, gardens };
    struct gameState *state = malloc(sizeof(struct gameState));

    initializeGame(2, k, seed, state);

    for(i=0; i<200; i++)
    {
        supplyPos = rand() % 9; // tried with all cards, but can only allow kingdom cards.
        coinage = rand() % 16 + 8;
        buyage = rand() % 8;
        state->coins = coinage;
        state->numBuys = buyage; // for random testing.

        checkBuyCard(supplyPos, state);
    }

    printf("\n\nAll tests passed!\n");


    free(state);
    return 0;

}
