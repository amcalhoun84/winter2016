/*Jonathan Lagrew
 *test isGameOver()
 *Notes:
 *Testing that game correctly ends when all the province cards are empty 
 *or the game will end when the three supply piles are empty.
 *Testing all possible combinations to make the game end. 
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
    int seed = 1000; // random seed
    int numPlayer = 2; 
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, tribute, embargo};
    struct gameState G;
    int result;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

#if (NOISY_TEST == 1)
    printf("\nTESTING isGameOver():\n");
	printf("Game over = 1\n");
#endif

    // test if game ends with no changes
    result = isGameOver(&G);
#if (NOISY_TEST == 1)
    printf("Test #1: Nothing changed\n");
    printf("Game is over: %d, expected: 0\n\n", result);
#endif
    assert(result == 0); //expect to not end 

    // test with 0 provinces
    G.supplyCount[province] = 0;
    result = isGameOver(&G);
#if (NOISY_TEST == 1)
    printf("Test 2: 0 Provinces\n");
    printf("Game is over: %d, expected: 1\n\n", result);
#endif
    assert(result == 1); //expect to end

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // test with 1 empty pile
    G.supplyCount[adventurer] = 0;
    result = isGameOver(&G);
#if (NOISY_TEST == 1)
    printf("Test 3: 1 Empty pile\n");
    printf("Game is over: %d, expected: 0\n\n", result);
#endif
    assert(result == 0); // expect to not end

    // test with 2 empty piles
    G.supplyCount[village] = 0;
    result = isGameOver(&G);
#if (NOISY_TEST == 1)
    printf("Test 4: 2 empty piles\n");
    printf("Game is over: %d, expected: 0\n\n", result);
#endif
    assert(result == 0); // expect to not end

    // test with 3 empty supply piles
    G.supplyCount[great_hall] = 0;
    result = isGameOver(&G);
#if (NOISY_TEST == 1)
    printf("Test 5: 3 empty piles\n");
    printf("Game is over: %d, expected: 1\n\n", result);
#endif
    assert(result == 1); // expect to end


    printf("All tests passed!\n");

    return 0;
}
