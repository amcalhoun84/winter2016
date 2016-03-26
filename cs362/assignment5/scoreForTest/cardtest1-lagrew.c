/*Jonathan Lagrew
 *cardtest1.c
 *test playSmithy()
 *Notes:
 *Smithy should draw 3 cards from player's own discard pile
 *and add them to player's own hand.
 */
 /*
 * Basic Requirements of Smithy:
 * 1. Current player should receive exact 3 cards.
 * 2. 3 cards should come from his own pile.
 * 3. No state change should occur for other players.
 * 4. No state change should occur to the victory card piles and kingdom card piles.
 * 5. No changes in coins, buys or actions
 * 6. Ensure that it is still the same player's turn before and after card
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "smithy"

int testSmithy(struct gameState *after, int handPos) //test smithy function
{
	int p = after->whoseTurn;//initialize whoseTurn stored as p
	struct gameState before;//before card played
	int thisPlayer = 0;
	memcpy(&before, after, sizeof(struct gameState));

	printf("Checking status before player buy...\n");
	playSmithy(after, thisPlayer, handPos);//play smithy with after game state and current hand position
	before.handCount[p] = before.handCount[p] + 2; //plus 2 additional after smithy goes to discard
	before.playedCardCount++; //inc card count after smithy played
	//check if the discard pile was shuffled into the deck
	//checking no state changes
	if(before.deckCount[p] < 3) //checking if discard pile shuffled
	{
		before.deckCount[p] = before.deckCount[p] + before.discardCount[p] - 3;//if less than 3, deck plus discard minus 3
	}
	else
	{
		before.deckCount[p] = before.deckCount[p] - 3; //then draws 3 cards
	}
	if(after->handCount[p] != before.handCount[p])
	{
		printf("ERROR 1: handCount is incorrect. Expected: %d Actual: %d\n", before.handCount[p], after->handCount[p]);
	}
	if(after->deckCount[p] != before.deckCount[p])
	{
		printf("ERROR 2: deckCount is incorrect. Expected: %d Actual: %d\n", before.deckCount[p], after->deckCount[p]);
	}
	if(before.playedCardCount != after->playedCardCount)
	{
		printf("ERROR 3: playedCardCount is incorrect. Expected: %d Actual: %d\n", before.playedCardCount, after->playedCardCount);
	}
	int i;
	for(i = 0; i < treasure_map; i++)//checking supply counts before and after buy
	{
		if(before.supplyCount[i] != after->supplyCount[i])
			printf("ERROR 4: Supply counts changed. before: %i, after: %i", before.supplyCount[i], after->supplyCount[i]);
	}

	if(before.whoseTurn != after->whoseTurn)//if whoseTurn changed then different player
		printf("ERROR 5: Different player from %i to %i", before.whoseTurn, after->whoseTurn);

	//check coins
	if(before.coins != after->coins)
		printf("ERROR 6: Number of coins changed from %i to %i", before.coins, after->coins);
	//check number of buys
	if(before.numBuys != after->numBuys)
		printf("ERROR 7: Number of buys has changed from %i, to %i", before.numBuys, after->numBuys);
	//check number of actions
	if(before.numActions != after->numActions)
		printf("ERROR 8: Number of actions has changed from %i to %i", before.numActions, after->numActions);

	return 0;
}

//main function for testing played card
int main()
{
	int p = 0;
	struct gameState G;//generates game state G filled with random bytes
	int handPos;
	int i, j, k, m, n, q;//initializing int

	for (i = 0; i < sizeof(struct gameState); i++) {//random gameState
		((char*)&G)[i] = floor(Random() * 256);
	}
	//starting random tests
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	SelectStream(2);
	PutSeed(3);

	G.whoseTurn = p;//gameState G's player's turn stored in value p

	for(k = 0; k < 250; k++) //250 number of tests
	{
		//from lecture 11
		G.handCount[p] = floor(Random() * MAX_HAND)+1;//fill random hand with one smithy

		G.deckCount[p] = floor(Random() * MAX_DECK);//fill random deck

		G.discardCount[p] = floor(Random() * MAX_DECK);//fill random discard pile

		G.playedCardCount = floor(Random() * MAX_DECK);//fill random played card pile


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
		handPos = floor(Random() * G.handCount[p]);//putting smithy in random hand position
		G.hand[p][handPos] = smithy;//play smithy
		testSmithy(&G, handPos);//running test smithy function
	}

	printf("Play Smithy Tests are concluded.\n\n");

	return 0;
}
