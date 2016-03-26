/*Jonathan Lagrew
 *test fullDeckCount()
 *Notes:
 *Testing that fullDeckCount works correctly for 2 deck, 2 hand and 2 discard pile cards. 
 card numbers: 1 estate, 4 copper, 6 gold, province 3, smithy 13, tribute 19, embargo 22
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
    int seed = 1000; // seed for random number
    int numPlayer = 3;
    int p, r;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, tribute, embargo};
    struct gameState G; 
    int copperCount, estateCount;// copper count and estate count
    int count;
    int result;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

#if (NOISY_TEST == 1)
    printf("\nTESTING fullDeckCount():\n");
#endif

    for (p = 0; p < numPlayer; p++)// loop through players, for testing set to 2 players, 0 and 1 
    {
#if (NOISY_TEST == 1)
        printf("\nTesting for Player %d:\n", p);// player 0 then player 1
        printf("Adding the following to the\n");
        printf("deck:         5 coppers\n");
        printf("hand:         10 golds\n");
        printf("discard pile: 2 provinces\n");
        printf("hand:         3 smithys\n");
        printf("discard pile: 4 tributes\n");
        printf("deck:         5 embargos\n");
		printf("Test results:\n");
#endif

        estateCount = 3; //setting estate count to 3
        result = fullDeckCount(p, 1, &G); //result returning estate number
		/* Here deck = hand + discard + deck */
#if (NOISY_TEST == 1)
        printf("estate result: %d, expected: %d\n", result, estateCount); 
#endif
        assert(result == estateCount);

        copperCount = 7; //start with 2 coppers plus the 5 added
        result = fullDeckCount(p, 4, &G);
#if (NOISY_TEST == 1)
        printf("copper result: %d, expected: %d\n", result, copperCount);
#endif
        assert(result == copperCount);

        // Test update of coppers (deck)
        count = 5;
        for (i = 0; i < count; i++) {
            G.deck[p][ G.deckCount[p] ] = copper;
            G.deckCount[p]++;
        }
        copperCount = copperCount + count;
        result = fullDeckCount(p, 4, &G); // copper card number 4
#if (NOISY_TEST == 1)
        printf("updated copper result: %d, expected: %d\n", result, copperCount);
#endif
        assert(result == copperCount);

        // Test update of golds (hand)
        count = 10;
        for (i = 0; i < count; i++) {
            G.hand[p][ G.handCount[p] ] = gold;
            G.handCount[p]++;
        }
        result = fullDeckCount(p, 6, &G); // gold card number 6
#if (NOISY_TEST == 1)
        printf("gold result: %d, expected: %d\n", result, count);
#endif
        assert(result == count);

        count = 2;
        for (i = 0; i < count; i++) {
            G.discard[p][ G.discardCount[p] ] = province; // test provine in discard pile
            G.discardCount[p]++;
        }
        result = fullDeckCount(p, 3, &G); // province card number 3
#if (NOISY_TEST == 1)
        printf("province result: %d, expected: %d\n", result, count);
#endif
        assert(result == count);

        count = 3;
        for (i = 0; i < count; i++) {
            G.hand[p][ G.handCount[p] ] = smithy; // test smithy in hand
            G.handCount[p]++;
        }
        result = fullDeckCount(p, 13, &G); // smithy card number 13
#if (NOISY_TEST == 1)
        printf("smithy result: %d, expected: %d\n", result, count);
#endif
        assert(result == count);

        count = 4;
        for (i = 0; i < count; i++) {
            G.discard[p][ G.discardCount[p] ] = tribute; // test tribute in discard pile
            G.discardCount[p]++;
        }
        result = fullDeckCount(p, 19, &G); // tribute card number 19
#if (NOISY_TEST == 1)
        printf("tribute result: %d, expected: %d\n", result, count);
#endif
        assert(result == count);

        count = 5; // setting embargo count to 5 
        for (i = 0; i < count; i++) { // test embargo in deck
            G.deck[p][ G.deckCount[p] ] = embargo;
            G.deckCount[p]++;
        }
        result = fullDeckCount(p, 22, &G); // embargo card number 22
#if (NOISY_TEST == 1)
        printf("embargo result: %d, expected: %d\n", result, count);
#endif
        assert(result == count);

    }

    printf("\nAll tests successful!\n");

    return 0;
}
