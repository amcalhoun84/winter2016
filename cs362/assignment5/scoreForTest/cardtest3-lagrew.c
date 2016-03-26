/*Jonathan Lagrew
 *cardtest3.c
 *test playVillage()
 *Notes:
 *Village is an Action card from the Base set. Village gives you +1 card from the deck and +2 actions,
 *allowing you to play more than one terminal action each turn.
 */
 /*
 * Basic Requirements of Village:
 * 1. Current player should receive a total of 1 cards from the deck.
 * 2. Current player should receive plus 2 actions.
 * 3. No state change should occur for other players.
 * 4. No state change should occur to the victory drawnCard piles and kingdom card piles.
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "Village"

int testplayVillage(struct gameState *after, int handPos)
{
	int p = after->whoseTurn;//initialize whoseTurn stored as p
	struct gameState before;
	memcpy(&before, after, sizeof(struct gameState));



	playVillage(after, 0, handPos);//run playVillage function with after gameState

	//testing hand count
	//hand counts should be the same before and after card played which account for
	//gaining a card and subtracting a village
	if(before.handCount[p] != after->handCount[p])
	{
		printf("ERROR 1: Hand count difference! Before: %d After: %d\n", before.handCount[p], after->handCount[p]);
	}
	//testing actions
	//after actions will be plus 2 due to card effect
	before.numActions = before.numActions + 2;
	if(before.numActions != after->numActions)
	{
		printf("ERROR 2: Actions has changed from %i to %i", before.numActions, after->numActions);
	}
	//testing buys
	if(before.numBuys != after->numBuys)
		printf("ERROR 3: Buys has changed from %i, to %i", before.numBuys, after->numBuys);

	//testing player
	if(before.whoseTurn != after->whoseTurn)
		printf("ERROR 4: Current player has changed from %i to %i", before.whoseTurn, after->whoseTurn);

	//testing coins
	if(before.coins != after->coins)
		printf("ERROR 5: Coins changed from %i to %i", before.coins, after->coins);


	return 0;
}

int main()
{
	int p = 0;//testing player 0
	int numTests = 250;//250 tests
	struct gameState G;//creating gameState G
	int handPos;//to track hand position
	int i, j, k, m, n, q;//initialize ints for for loops

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	SelectStream(2);
	PutSeed(3);

	for(k = 0; k < numTests; k++)
	{
		for (i = 0; i < sizeof(struct gameState); i++) { //from the lessons, random gameState
			((char*)&G)[i] = floor(Random() * 256);
		}
		G.whoseTurn = p;
		G.numActions = 1;

		//filling in random cards based on lecture 11 and 12 Random Testing
		G.playedCardCount = floor(Random() * MAX_DECK);//generate random played card count
		G.handCount[p] = floor(Random() * MAX_HAND)+1;//+1 for village room in the hand
		G.deckCount[p] = floor(Random() * MAX_DECK);//from lecture 11 to generate random deck
		G.discardCount[p] = floor(Random() * MAX_DECK);//from lecture 11 to generate random discard

		for(m = 0; m < G.handCount[p]; m++)
		{
			G.hand[p][m] = floor(Random() * treasure_map) + 1;
		}

		for(j = 0; j < G.discardCount[p]; j++)
		{
			G.discard[p][j] = floor(Random() * treasure_map) + 1;
		}

		for(n = 0; n < G.deckCount[p]; n++)
		{
			G.deck[p][n] = floor(Random() * treasure_map) + 1;
		}
		for(q = 0; q < G.playedCardCount; q++)
		{
			G.playedCards[q] = floor(Random() * treasure_map) + 1;
		}
		//putting village card in random hand position
		handPos = floor(Random() * G.handCount[p]);
		G.hand[p][handPos] = village;
		//run test above
		testplayVillage(&G, handPos);

	}

	printf("Play Village testing concluded.\n\n");
	return 0;
}
