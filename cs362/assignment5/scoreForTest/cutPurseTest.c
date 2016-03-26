/* -----------------------------------------------------------------------
 * test cutpurse
 *
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "rngs.h"

int cutpurseDiscard(int numPlayer, int testNum, int handNum, bool *pass);

// set NOISY_TEST to 0 to remove full deck printfs from output
// (0 to show hands only not decks and discard piles)
#define NOISY_TEST 0

int main() {
    int i;
    int seed = 1000;

    int numPlayer = 4;
    int p, r;
    int k[10] = {adventurer, great_hall, cutpurse, gardens, mine
               , remodel, smithy, village, sea_hag, embargo};

    struct gameState G, testG;
    int copperCount;
    int copperList[4];
    int initialCoins, updatedCoins;
    // For test numbers and number of hands
    int testNum, handNum;
    bool pass = true;

    printf("----------------- Testing cutpurse\n");

    // Test number: 1, 2, 3...
    testNum = 1;
    // Test variations on
    // number of hands: 4, 3, 2...
    handNum = 4;

    cutpurseDiscard(4, testNum, handNum, &pass);
    testNum++;
    handNum--;
    cutpurseDiscard(3, testNum, handNum, &pass);
    testNum++;
    handNum--;
    cutpurseDiscard(2, testNum, handNum, &pass);

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Start testing
    p = 0;
    printf("\n----------------- Test 4: hand with no coppers\n");

    // Change number of players
    numPlayer = 4;

    // Put cutpurse in hand of player 0
    testG.hand[p][ testG.handCount[p] ] = cutpurse;
    testG.handCount[p]++;

    // Make player 1 have no coppers to reveal the hand
    p = 1;
    for (i = 0; i < testG.deckCount[p]; i++) {
        testG.deck[p][i] = estate;
    }

    // Move players' cards from decks to hands
    // so that we can check their hands for coppers to discard
    for (p = 1; p < numPlayer; p++) {
        for (i = 0; i < 10; i++) {
            testG.hand[p][i] = testG.deck[p][i];//Add card to hand
            testG.deckCount[p]--;
            testG.handCount[p]++;//Increment hand count
        }
    }

    // Check pile counts
    printf("----------------- AFTER PUT CUTPURSE IN HAND FOR PLAYER 0:\n");
    printf("----------------- AFTER MAKE A HAND WITH NO COPPERS FOR PLAYER 1:\n");

    // Initial coins
    initialCoins = testG.coins;
    printf("Initial coins: %d\n", initialCoins);

    for (p = 0; p < numPlayer; p++) {

        copperCount = 0;

        printf("----------------- Player %d\n", p);

#if (NOISY_TEST == 1)
        printf("DECK COUNT\n");
        for (i = 0; i < testG.deckCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.deck[p][i]);
        }

        printf("DISCARD COUNT\n");
        for (i = 0; i < testG.discardCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.discard[p][i]);
        }
#endif

        printf("HAND COUNT\n");
        for (i = 0; i < testG.handCount[p]; i++)
        {
#if (NOISY_TEST == 1)
            printf("Position %d, Card: %d\n", i, testG.hand[p][i]);
#endif
            if (testG.hand[p][i] == copper) {
                copperCount++;
            }
        }

        copperList[p] = copperCount;
        printf("Copper count: %d\n", copperCount);
    }

    // Play cutpurse
    p = 0;
    // playCutpurse(p, &testG, 5);
    cardEffect(cutpurse, 0, 0, 0, &testG, 5, 0);


    // Check pile counts
    printf("----------------- AFTER PLAY CUTPURSE\n");

    // cutpurse gives +2 coins
    updatedCoins = initialCoins + 2;
    printf("Updated coins: %d, Expected: %d\n", testG.coins, updatedCoins);
    assert(testG.coins == updatedCoins);

    for (p = 0; p < numPlayer; p++) {

        copperCount = 0;

        printf("----------------- Player %d\n", p);

#if (NOISY_TEST == 1)
        printf("DECK COUNT\n");
        for (i = 0; i < testG.deckCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.deck[p][i]);
        }

        printf("DISCARD COUNT\n");
        for (i = 0; i < testG.discardCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.discard[p][i]);
        }
#endif

        printf("HAND COUNT\n");
        for (i = 0; i < testG.handCount[p]; i++)
        {
#if (NOISY_TEST == 1)
            printf("Position %d, Card: %d\n", i, testG.hand[p][i]);
#endif
            if (testG.hand[p][i] == copper) {
                copperCount++;
            }
        }

        // If not Player 0, then each other player discards a copper
        // Player 1 should be revealing hand with no coppers
        if (p != 0 && p != 1) {
            printf("Copper count: %d, Expected: %d\n", copperCount, copperList[p] - 1);
            if (copperCount != copperList[p] - 1) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }
        // Player 0 played cutpurse and so copper count should not change
        // Player 1 has no coppers so copper count should not change
        else {
            printf("Copper count: %d, Expected: %d\n", copperCount, copperList[p]);
            if (copperCount != copperList[p]) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }
    }

    // Check if satisfied condition for printing: Player %d reveals card number %d\n
    // Condition: if (j == state->handCount[i])
    // Player 1 has a hand with no coppers
    p = 1;
    int j;
    // Use logic from playCutpurse
    for (i = 0; i <= testG.handCount[p]; i++)
    {
        // If there is a copper in the hand, then j will never reach handCount
        if (testG.hand[p][i] == copper) {
            break;
        }

        // If there are no coppers in the hand, then j should reach handCount
        j = i;
    }

    // No coppers so we expect to satisfy the condition for revealing cards
    printf("Check if satisfied condition for revealing cards\n");
    printf("(j == handCount):\n");
    printf("j = %d, handCount: %d\n", j, testG.handCount[p]);
    if (j != testG.handCount[p]) {
        printf("----------------- TEST FAILED!\n");
        pass = false;
    }

    if (pass) {
        printf("\nAll tests passed!\n");
    }
    else {
        printf("\nSome test(s) failed!\n");
    }

    return 0;
}

int cutpurseDiscard(int numPlayer, int testNum, int handNum, bool *pass) {
    int i;
    int seed = 1000;

    int p, r;
    int k[10] = {adventurer, great_hall, cutpurse, gardens, mine
               , remodel, smithy, village, sea_hag, embargo};

    struct gameState G, testG;
    int copperCount;
    int copperList[4];
    int initialCoins, updatedCoins;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // Copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));

    // Start testing
    p = 0;
    printf("\n----------------- Test %d: %d hands with coppers\n", testNum, handNum);

    // Put cutpurse in hand of player 0
    testG.hand[p][ testG.handCount[p] ] = cutpurse;
    testG.handCount[p]++;

    // Move players' cards from decks to hands
    // so that we can check their hands for coppers to discard
    for (p = 1; p < numPlayer; p++) {
        for (i = 0; i < 10; i++) {
            testG.hand[p][i] = testG.deck[p][i];//Add card to hand
            testG.deckCount[p]--;
            testG.handCount[p]++;//Increment hand count
        }
    }

    // Check pile counts
    printf("----------------- AFTER PUT CUTPURSE IN HAND FOR PLAYER 0:\n");
    printf("----------------- AFTER MOVE CARDS FROM DECKS TO HANDS FOR OTHER PLAYER(S):\n");

    // Initial coins
    initialCoins = testG.coins;
    printf("Initial coins: %d\n", initialCoins);

    for (p = 0; p < numPlayer; p++) {

        copperCount = 0;

        printf("----------------- Player %d\n", p);

#if (NOISY_TEST == 1)
        printf("DECK COUNT\n");
        for (i = 0; i < testG.deckCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.deck[p][i]);
        }

        printf("DISCARD COUNT\n");
        for (i = 0; i < testG.discardCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.discard[p][i]);
        }
#endif

        printf("HAND COUNT\n");
        for (i = 0; i < testG.handCount[p]; i++)
        {
#if (NOISY_TEST == 1)
            printf("Position %d, Card: %d\n", i, testG.hand[p][i]);
#endif
            if (testG.hand[p][i] == copper) {
                copperCount++;
            }
        }

        copperList[p] = copperCount;
        printf("Copper count: %d\n", copperCount);
    }

    // Play cutpurse
    p = 0;
    //playCutpurse(p, &testG, 5);
    cardEffect(cutpurse, 0, 0, 0, &testG, 5, 0);

    // Check pile counts
    printf("----------------- AFTER PLAY CUTPURSE\n");

    // cutpurse gives +2 coins
    updatedCoins = initialCoins + 2;
    printf("Updated coins: %d, Expected: %d\n", testG.coins, updatedCoins);
    assert(testG.coins == updatedCoins);

    for (p = 0; p < numPlayer; p++) {

        copperCount = 0;

        printf("----------------- Player %d\n", p);

#if (NOISY_TEST == 1)
        printf("DECK COUNT\n");
        for (i = 0; i < testG.deckCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.deck[p][i]);
        }

        printf("DISCARD COUNT\n");
        for (i = 0; i < testG.discardCount[p]; i++)
        {
            printf("Position %d, Card: %d\n", i, testG.discard[p][i]);
        }
#endif

        printf("HAND COUNT\n");
        for (i = 0; i < testG.handCount[p]; i++)
        {
#if (NOISY_TEST == 1)
            printf("Position %d, Card: %d\n", i, testG.hand[p][i]);
#endif
            if (testG.hand[p][i] == copper) {
                copperCount++;
            }
        }

        // If not Player 0, then each other player discards a copper
        if (p != 0) {
            printf("Copper count: %d, Expected: %d\n", copperCount, copperList[p] - 1);
            if (copperCount != copperList[p] - 1) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }
        // Player 0 played cutpurse and so copper count should not change
        else {
            printf("Copper count: %d, Expected: %d\n", copperCount, copperList[p]);
            if (copperCount != copperList[p]) {
                printf("----------------- TEST FAILED!\n");
                pass = false;
            }
        }
    }

    return 0;
}
