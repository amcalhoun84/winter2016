Nancy Chan
CS 362
Winter 2016
Assignment 3: bug1.c

Bugs found:

scoreFor():
	unittest4 detected a bug in the calculation of the gardens effect. A gardens card is worth
	1 victory point for every 10 cards in the player's deck (the player's discard pile and hand
	are a part of the player's deck at that point) rounded down. Instead of counting all of the
	player's cards and then dividing by 10. scoreFor() uses fullDeckCount() to only get the
	player's total number of curse cards and then dividing by 10.

	unittest4 also detected a bug in the score calculation of the player's deck. scoreFor() fails
	to consider cards in the player's deck when the player's discard pile count is zero.

playSmithy():
	cardtest1 detected incorrect counts of the player's cards after playing smithy. Instead of 7
	cards in the deck, there were 6. Instead of 8 cards in the hand, there were 9. This means that
	there was an extra card in the hand and one missing card from the deck and so smithy drew 4
	cards instead of 3 since the other player's cards, victory card piles, and kingdom card piles
	were unaffected.
	
	Additionally, the discard pile was empty which means that after being played,
	smithy was trashed rather than discarded.

playAdventurer():
	cardtest2 detected that non-treasure cards revealed are not properly discarded. They could not be
	accounted for in the player's own piles, the other player's piles, nor the estate pile (only the
	initial coppers and estates were used). Instead of being discarded, it seems that they have been
	removed from play.

playCutpurse():
	cardtest3 detected that the last (highest numbered) player is always skipped and thus, is unaffected 
	by the card's effect of having to discard a copper despite having coppers in their hand. 

	Additionally, cardtest3 detected that even though a player may have no coppers in their hand, they
	are not forced to reveal their hand per the card's effect.

playRemodel():
	cardtest4 detected that remodel was often not correctly trashing and gaining cards. Sometimes, it would
	trash a card to gain a card that was worth over 2 coins more than the card being trashed. Sometimes, it
	would fail to trash a card to gain a card that was worth 2 coins or fewer more than the card being trashed.

	By setting up control cards in the player's hand, cardtest4 discovered that choice1 and choice2 were not
	being correctly processed. Instead of the respective values of choice1 and choice2 determining whether or not
	choice1 could be trashed to gain choice2, the selection of control cards in the deck was the determining factor.

	When the control cards are all set to copper, remodel always trashes to gain a card regardless of the card values.
	When the control cards are all set to estate, remodel always fails to trash to gain a card regardless of the
	card values.

	This is because of the conditional statement:
  	if ( (getCost(state->hand[currentPlayer][choice2]) + 2) > getCost(choice1) )
  	{
    	return -1;
  	}
  	
	Control cards = copper:
		Left side = 0 + 2. Right side = 2. 2 > 2 = False so we do not return -1 and remodel succeeds.

	Control cards = estate:
		Left side = 2 + 2. Right side = 2. 4 > 2 = True so we return -1 and remodel fails.

	Additionally, cardtest4 detected that even when remodel was correctly trashing and gaining cards, remodel
	itself was not being added to the discard pile after being played.
