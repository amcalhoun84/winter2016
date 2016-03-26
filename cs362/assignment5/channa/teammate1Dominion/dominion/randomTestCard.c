/* -----------------------------------------------------------------------
 * test remodel
 * 
 *
 * randomtestcard: randomtestcard.c dominion.o rngs.o
 *      gcc -o randomtestcard -g  randomtestcard.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "rngs.h"

int trashOK(int trashCard, int gainCard);
int displayAll(struct gameState *state, int p);
int displayDeck(struct gameState *state, int p);
int displayDiscard(struct gameState *state, int p);
int displayHand(struct gameState *state, int p);

int main() {
    int i, j;
    int seed = 1000;

    int numPlayer = 2;
    int p, r;
    int k[10] = {adventurer, great_hall, cutpurse, gardens, mine
               , remodel, smithy, village, sea_hag, embargo};

    struct gameState G, testG;
    // Card enum - Name        - Cost 
    // 0         - curse       - 0
    // 22        - embargo     - 2
    // 14        - village     - 3
    // 13        - smithy      - 4
    // 11        - mine        - 5
    //  7        - adventurer  - 6
    //  3        - province    - 8
    // toTest[] is the list of cards we will try to gain
    int toTest[] = {curse, embargo, village, smithy, mine, adventurer, province};

    int index;
    int trashCard, trashIndex;
    int gainCard, gainIndex;
    int preDeck, preDiscard, preHand;
    int expectDeck, expectDiscard, expectHand;
    int result, expected;
    bool pass = true;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    printf("----------------- Testing remodel\n");

    for (j = 1; j < 1001; j++) {
        // Copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));

        // Start testing
        p = 0;
        printf("\n----------------- Random Test %d\n", j);
        printf("----------------- Initial counts\n");

        // Initialize card counts
        testG.deckCount[p] = 0;
        testG.discardCount[p] = 0;
        testG.handCount[p] = 0;

        // Create hand of 10 random cards
        for (i = 0; i < 10; i++) {
            index = floor(Random() * 10);
            testG.hand[p][i] = k[index];
            testG.handCount[p]++;
        }

        // Put remodel in hand
        testG.hand[p][0] = remodel;

        // Decide on card to trash
        // trashIndex = (Random() % max - min + 1)) + min
        // max = 9, min = 1 (remodel is at index 0)
        trashIndex = (floor(Random() * 9)) + 1;
        // trashCard is the card at trashIndex
        trashCard = testG.hand[p][trashIndex];

        // Decide on card to gain
        // gainIndex = (Random() % max - min + 1)) + min
        // max = 6, min = 0
        gainIndex = floor(Random() * 6);
        // trashCard is the card at trashIndex
        gainCard = toTest[gainIndex];

        // Save pre-test values
        preDeck = testG.deckCount[p];
        preDiscard = testG.discardCount[p];
        preHand = testG.handCount[p];

        displayAll(&testG, p);

        printf("trashCard: %d, trashIndex: %d\n", trashCard, trashIndex);
        printf("gainCard: %d, gainIndex: %d\n", gainCard, gainIndex);

        printf("----------------- After playRemodel\n");

        // game state
        // player
        // card to trash: int choice1 = trashIndex
        // card to gain : int choice2 = gainCard
        // remodel index: int handPos = 0 (testG.hand[p][0])
        result = playRemodel(&testG, p, trashIndex, gainCard, 0);
        expected = trashOK(trashCard, gainCard);

        displayAll(&testG, p);

        printf("\nchoice1 cost: %d, choice2 cost: %d\n", getCost(trashCard), getCost(gainCard));
        printf("Result: %d, Expected: %d\n", result, expected);
        if (result != expected) {
            printf("----------------- TEST FAILED!\n");
            pass = false;
        }

        // Successful trash and gain
        // no change to deckCount
        // discardCount should be + 2 because gained a card and played remodel
        // handCount should be -2 because played remodel and trashed a card
        expectDeck = preDeck;
        expectDiscard = preDiscard + 2;
        expectHand = preHand - 2;

        if (expected == 0) {
            printf("Deck count: %d, Expected: %d\n", testG.deckCount[p], expectDeck);
            if (testG.deckCount[p] != expectDeck) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }

            printf("Discard count: %d, Expected: %d\n", testG.discardCount[p], expectDiscard);
            if (testG.discardCount[p] != expectDiscard) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }

            printf("Hand count: %d, Expected: %d\n", testG.handCount[p], expectHand);
            if (testG.handCount[p] != expectHand) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }

        // Unsuccessful - no trash/gain
        // no change to deckCount
        // no change to discardCount (could not gain card)
        // no change to handCount (keep remodel and could not trash card)
        expectDeck = preDeck;
        expectDiscard = preDiscard;
        expectHand = preHand;

        if (expected == -1) {
            printf("Deck count: %d, Expected: %d\n", testG.deckCount[p], expectDeck);
            if (testG.deckCount[p] != expectDeck) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }

            printf("Discard count: %d, Expected: %d\n", testG.discardCount[p], expectDiscard);
            if (testG.discardCount[p] != expectDiscard) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }

            printf("Hand count: %d, Expected: %d\n", testG.handCount[p], expectHand);
            if (testG.handCount[p] != expectHand) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }
    }

    if (pass) {
        printf("\nAll tests passed!\n");
    }
    else {
        printf("\nSome test(s) failed!\n");
    }

    return 0;
}

int trashOK(int trashCard, int gainCard) {
    // Gain a card costing up to 2 more than the trashed card.
    if (getCost(trashCard) + 2 >= getCost(gainCard)) {
        // success = 0
        return 0;
    }

    // failure = -1
    return -1;
}

int displayAll(struct gameState *state, int p) {
    int i;

    printf("DECK\n");
    for (i = 0; i < state->deckCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->deck[p][i]);
    }

    printf("DISCARD\n");
    for (i = 0; i < state->discardCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->discard[p][i]);
    }

    printf("HAND\n");
    for (i = 0; i < state->handCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->hand[p][i]);
    }

    return 0;
}

int displayDeck(struct gameState *state, int p) {
    int i;

    printf("DECK\n");
    for (i = 0; i < state->deckCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->deck[p][i]);
    }

    return 0;
}

int displayDiscard(struct gameState *state, int p) {
    int i;

    printf("DISCARD\n");
    for (i = 0; i < state->discardCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->discard[p][i]);
    }

    return 0;
}

int displayHand(struct gameState *state, int p) {
    int i;

    printf("HAND\n");
    for (i = 0; i < state->handCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->hand[p][i]);
    }

    return 0;
}
