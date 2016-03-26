/*
Jonathan Lagrew
CS 362 Winter 2016
Assignement 2

Documentation should contain your understanding of smithy and adventurer cards (code). 
It should also contain documentation of your understanding of discardCard() and updateCoins() method. 
 Keep your documentation short, though there is no upper limit. Documentation.c file should contain at least 100 words. 


1) Smithy
	-The cost of the card is 4 coins.
	-This card adds 3 cards to the player's hand who drew the card.
	-Drawing cards happens by taking them from the top of the deck.
	-Code checks if pile is empty and will take the cards from the discard pile.
	-Card envokes drawCard function in a loop, 3 times for the 3 additional cards.
	-Finally, discardCard() is called to take this card out of the player's hand.
	
2) Adventurer
	-The cost of the card is 6 coins. 
	-This card draws cards until 2 treasure cards (bronze, silver or gold) have been drawn.
	-Adds these combination of bronze, silver or gold cards to the player's hand.
	-Players can see the drawn cards face up.
	-These extra cards are discarded to the discard pile.
	-First the card calls drawCard() function.
	-If the pile is empty, then shuffle() is called.
	-This cycle goes on until 2 treasure cards are found and put into the player's hand.
	-A counter is used to increment the treasure cards if a treasure card is drawn.
	-The top card of the hand if not a treasure card is discarded. 
	
3) discardCard()
	-Parameters: hand position, current player, game state and trash flag.
	-Description: This function updates the player's hand count and hand. The discarded card goes to the discard pile and discard pile count in incremented. 
				  The hand count is decremented by 1 if the card is in the last position or if its the player's only card. If the card is in any other position,
				  the function replaces the discarded card with the last card in the hand and decrements the last card in the hand's count by 1 and does the same
				  for the remaining cards in the hand. Much like removing a value from an array and filling in the remaining spots accordingly. 
				  
4) updateCoins()
	-Parameters: player, game state, and bonus
	-Description: This function updates the coin count then calculates the coin count by iterating through the player's cards to increment the coins based on their
				  value of copper, silver or gold. Copper has a value of 1, silver has a value of 2, and gold a value of 3. Finally, it adds the bonus to the coin 
				  count. 

 */