Nancy Chan
CS 362
Winter 2016
Assignment 2: documentation.c

Submit a file called documentation.c (.c file because github thinks you are using github for file storage if you use many .txt files) that contains documentation
of smithy, adventurer cards. Documentation should contain your understanding of smithy and adventurer cards (code). It should also contain documentation of your
understanding of discardCard() and updateCoins() method. Keep your documentation short, though there is no upper limit. Documentation.c file should contain at
least 100 words.

smithy:
	The smithy adds 3 cards to the player's hand by drawing from the deck (or the discard pile if the deck is empty).
	It costs 4 coins.

	When played, smithy calls drawCard() 3 times in a loop. discardCard() is then called to discard smithy from the hand.

adventurer:
	The adventurer draws cards until 2 treasure cards have been added to the player's hand. The non-treasure cards that were revealed are discarded.
	It costs 6 coins.

	When played, adventurer calls drawCard() (shuffle() is called if the deck is empty) while the number of treasure cards is less than 2.
	The counter for treasure cards is incremented if the card drawn is a copper, silver, or gold.
	If a non-treasure card was drawn, then that card (which is the most recently drawn card and top card of the hand) is removed.
	All cards in play that have been drawn are then discarded.

discardCard():
	The discardCard function takes four arguments: hand position, current player, game state, and trash flag.
	When called, it updates the player's hand and hand count. It also adds cards to the played pile and played pile count if the trash flag is not set.
	If the card is in the last hand position or if there is only one card in the hand, then it simply decrements the hand count.
	Otherwise, it replaces the discarded card with the last card in hand and sets the last card to -1 in addition to decrementing the hand count.

updateCoins():
	The updateCoins function takes three arguments: player, game state, and bonus.
	When called, it resets the coin count. It then calculates the coin count by looping through the player's hand to increment the coins according to the treasure values:
	Copper: 1 coin; Silver: 2 coins; Gold: 3 coins
	It then adds the bonus amount to the coin count.