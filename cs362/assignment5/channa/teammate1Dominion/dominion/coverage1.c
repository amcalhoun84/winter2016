Nancy Chan
CS 362
Winter 2016
Assignment 3: coverage1.c

Aggregate coverage for all 8 test files:
Lines executed:37.04% of 575
Branches executed:47.96% of 417
Taken at least once:36.21% of 417
Calls executed:21.05% of 95

cardtest1:
	100% statement coverage of playSmithy()
	branch coverage: function playSmithy called 1 returned 100% blocks executed 100%

cardtest2:
	statements not covered in playAdventurer():
		-: 1258:      //if the deck is empty we need to shuffle discard and add to deck
	#####: 1259:      shuffle(currentPlayer, state);
		6: 1274:  while(z-1>=0)
		-: 1275:  {
	#####: 1276:    state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
	#####: 1277:    z=z-1;
		-: 1278:  }
	I did not use an empty deck in any of the tests here so shuffle was never called. The bug I introduced
	made it so that the counter z does not increment correctly. As a result, z, which was intialized to 0,
	stays at 0 and the while conditiion cannot be met for the following two lines below it to execute.
	
	branch coverage: function playAdventurer called 3 returned 100% blocks executed 86%
	Not all branches were taken because 1) I did not use an empty deck at any point, 2) the only treasure
	card used was copper (so silver and gold were not tested), and 3) I introduced a bug affecting the
	z counter.

	boundary coverage: I checked the boundaries of different placements of coppers in the deck (top of the deck,
	bottom of the deck, and interspersed through the deck).

cardtest3:
	statements not covered in playCutpurse():
	#####: 1327:          for (k = 0; k < state->handCount[i]; k += 2)
		-: 1328:          {
	#####: 1329:            if (DEBUG)
		-: 1330:              printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
		-: 1331:          } 
	#####: 1332:          break;
	Although I did have a test involving a hand with no copper that should have resulted in the player
	revealing the cards in their hand, the condition required for the for loop to execute: 
	if (j == state->handCount[i]) was not satisfied. j would increase to state->handCount[i] - 1 but not
	state->handCount[i]. We do not see j reaching state->handCount[i] because the outer for loop condition:
	j < state->handCount[i] prevents us from evaluating if (j == state->handCount[i]) again once we have
	examined all the cards.

	branch coverage: function playCutpurse called 4 returned 100% blocks executed 74%
	The branch not taken is the same code block described above in statement coverage.

	boundary coverage: I checked the boundaries of having enough coppers to discard as well as
	having no coppers to discard.

cardtest4:
	100% statement coverage of playRemodel()
	branch coverage: function playRemodel called 50 returned 100% blocks executed 100%
	boundary coverage: I checked the boundaries of attempting to trash cards to gain cards that
	had a value of less than + 2 the value of the trashed card, exactly + 2 the value of the
	trashed card, and more than + 2 the value of the trashed card.

unittest1:
	100% statement coverage of updateCoins()
	branch coverage: function updateCoins called 63 returned 100% blocks executed 100%
	boundary coverage: I tested the boundaries by using different combinations of coins.
	I would start with coppers and gradually replace them with silvers and golds until
	they were all silver or all gold instead of all copper.

unittest2:
	100% statement coverage of fullDeckCount()
	branch coverage: function fullDeckCount called 20 returned 100% blocks executed 100%

unittest3:
	100% statement coverage of isGameOver()
	branch coverage: function isGameOver called 9 returned 100% blocks executed 100%
	boundary coverage: I tested the boundaries by using an empty province pile, an almost
	empty province pile, and different numbers of other supply piles to check the minimum
	numbers and types of empty piles that would cause the game to be over.

unittest4:
	100% statement coverage of scoreFor()

	branch coverage: function scoreFor called 2 returned 100% blocks executed 73%
	For scoreFor(), I tested 2 players' scores. Player 1's victory cards included
	estate, duchy, province, great hall, gardens, and curse. Player 2's victory cards
	included only the default estate. Not all branches were taken because I only
	focused on adding cards to Player 1's discard pile and letting cards remain in
	Player 2's deck but scoreFor() checks player's decks, discard piles, and hands.
