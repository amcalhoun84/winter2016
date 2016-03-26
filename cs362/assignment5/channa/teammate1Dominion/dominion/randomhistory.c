Nancy Chan
CS 362
Winter 2016
Assignment 4: randomhistory.c

Adding the random testers improved coverage in all aspects:

Assignment 3:
Aggregate coverage for all 8 test files:
Lines executed:37.04% of 575
Branches executed:47.96% of 417
Taken at least once:36.21% of 417
Calls executed:21.05% of 95

Assignment 4:
Aggregate coverage for all 10 test files:
Lines executed:41.57% of 575
Branches executed:48.92% of 417
Taken at least once:41.01% of 417
Calls executed:23.16% of 95

randomtestadventurer:

	I developed the random tester by creating a deck of 10 random cards, an empty discard pile,
	and a fixed hand of 5 cards. I then saved pre-test values to compare with post-test values.
	I calculated expected values for three scenarios: 1) no treasures in the deck, 2) one treasure
	in the deck, and 3) at least 2 treasures in the deck. I compared the expected values with the
	actual values in the changes to the deck, discard, and hand after playing the adventurer card. 
	
	In terms of coverage, for Assignment 3, I had not tested the behavior of adventurer in the case
	of an empty deck and so the shuffle line in the playAdventurer function had not been executed.
	Adding the random tester gave a more realistic assortment of cards which could result in an
	empty deck as the player may go through the entire deck in a deck containing no treasures. This
	improved coverage as the following line which was previously not executed is now executed:

	-: 1258:      //if the deck is empty we need to shuffle discard and add to deck
   20: 1259:      shuffle(currentPlayer, state);

   However, the last section of code in playAdventurer is still not executed and remains inaccessible
   because of the bug that I introduced in removing an incrementing statement for z:

  200: 1274:  while(z-1>=0)
	-: 1275:  {
#####: 1276:    state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
#####: 1277:    z=z-1;
	-: 1278:  }

	All branches are taken at least some percentage of the time except for the branch in which the above
	while(z-1>=0) condition is satisfied.

	Effort to check the correctness of the specification:

	Examples of scenarios that the random testers cover:
		At least 2 treasures: Random Deck Test 1
			- Deck count and hand count are correct. Discard pile is incorrect.
		Exactly 2 treasures: Random Deck Test 6
			- Deck count and hand count are correct. Discard pile is incorrect.
		1 treasure: Random Deck Test 16
			- Deck count and hand count are correct. Discard pile is incorrect.
		No treasures: Random Deck Test 2
			- Deck count is correct. Discard pile and hand count are incorrect.

	Note: The deck, discard pile, and hand counts are all correct only when there are at least 2 treasures at the top of
	the deck (Example: Random Test 20) because in that instance, there is no need to discard other revealed cards and so
	the bug in not correctly discarding cards does not occur.

randomtestcard:

	I developed the random tester by creating a hand of 10 random cards, I then put the remodel card in index 0 of the
	hand (since the call to playRemodel requires passing its index). I then randomly select a card in the hand to trash
	and randomly select a card to try to gain (from an array of cards representing each possible cost from 0 to 8).
	Pre-test values are saved in order to compare them with post-test values. I created a function called trashOK()
	which looks up the cost of the card to trash and card to gain and determines if it is possible to trash that card to
	gain that other card according to the remodel card logic. I then check the deck, discard pile, and hand, comparing
	expected values with actual values resulting from changes to the cards after playing the remodel card. If it is
	possible to trash a card to gain a card, then there should be no change to the deck, + 2 added to the discard because
	a card was gained and remodel was played, and - 2 subtracted from the hand because a card was trashed and remodel
	was played. If it is not possible, then there should be no changes to the deck, discard pile, and hand.

	In terms of coverage, the gcov file shows 100% statement coverage of playRemodel() and "function playRemodel called
	1000 returned 100% blocks executed 100%". playRemodel is called many more times now because using a random tester
	generates a larger number of different test cases.

	Effort to check the correctness of the specification:

	choice1 = trashCard
	choice2 = gainCard
	Cost of trashCard + 2 < Cost of gainCard:
		Random Test 75
			- choice1 cost: 2, choice2 cost: 5
			- Result: 0, Expected: -1
		Random Test 661
			- choice1 cost: 2, choice2 cost: 5
			- Result: -1, Expected: -1

	Cost of gainCard = Cost of trashCard + 2:
		Random Test 51
			- choice1 cost: 3, choice2 cost: 5
			- Result: -1, Expected: 0
		Random Test 94
			- choice1 cost: 3, choice2 cost: 5
			- Result: 0, Expected: 0

	Cost of gainCard < Cost of trashCard + 2:
		Random Test 3
			- choice1 cost: 4, choice2 cost: 0
			- Result: 0, Expected: 0
		Random Test 9
			- choice1 cost: 4, choice2 cost: 0
			- Result: -1, Expected: 0

	Testing reveals that there are inconsistencies in the behavior of playRemodel. We may have the same combination of
	the cost of choice1 and choice2 but different results. We see more inconsistencies now that the hand is made of
	random cards rather than consisting of the same control card which is the approach I used for Assignment 3.
	Random testing confirms that even though we should only be examining the card to trash and the card to gain, the
	other cards in the hand influence the result which violates the intended logic of remodel and exposes the bug that
	that playRemodel is using the wrong index for the hand.
