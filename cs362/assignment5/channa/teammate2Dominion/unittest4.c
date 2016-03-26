/*Jonathan Lagrew
 *test scoreFor()
 *Notes: 
 *Testing that the score is calculated correctly for each curse, estate,
 *duchy, province, great hall or gardens. 
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
    int numPlayer = 3;
    int p, r;
    int k[10] = {adventurer, great_hall, feast, gardens, mine, remodel, smithy, village, tribute, embargo};   
    struct gameState G;
    int estateCount, duchyCount, provinceCount, ghCount, gardensCount, curseCount; // counter for cards
    int result; // test result value
    int score; // score calculation 
    int garden_effect; // to keep track of garden card effect
    bool pass = true; // bool for pass or fail
    int failed = 0;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // ----PLAYER 0-----
    p = 0;

#if (NOISY_TEST == 1)
    printf("\nTESTING scoreFor():\n");
    printf("\nTest 1 - Player %d:\n", p);
    printf("Putting in the discard pile:\n");
    printf("    5 duchy\n");
    printf("    3 provinces\n");
    printf("    1 great halls\n");
    printf("    2 gardens\n");
    printf("    1 curses\n");
#endif
    estateCount = 3; // 3 from initialization 
    duchyCount = 5; // duchy
    G.discard[p][ G.discardCount[p] ] = duchy;
    G.discardCount[p]++;
	
    provinceCount = 3; // province
    for (i = 0; i < provinceCount; i++) {
        G.discard[p][ G.discardCount[p] ] = province;
        G.discardCount[p]++;
    }

    ghCount = 1; // great hall
    for (i = 0; i < ghCount; i++) {
        G.discard[p][ G.discardCount[p] ] = great_hall;
        G.discardCount[p]++;
    }

    gardensCount = 2; // gardens
    for (i = 0; i < gardensCount; i++) {
        G.discard[p][ G.discardCount[p] ] = gardens;
        G.discardCount[p]++;
    }

    curseCount = 1; // curse
    for (i = 0; i < curseCount; i++) {
        G.discard[p][ G.discardCount[p] ] = curse;
        G.discardCount[p]++;
    }

    result = scoreFor(p, &G); // Storing scoreFor() test in result
    
    score = 0; // Calculate correct score to compare to result 
    score = score + (curseCount * -1); // curse -1
    score = score + estateCount;
    score = score + (duchyCount * 3); // duchy * 3
    score = score + (provinceCount * 6); //province * 6
    score = score + ghCount;
#if (NOISY_TEST == 1)
    printf("deckCount: %d\n", G.deckCount[p]); // print deck count
    printf("discardCount: %d\n", G.discardCount[p]); // print discard count
    printf("handCount: %d\n", G.handCount[p]); // print hand count
#endif
    garden_effect = G.deckCount[p] + G.discardCount[p] + G.handCount[p]; // deck count + discard count + hand count for garden 
    score = score + (garden_effect / 10 * gardensCount); // total count divided by 10 then times garden count

#if (NOISY_TEST == 1)
    printf("Player %d score: %d, expected: %d\n", p, result, score); // compare actual score to expected 
#endif
    if (result != score) {
        pass = false;
        failed++;
    }

	
    // ------PLAYER 1------
    p = 1;
	
    result = scoreFor(p, &G);

    curseCount = 0;
    estateCount = 3;
    duchyCount = 0;
    provinceCount = 0;
    ghCount = 0;
    gardensCount = 0;

    score = 0;
    score = score + (curseCount * -1);
    score = score + estateCount;
    score = score + (duchyCount * 3);
    score = score + (provinceCount * 6);
    score = score + ghCount;
#if (NOISY_TEST == 1)
    printf("\nTest 2 - Player %d:\n", p);
    printf("All cards in deck, no additional card changes:\n");
    printf("deckCount: %d\n", G.deckCount[p]);
    printf("discardCount: %d\n", G.discardCount[p]);
    printf("handCount: %d\n", G.handCount[p]);
#endif
    garden_effect = G.deckCount[p] + G.discardCount[p] + G.handCount[p];
    score = score + (garden_effect / 10 * gardensCount);

#if (NOISY_TEST == 1)
    printf("Player %d score: %d, expected: %d\n", p, result, score);
#endif
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
