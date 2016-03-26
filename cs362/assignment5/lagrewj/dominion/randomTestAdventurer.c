/*
** Name: Andrew Michael Calhoun
** Date: 2/5/2014
** CS 362 - Assignment 4
** randomtestadventurer.c
*/


#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


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

int main()
{
	srand(time(NULL));

	int i, j, a;

	struct gameState state;
	struct gameState testState;

	int k[10] = { adventurer, salvager, smithy, village, minion, mine, gardens, council_room, great_hall, sea_hag };

	int newCards = 0, discarded = 1, numTests = 1000;
	int handPos = 0, shuffledCards = 0, z = 0, temphand[MAX_HAND];
	int thisPlayer = 0, numPlayers = 2, seed = 29365;
	int drawnTreasure;
	int totalSuccesses, totalFailures;
	int successBool;

	const char* cardNames[] = {"curse", "estate", "duchy", "province", "copper", "silver", "gold", "adventurer",
							    "council_room", "feast", "gardens", "mine", "remodel", "smithy", "village", "baron", "great_hall", "minion",
								"steward", "tribute", "ambassador", "cutpurse", "embargo", "outpost", "salvager", "sea_hag", "treasure_map"};


	initializeGame(numPlayers, k, seed, &state);

	// Make sure we are at least close to the base game:

	assert(state.supplyCount[estate] == 8);
	assert(state.supplyCount[duchy] == 8);
	assert(state.supplyCount[province] == 8);

	assert(state.supplyCount[copper] == 46);
	assert(state.supplyCount[silver] == 40);
	assert(state.supplyCount[gold] == 30);


	printf("\nGENERATING CARD TESTS FOR %s\n", TESTCARD);

	totalSuccesses = totalFailures = 0;

	for(i = 0; i < numTests; i++)
	{
		drawnTreasure = 0;

		for(j=0; j < temphand[MAX_HAND]; j++)
		{
			temphand[j] = random() % 27;
			if(temphand[j] == 4 || temphand[j] == 5 || temphand[j] == 6)
				drawnTreasure++;
		}

		for(j=0; j < temphand[MAX_HAND]; j++)
		{
			state.hand[thisPlayer][j] = temphand[j];
		}

		successBool = 1;
		printf("\nITERATION %d\n", i + 1);
		printf("Drawn Treasure: %d\n", drawnTreasure);

		// Starting Hands...
//		state.hand[thisPlayer][0] = adventurer;
//		state.hand[thisPlayer][1] = smithy;
//		state.hand[thisPlayer][2] = silver;
//		state.hand[thisPlayer][3] = ambassador;
//		state.hand[thisPlayer][4] = mine;

		memcpy(&testState, &state, sizeof(struct gameState));


		if(testState.supplyCount[estate] != 8 || testState.supplyCount[duchy] != 8 || testState.supplyCount[province] != 8)
		{
			printf("\nFailed Test, victory cards are incorrect.\n");
			successBool = 0;
		}

		if((testState.supplyCount[copper] + testState.supplyCount[silver] + testState.supplyCount[gold]) != 116)
		{
			printf("\nTest failed, total coins is NOT 116\n");
			successBool = 0;
		}

		if(testState.supplyCount[copper] != 46 || testState.supplyCount[silver] != 40 || testState.supplyCount[gold] != 30)
		{
			printf("\nTest failed, treasure card totals are incorrect. \n");
			successBool = 0;
		}

		newCards = 0;
		discarded = 0;

		if(testState.deckCount[thisPlayer] != state.deckCount[thisPlayer] - newCards + shuffledCards)
		{
			printf("\nTest failed, card changes are incorrect. Player 1 Deck count is incorrect.\n");
			successBool = 0;
		}

		if(testState.deckCount[thisPlayer + 1] != state.deckCount[thisPlayer + 1] - newCards + shuffledCards)
		{
			printf("\nTest failed, card changes are incorrect. Player 2 Deck Count is incorrect\n");
			successBool = 0;
		}

		playAdventurer(&testState);

		printf("\nCards in hand for player 1 are: ");
		for(j=0; j<testState.handCount[thisPlayer]; j++)
		{
			printf("Card: %s\n", cardNames[testState.hand[thisPlayer][j]]);

		}

		printf("\nCards in the deck for player 1: ");
		for(j=0; j<testState.deckCount[thisPlayer]; j++)
		{
			printf("Card in Deck: %s\n", cardNames[testState.deck[thisPlayer][j]]);
		}


	//	playAdventurer(&testState, thisPlayer, 0, drawnTreasure, temphand, z);

		printf("\nEnd Turn Cards for Player 1: ");

		for(j=0; j<testState.handCount[thisPlayer]; j++)
		{
			printf("Card: %s\n", cardNames[testState.hand[thisPlayer][j]]);
		}

		printf("\nEnd Turn Deck Cards for Player 1: ");

		for(j=0; j<testState.deckCount[thisPlayer]; j++)
		{
			printf("Cards in Deck: %s\n", cardNames[testState.hand[thisPlayer][j]]);
		}

		newCards = testState.handCount[thisPlayer] - state.handCount[thisPlayer];

		if(testState.handCount[thisPlayer] != state.handCount[thisPlayer] + newCards)
		{
			printf("\nTest failed, player 1 cards are incorrect. \n");
			successBool = 0;
		}

		// 46 + 40 + 30 = 116

		if((testState.supplyCount[copper] + testState.supplyCount[silver] + testState.supplyCount[gold]) != 116)
		{
			printf("\nTest failed, total coins is NOT 116\n");
			successBool = 0;
		}

/*		if(testState.supplyCount[estate] != 8 || testState.supplyCount[duchy] != 8 || testState.supplyCount[province] != 8)
		{
			printf("\nFailed Test, victory cards are incorrect.\n");
			successBool = 0;
		}*/

/*		if(testState.supplyCount[copper] != 46 || testState.supplyCount[silver] != 40 || testState.supplyCount[silver] != 30)
		{
			printf("\nTest failed, treasure card totals are incorrect. \n");
			successBool = 0;
		}*/

		newCards = testState.handCount[thisPlayer] - state.handCount[thisPlayer];;
		discarded = testState.discardCount[thisPlayer] - state.discardCount[thisPlayer];;

		printf("New Cards: %d\n", newCards);
		printf("Discarded: %d\n", discarded);

		if(testState.handCount[thisPlayer] != state.handCount[thisPlayer] + newCards - discarded)
		{
			printf("\nFailed test, Player 1 hand count is incorrect.\n");
			printf("Expected: %d,   Result: %d\n", testState.handCount[thisPlayer], state.handCount[thisPlayer] + newCards - discarded);
			successBool = 0;
		}

		a = 0;

		for(j = 0; j < testState.discardCount[thisPlayer]; j++)
		{
			if(testState.discard[thisPlayer][j] == copper || testState.discard[thisPlayer][j] == silver || testState.discard[thisPlayer][j] == gold)
			{
				a++;
			}
		}

		if(a > 0)
		{
			printf("\nTest failed, the discard piles have treasure cards in them. %d total treasure discarded:", a);
			successBool = 0;
		}


		newCards = 0;
		discarded = 0;

		if(testState.deckCount[thisPlayer + 1] != state.deckCount[thisPlayer + 1] - newCards + shuffledCards)
		{
			printf("\nTest failed, player 2 deck is incorrect.");
			successBool = 0;
		}

		if(successBool == 1)
		{
			totalSuccesses++;
		}
		else
		{
			totalFailures++;
		}

	}

	//printf("Testing seed: %d", seed); // 29365 for debugging purposes. Had to find one that had consistent test failures.
	printf("Testing complete for %s.\n", TESTCARD);
	printf("Total Tests: %d\n", numTests);
	printf("Total Successful: %d\n", totalSuccesses);
	printf("Total Unsuccessful: %d\n", totalFailures);

	return 0;
}
