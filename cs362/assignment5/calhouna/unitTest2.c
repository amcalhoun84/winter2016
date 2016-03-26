// Andrew M. Calhoun
// UNIT TEST - ASSIGNMENT 3
// unitTest2.c
// Unit Test for discardCard function.

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
#define NOISY_TEST 0

/* discardCard for Reference

int discardCard(int handPos, int currentPlayer, struct gameState *state, int trashFlag)
{

  //if card is not trashed, added to Played pile
  if (trashFlag < 1)
    {
      //add card to played pile
      state->playedCards[state->playedCardCount] = state->hand[currentPlayer][handPos];
      state->playedCardCount++;
    }

  //set played card to -1
  state->hand[currentPlayer][handPos] = -1;

  //remove card from player's hand
  if ( handPos == (state->handCount[currentPlayer] - 1) ) 	//last card in hand array is played
    {
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
    }
  else if ( state->handCount[currentPlayer] == 1 ) //only one card in hand
    {
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
    }
  else
    {
      //replace discarded card with last card in hand
      state->hand[currentPlayer][handPos] = state->hand[currentPlayer][ (state->handCount[currentPlayer] - 1)];
      //set last card to -1
      state->hand[currentPlayer][state->handCount[currentPlayer] - 1] = -1;
      //reduce number of cards in hand
      state->handCount[currentPlayer]--;
    }

  return 0;
}

*/

int checkDiscardCard(int handPos, int currentPlayer, struct gameState *post, int trashFlag)
{

    struct gameState *pre = malloc(sizeof(struct gameState));
    memcpy(pre, post, sizeof(struct gameState));
    int r;

    r = discardCard(handPos, currentPlayer, post, trashFlag);

    if(trashFlag != 1)
    {
        assert(post->playedCards[pre->playedCardCount] == pre->hand[currentPlayer][handPos]);
        assert(pre->playedCardCount + 1 == post->playedCardCount);
    }

    assert(post->hand[currentPlayer][handPos] = -1);

    if(handPos == (pre->handCount[currentPlayer] - 1))
    {
        assert(pre->handCount[currentPlayer] - 1 == post->handCount[currentPlayer] );
    }
    else if (pre->handCount[currentPlayer] == 1)
    {
        assert(pre->handCount[currentPlayer] - 1 == post->handCount[currentPlayer]);
    }
    else
    {
        assert(pre->hand[currentPlayer][(pre->handCount[currentPlayer])] == post->hand[currentPlayer][ (post->handCount[currentPlayer] + 1 ) ] );
        assert(post->hand[currentPlayer][post->handCount[currentPlayer]] == -1);
        assert(pre->handCount[currentPlayer] - 1 == post->handCount[currentPlayer] );
    }

    assert(r==0);
    free(pre);
    return 0;

}

int main()
{
    srand(time(NULL));
    struct gameState *state = malloc(sizeof(struct gameState));
    int player = 0, handPos, trash;
    int k[10] = {adventurer, smithy, great_hall, council_room, baron, minion, mine, village, ambassador, salvager};

    int seed = rand() % 65535;

    initializeGame(2, k, seed, state);

    printf("Testing the Discard Card Function\n");

    // for(i = 0; i < sizeof(state); i++)
        // printf("Failure Point 1\n");
       //  int handCountConst = floor(Random() * MAX_HAND);
        // p=0;
        handPos = rand() % 10;

        state->deckCount[0] = 50;
        state->discardCount[0] = 10;
        state->handCount[0] = 8;
        state->discard[player][state->discardCount[player]] = 100;
        state->playedCardCount = rand() % 10;
        // printf("Failure Point 2\n");

        if(handPos > 0)
        {
            state->playedCards[state->playedCardCount] = state->hand[0][handPos];
        }
        else
        {
            state->playedCards[state->playedCardCount] = state->hand[0][handPos - 1];
        }

        trash = 0;

        checkDiscardCard(handPos, player, state, trash); // trash = 0 check.
        printf("All assertions passed #1.\n");
        trash = 1;
        handPos = rand() % 10;
        checkDiscardCard(handPos, player, state, trash); // trash = 1 check.
        printf("All assertions passed #2.\n");
        state->handCount[player] = 1;
        trash = -1;
        checkDiscardCard(handPos, player, state, trash); // trash = -1 check.
        printf("All assertions passed #3.\n");

        printf("TESTS OK!\n\n");
        free(state);
        exit(0);
        return 0;
}
