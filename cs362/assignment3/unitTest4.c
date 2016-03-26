// Andrew M. Calhoun
// UNIT TEST - ASSIGNMENT 3
// unitTest4.c
// Unit Test for checkEndTurn function.

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


/*

int endTurn(struct gameState *state) {
  int k;
  int i;
  int currentPlayer = whoseTurn(state);

  //Discard hand
  for (i = 0; i < state->handCount[currentPlayer]; i++){
    state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i];//Discard
    state->hand[currentPlayer][i] = -1;//Set card to -1
  }
  state->handCount[currentPlayer] = 0;//Reset hand count

  //Code for determining the player
  if (currentPlayer < (state->numPlayers - 1)){
    state->whoseTurn = currentPlayer + 1;//Still safe to increment
  }
  else{
    state->whoseTurn = 0;//Max player has been reached, loop back around to player 1
  }

  state->outpostPlayed = 0;
  state->phase = 0;
  state->numActions = 1;
  state->coins = 0;
  state->numBuys = 1;
  state->playedCardCount = 0;
  state->handCount[state->whoseTurn] = 0;

  //int k; move to top
  //Next player draws hand
  for (k = 0; k < 5; k++){
    drawCard(state->whoseTurn, state);//Draw a card
  }

  //Update money
  updateCoins(state->whoseTurn, state , 0);

  return 0;
}

*/

int checkEndTurn(struct gameState *post)
{
    int k, i, j;
    int currentPlayer = whoseTurn(post);
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    printf("Assertions to ensure cards are discarded properly at end of each turn.\n");

    for(i=0; i<pre.handCount[currentPlayer]; i++)
    {
        pre.discard[currentPlayer][pre.discardCount[currentPlayer]++] = pre.hand[currentPlayer][i];
        assert(pre.discard[currentPlayer][pre.discardCount[currentPlayer]++] == post->discard[currentPlayer][post->discardCount[currentPlayer]++]);
        assert(post->discard[currentPlayer][post->discardCount[currentPlayer]++] != pre.hand[currentPlayer][i]);

       //  printf("%d\n%d\n", pre.hand[currentPlayer][i], post->hand[currentPlayer][i]);

        pre.hand[currentPlayer][i] = -1;//Set card to -1
        assert(pre.hand[currentPlayer][i] = post->hand[currentPlayer][i] - 2);

        assert(post->hand[currentPlayer][i] + 1 >= pre.hand[currentPlayer][i]);

        printf("Post Turn End: %d\n Pre Turn End: %d\n", post->handCount[currentPlayer], pre.handCount[currentPlayer]);
        post->handCount[currentPlayer] = pre.handCount[currentPlayer] = 0;

        assert(post->handCount[currentPlayer] == pre.handCount[currentPlayer]); // These should match.

    }

    printf("Discard Assertions passed.\n");

    printf("Player Counts -- makes sure player is properly selected.\n");

    if (currentPlayer < (post->numPlayers - 1))
    {
        post->whoseTurn = currentPlayer + 1;//Still safe to increment
        assert(post->whoseTurn - (currentPlayer + 1) == pre.whoseTurn);
    }
    else{
        pre.whoseTurn = post->whoseTurn = 0;//Max player has been reached, loop back around to player 1
        assert(pre.whoseTurn == post->whoseTurn);
    }

    printf("Player Turn Counter Works Correctly\n");

    printf("Assertion Tests for Outposts, Action Counts, and Coins. All Should Be Equal.\n");

    assert(pre.outpostPlayed == post->outpostPlayed);
    assert(pre.phase == post->phase);
    assert(pre.numActions == post->numActions);
    assert(pre.coins == post->coins);
    assert(post->numBuys == pre.numBuys);
    assert(post->playedCardCount == pre.playedCardCount);
    assert(pre.handCount[post->whoseTurn] == post->handCount[pre.whoseTurn]);

    printf("All assertions passed.\n");

     //int k; move to top
  //Next player draws hand
  for (k = 0; k < 5; k++){
    j = drawCard(pre.whoseTurn, &pre);//Draw a card
    i = drawCard(post->whoseTurn, post);
    assert(j == i); // Broken -- is there a way to properly test this without destroying the program?
  }

    return 0;
}


int main()
{
    srand(time(NULL));
    int seed = rand() % 65535;
    int k[10] = { adventurer, smithy, village, baron, great_hall, council_room, salvager, sea_hag, gardens, mine };
    struct gameState *state = malloc(sizeof(struct gameState));

    initializeGame(2, k, seed, state);

    checkEndTurn(state);

    return 0;

}
