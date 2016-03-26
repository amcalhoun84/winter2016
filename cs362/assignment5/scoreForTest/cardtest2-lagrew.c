/*Jonathan Lagrew
 *cardtest2.c
 *test playAdventurer()
 *Notes:
 *Adventurer is an action card that draws cards from the player's deck until 2 treasure cards are drawn.
 */
 /*
 * Basic Requirements of Adventurer:
 * 1. Current player should receive a total of 2 treasure cards.
 * 2. If drawn card is not a treasure card, then discard and pick a new card.
 * 3. No state change should occur for other players.
 * 4. No state change should occur to the victory drawnCard piles and kingdom card piles.
 * 5. Player is the same when checking before and after treasure counts.
 */

/*
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>

#define TESTCARD "Adventurer"

int testAdventurer(struct gameState *after)
{
	int beforeTreasure, afterTreasure;
	int drawnCard;
	int p = after->whoseTurn;//initialize whoseTurn stored as p

	struct gameState before;//before card played
	memcpy(&before, after, sizeof(struct gameState));
	int drawnTreasure = rand() % 5;
	int thisPlayer = 0;
	int cardDrawn = rand() % 27;
	int tempHand[MAX_HAND];


	playAdventurer(after, thisPlayer, cardDrawn, drawnTreasure, tempHand, 0);

	//first checking for state changes
	int beforeTotal = before.deckCount[p] + before.discardCount[p];//because 2 cards in the hand of player
	int afterTotal = after->deckCount[p] + after->discardCount[p];

	//if deck counts not different by 2 then error msg
	if(afterTotal != beforeTotal)
	{
		printf("ERROR 1: Total card count difference! Should be 2 less than before total. Before: %d After: %d\n", beforeTotal, afterTotal);
	}
	//checking hand state
	// before.handCount[p] =  before.handCount[p]+2;//hand count should be different by 2
	if(after->handCount[p] != before.handCount[p] + 2)//before + 2 should equal after
	{
		printf("ERROR 2: Hand count difference! Before: %d After: %d\n", before.handCount[p], after->handCount[p]);
	}

	//checking all treasure card types in hand are higher than before adventurer was played
	int i;
	for(i = 0; i < before.handCount[p]; i++) //ensuring there are more treasure cards in hand after adventurer is played
	{
		//before
		drawnCard = before.hand[p][i];//checking players hand with all 3 treasure card types
		if (drawnCard == copper || drawnCard == silver || drawnCard == gold)
			beforeTreasure++;
	}
	int j;
	for(j = 0; j < after->handCount[p]; j++)
	{
		//after
		drawnCard = after->hand[p][j];//checking after hand with all 3 treasure card types
		if (drawnCard == copper || drawnCard == silver || drawnCard == gold)
			afterTreasure++;
	}
	//testing if after treasure count is greater after
	if(!(afterTreasure > beforeTreasure))
		printf("ERROR 3: Not enough treasure cards added to hand. Before: %d After: %d\n", beforeTreasure, afterTreasure);
	//Testing State Changes:
		//checking that buys didn't change
	if(before.numBuys != after->numBuys)
		printf("ERROR 4: Buys has changed from %i, to %i", before.numBuys, after->numBuys);
	//Ensuring that before and after player is the same using whoseTurn
	if(before.whoseTurn != after->whoseTurn)
		printf("ERROR 5: Player has changed from %i to %i", before.whoseTurn, after->whoseTurn);
		//checking the number of actions stayed the same
	if(before.numActions != after->numActions)
		printf("ERROR: Actions has changed from %i to %i", before.numActions, after->numActions);
	//checking that number of coins didn't change
	if(before.coins != after->coins)
		printf("ERROR 6: Coins changed from %i to %i", before.coins, after->coins);

	return 0;
}

//main function for starting the 250 tests
int main()
{
	int p = 0;//testing player 0
	struct gameState G;//creating new game state
	int numTests = 250; //250 tests
	int k, m, j, n, i;

	//random game state
	for (i = 0; i < sizeof(struct gameState); i++) {
		((char*)&G)[i] = floor(Random() * 256);
	}

	G.whoseTurn = p;//setting whoseTurn to 0
	SelectStream(2);
	PutSeed(3);
	printf("----------------- Testing card: %s ----------------\n", TESTCARD);

	for(k = 0; k < numTests; k++)
	{
		//filling in random cards based on lecture 11 and 12 Random Testing
		G.handCount[p] = floor(Random() * MAX_HAND)+1;//need at least one adventurer in our hand
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

		testAdventurer(&G);//run tests

	}

	printf("Play Adventurer Tests are concluded.\n\n");

	return 0;
}
