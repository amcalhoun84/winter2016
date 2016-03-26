/* -----------------------------------------------------------------------
 * test scoreFor()
 *
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
    int i;
    int seed = 1000;

    int numPlayer = 2;
    int p, r;
    int k[10] = {adventurer, great_hall, feast, gardens, mine
               , remodel, smithy, village, tribute, embargo};

    // const char *cards[] = {"curse", "estate", "duchy", "province", "copper", "silver", "gold", "adventurer", "council_room",
    //     "feast", "gardens", "mine", "remodel", "smithy", "village", "baron", "great_hall", "minion", "steward",
    //     "tribute", "ambassador", "cutpurse", "embargo", "outpost", "salvager", "sea_hag", "treasure_map"
    // };

    struct gameState G;
    // Count of cards to buy
    int estateCount, duchyCount, provinceCount, ghCount, gardensCount, curseCount;
    // Test function calculation
    int result;
    // Correct calculation
    int score;
    // For keeping track of gardens effect
    int deck_discard_hand;
    // Check for pass/fail
    bool pass = true;
    int failed = 0;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // TESTING PLAYER 0
    p = 0;

#if (NOISY_TEST == 1)
    printf("----------------- TESTING scoreFor():\n");
    printf("\n----------------- Test 1 - Player %d:\n", p);
    printf("Adding the following to discard:\n");
    printf("    1 duchy\n");
    printf("    2 provinces\n");
    printf("    5 great halls\n");
    printf("    4 gardens\n");
    printf("    2 curses\n");
#endif

    // From initializeGame()
    estateCount = 3;

    // Buy duchy
    duchyCount = 1;
    G.discard[p][ G.discardCount[p] ] = duchy;
    G.discardCount[p]++;

    // Buy province
    provinceCount = 2;
    for (i = 0; i < provinceCount; i++) {
        G.discard[p][ G.discardCount[p] ] = province;
        G.discardCount[p]++;
    }

    // Buy great_hall
    ghCount = 5;
    for (i = 0; i < ghCount; i++) {
        G.discard[p][ G.discardCount[p] ] = great_hall;
        G.discardCount[p]++;
    }

    // Buy gardens
    gardensCount = 4;
    for (i = 0; i < gardensCount; i++) {
        G.discard[p][ G.discardCount[p] ] = gardens;
        G.discardCount[p]++;
    }

    // Buy curse
    curseCount = 2;
    for (i = 0; i < curseCount; i++) {
        G.discard[p][ G.discardCount[p] ] = curse;
        G.discardCount[p]++;
    }

#if (NOISY_TEST == 1)
    // 27 types of cards
    // printf("Totals of cards by type for Player %d:\n", p);
    // for (i = 0; i < 27; i++) {
    //     result = fullDeckCount(p, i, &G);
    //     printf("Card count for %s: %d\n", cards[i], result);
    // }
#endif

    // Test scoreFor()
    result = scoreFor(p, &G);

    // Calculate correct score
    score = 0;
    score = score + (curseCount * -1);
    score = score + estateCount;
    score = score + (duchyCount * 3);
    score = score + (provinceCount * 6);
    score = score + ghCount;
#if (NOISY_TEST == 1)
    printf("deckCount: %d\n", G.deckCount[p]);
    printf("discardCount: %d\n", G.discardCount[p]);
    printf("handCount: %d\n", G.handCount[p]);
#endif
    deck_discard_hand = G.deckCount[p] + G.discardCount[p] + G.handCount[p];
    printf("Total Deck: %d\n", deck_discard_hand);
   // printf("Gardens: %d\n", (deck_discard_hand /10) * gardensCount);

    int retGardens = ((deck_discard_hand /10) * gardensCount);
    score += retGardens;

    // Verify
#if (NOISY_TEST == 1)
    printf("Player %d score: %d, expected: %d\n", (p+1), result, score);
#endif
    // Assert here halts program execution
    // assert(result == score);
    if (result != score) {
        pass = false;
        failed++;
    }

    // TESTING PLAYER 1
    p = 1;

#if (NOISY_TEST == 1)
    // 27 types of cards
    // printf("Test 2\nTotals of cards by type for Player %d:\n", p);
    // for (i = 0; i < 27; i++) {
    //     result = fullDeckCount(p, i, &G);
    //     printf("Card count for %s: %d\n", cards[i], result);
    // }
#endif

    // Test scoreFor()
    result = scoreFor(p, &G);

    // From initializeGame()
    curseCount = 0;
    estateCount = 3;
    duchyCount = 0;
    provinceCount = 0;
    ghCount = 0;
    gardensCount = 0;

    // Calculate correct score
    score = 0;
    score = score + (curseCount * -1);
    score = score + estateCount;
    score = score + (duchyCount * 3);
    score = score + (provinceCount * 6);
    score = score + ghCount;
#if (NOISY_TEST == 1)
    printf("\n----------------- Test 2 - Player %d:\n", p);
    printf("No changes and all cards in deck:\n");
    printf("deckCount: %d\n", G.deckCount[p]);
    printf("discardCount: %d\n", G.discardCount[p]);
    printf("handCount: %d\n", G.handCount[p]);
#endif
    deck_discard_hand = G.deckCount[p] + G.discardCount[p] + G.handCount[p];
    score = score + ((deck_discard_hand / 10) * gardensCount);

    // Verify
#if (NOISY_TEST == 1)
    printf("Player %d score: %d, expected: %d\n", p, result, score);
#endif
    // Assert here halts program execution
    // assert(result == score);
    if (result != score) {
        pass = false;
        failed++;
    }

    if (pass) {
        printf("\nAll tests passed!\n");
    }
    else {
        printf("\n%d/2 test(s) failed!\n", failed);
    }

    return 0;
}
