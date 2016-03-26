/* -----------------------------------------------------------------------
 * test remodel
 *
 *
 * cardtest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "rngs.h"

int remodelTrash(struct gameState *state, int p, int controlCard, int trashCard, int toTest[], int expected[]);
int displayDeck(struct gameState *state, int p);

int main() {
    int i;
    int seed = 1000;

    int numPlayer = 2;
    int p, r;
    int k[10] = {adventurer, great_hall, cutpurse, gardens, mine
               , remodel, smithy, village, sea_hag, embargo};

    struct gameState G, testG;
    // Card enum - Name        - Cost
    // 22        - embargo     - 2
    // 14        - village     - 3
    // 13        - smithy      - 4
    // 11        - mine        - 5
    //  7        - adventurer  - 6
    // toTest[] is the list of cards we will try to gain
    int toTest[] = {embargo, village, smithy, mine, adventurer};
    // Keep track of which tests passed/failed
    int passValues[10];

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    printf("----------------- Testing remodel\n");

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Start testing
    p = 0;
    printf("\n----------------- Test 1 (Cost 2): Trash embargo\n");

    // choice1: 2, choice2: 2
    // Expect: 0
    // choice1: 2, choice2: 3
    // Expect: 0
    // choice1: 2, choice2: 4
    // Expect: 0
    // choice1: 2, choice2: 5
    // Expect: -1
    // choice1: 2, choice2: 6
    // Expect: -1
    int expected1[] = {0, 0, 0, -1, -1};

    // remodelTrash parameters:
    // 0 = game state
    // 1 = player
    // 2 = control card to fill the hand
    // 3 = card to trash
    // 4 = cards to gain
    // 5 = expected return values:
    // (0 if successfully trashed and gained; otherwise 1)
    // remodelTrash(0, 1, 2, 3, 4, 5);

    printf("CONTROL CARDS IN HAND ARE COPPERS:\n");
    passValues[0] = remodelTrash(&testG, p, copper, embargo, toTest, expected1);
    printf("\nCONTROL CARDS IN HAND ARE ESTATES:\n");
    passValues[1] = remodelTrash(&testG, p, estate, embargo, toTest, expected1);

    p = 0;
    printf("\n----------------- Test 2 (Cost 3): Trash village\n");

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // choice1: 3, choice2: 2
    // Expect: 0
    // choice1: 3, choice2: 3
    // Expect: 0
    // choice1: 3, choice2: 4
    // Expect: 0
    // choice1: 3, choice2: 5
    // Expect: 0
    // choice1: 3, choice2: 6
    // Expect: -1
    int expected2[] = {0, 0, 0, 0, -1};

    printf("CONTROL CARDS IN HAND ARE COPPERS:\n");
    passValues[2] = remodelTrash(&testG, p, copper, village, toTest, expected2);
    printf("\nCONTROL CARDS IN HAND ARE ESTATES:\n");
    passValues[3] = remodelTrash(&testG, p, estate, village, toTest, expected2);

    p = 0;
    printf("\n----------------- Test 3 (Cost 4): Trash smithy\n");

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // choice1: 4, choice2: 2
    // Expect: 0
    // choice1: 4, choice2: 3
    // Expect: 0
    // choice1: 4, choice2: 4
    // Expect: 0
    // choice1: 4, choice2: 5
    // Expect: 0
    // choice1: 4, choice2: 6
    // Expect: 0
    int expected3[] = {0, 0, 0, 0, 0};

    printf("CONTROL CARDS IN HAND ARE COPPERS:\n");
    passValues[4] = remodelTrash(&testG, p, copper, smithy, toTest, expected3);
    printf("\nCONTROL CARDS IN HAND ARE ESTATES:\n");
    passValues[5] = remodelTrash(&testG, p, estate, smithy, toTest, expected3);

    p = 0;
    printf("\n----------------- Test 4 (Cost 5): Trash mine\n");

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // choice1: 5, choice2: 2
    // Expect: 0
    // choice1: 5, choice2: 3
    // Expect: 0
    // choice1: 5, choice2: 4
    // Expect: 0
    // choice1: 5, choice2: 5
    // Expect: 0
    // choice1: 5, choice2: 6
    // Expect: 0
    int expected4[] = {0, 0, 0, 0, 0};

    printf("CONTROL CARDS IN HAND ARE COPPERS:\n");
    passValues[6] = remodelTrash(&testG, p, copper, mine, toTest, expected4);
    printf("\nCONTROL CARDS IN HAND ARE ESTATES:\n");
    passValues[7] = remodelTrash(&testG, p, estate, mine, toTest, expected4);

    p = 0;
    printf("\n----------------- Test 5 (Cost 6): Trash adventurer\n");

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // choice1: 5, choice2: 2
    // Expect: 0
    // choice1: 5, choice2: 3
    // Expect: 0
    // choice1: 5, choice2: 4
    // Expect: 0
    // choice1: 5, choice2: 5
    // Expect: 0
    // choice1: 5, choice2: 6
    // Expect: 0
    int expected5[] = {0, 0, 0, 0, 0};

    printf("CONTROL CARDS IN HAND ARE COPPERS:\n");
    passValues[8] = remodelTrash(&testG, p, copper, adventurer, toTest, expected5);
    printf("\nCONTROL CARDS IN HAND ARE ESTATES:\n");
    passValues[9] = remodelTrash(&testG, p, estate, adventurer, toTest, expected5);

    // Check if any tests failed (returned -1)
    for (i = 0; i < 10; i++) {
        if (passValues[i] == -1) {
            printf("Some test(s) failed!\n");
            return 0;
        }
    }

    // If all returned 0, then all tests passed
    printf("All tests passed!\n");

    return 0;
}

int remodelTrash(struct gameState *state, int p, int controlCard, int trashCard, int toTest[], int expected[]) {
    int i, j, k;
    int result;
    int initialDiscard = 0;
    int initialGained = 0;
    int initialHand = 5;
    int expectDiscard, expectGained, expectHand;
    bool pass = true;

    // toTest[] has 5 elements
    for (i = 0; i < 5; i++) {
        // Reset hand to look like:
        // remodel, trashCard, controlCard, controlCard, controlCard
        state->handCount[p] = 5;

        // Reset deck and discard pile
        state->deckCount[p] = 0;
        state->discardCount[p] = 0;

        // Set all cards in hand to same control card
        int control[MAX_HAND];
        for (j = 0; j < MAX_HAND; j++)
        {
            control[j] = controlCard;
        }

        memcpy(state->hand[p], control, sizeof(int) * MAX_HAND);

        // Put remodel in hand
        state->hand[p][0] = remodel;

        // Put card to trash in hand
        state->hand[p][1] = trashCard;

        // game state
        // player
        // card to trash: int choice1 = trashCard (testG.hand[p][1])
        // card to gain : int choice2 = toTest[]
        // remodel index: int handPos = 0 (testG.hand[p][0])
        // result = playRemodel(state, p, 1, toTest[i], 0);
          result = cardEffect(remodel, 1, toTest[i], 0, state, 0, 0);

        printf("\nchoice1 cost: %d, choice2 cost: %d\n", getCost(trashCard), getCost(toTest[i]));
        printf("Result: %d, Expected: %d\n", result, expected[i]);
        if (result != expected[i]) {
            printf("----------------- TEST FAILED!\n");
            pass = false;
        }

        // Check for gained cards in discard pile
        int gainedCount = 0;
        // Card enum - Name        - Cost
        // 22        - embargo     - 2
        // 14        - village     - 3
        // 13        - smithy      - 4
        // 11        - mine        - 5
        //  7        - adventurer  - 6
        for (k = 0; k < state->discardCount[p]; k++) {
            if (state->discard[p][k] == 7 || state->discard[p][k] == 11 || state->discard[p][k] == 13
                || state->discard[p][k] == 14 || state->discard[p][k] == 22) {
                gainedCount++;
            }
        }

        // Successful trash and gain
        // discardCount should be 2 because gained a card and played remodel
        // gainedCard should be 1
        // handCount should be 3 (-2 because played remodel and trashed a card)
        expectDiscard = initialDiscard + 2;
        expectGained = initialGained + 1;
        expectHand = initialHand - 2;

        if (expected[i] == 0) {
            printf("Discard count: %d, Expected: %d\n", state->discardCount[p], expectDiscard);
            if (state->discardCount[p] != expectDiscard) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
            printf("Gained count: %d, Expected: %d\n", gainedCount, expectGained);
            if (gainedCount != expectGained) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
            printf("Hand count: %d, Expected: %d\n", state->handCount[p], expectHand);
            if (state->handCount[p] != expectHand) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }

        // Unsuccessful - no trash/gain
        // discardCount should be 0
        // gainedCard should be 0
        // handCount should be 5 (keep remodel and could not trash card to gain card)
        expectDiscard = initialDiscard;
        expectGained = initialGained;
        expectHand = initialHand;

        if (expected[i] == -1) {
            printf("Discard count: %d, Expected: %d\n", state->discardCount[p], expectDiscard);
            if (state->discardCount[p] != expectDiscard) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
            printf("Gained count: %d, Expected: %d\n", gainedCount, expectGained);
            if (gainedCount != expectGained) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
            printf("Hand count: %d, Expected: %d\n", state->handCount[p], expectHand);
            if (state->handCount[p] != expectHand) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }

        // displayDeck(state, p);
    }

    if (pass) {
        return 0;
    }
    else {
        return -1;
    }
}

int displayDeck(struct gameState *state, int p) {
    int i;

    printf("DECK COUNT\n");
    for (i = 0; i < state->deckCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->deck[p][i]);
    }

    printf("DISCARD COUNT\n");
    for (i = 0; i < state->discardCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->discard[p][i]);
    }

    printf("HAND COUNT\n");
    for (i = 0; i < state->handCount[p]; i++)
    {
        printf("Position %d, Card: %d\n", i, state->hand[p][i]);
    }

    return 0;
}
