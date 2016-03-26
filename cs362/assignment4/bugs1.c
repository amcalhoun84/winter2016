Andrew M. Calhoun
calhouna@oregonstate.edu
CS 362 - Assignment 3
bugs1.c

Bugs Found:
    updateCoins() - unitTest1.c
        No glaring errors or bugs found. Originally, there was an error found with failing tests, but it was a result of my unit test improperly comparing a value. Once this was fixed, no errors were found.

    discardCard() - unitTest2.c
        No bugs found during testing.

    buyCard() - unitTest3.c
        No bugs found during testing. All buys are successful as long as player has enough coins to make the purchase.

    endTurn() - unitTest4.c
        No bugs found during unit testing.

    playAdventurer() - cardTest1.c
        Adventurer card will give two drawnTreasures instead of one, causing the loop to terminate early. This affects the seeding results in games, as well as affects the ratio.
        As an interesting note, the deck count is randomized within the check itself to ensure consistency in testing. Also, the game shuffle the cards again if the player has one card.
        It should be only if the player has less than 1 card. This can result in strange results depending on the seed. The test can sometimes succeed 100% of the time, othertimes it fails
        frequently. The chosen seed is randomized and reset during each test. 20 tests are given.

    playSmithy() - cardTest2.c
        Smithy does not properly add 3 cards due to an incrementation error. Rather than using a post-increment (i++), the code is using a prefix increment (++i), this results in only two cards
        being given. During tests, a +4 is taken into account because the card is granted to the player. If the player does not have 4 more cards in their hand during a test, then the test fails.

    playVillage() - cardTest3.c
        The village is not properly discarded, which causes it to remain in the player's hand.

    playMinion() - cardTest4.c
        The minion proved to be very buggy. The minion properly increments actions, but removes coins from the player's stash under choice 1. Under choice 2, it does not properly grant cards or discard them. This
        results in other players ending up with additional cards, on top of the one's they already have in their hand and does not force other players to discard their cards. The current way the system is set up as well,
        it does not appear that cards are properly incremented into player hands.

