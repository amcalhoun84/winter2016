/*
Jonathan Lagrew
bug1.c
CS 362 Winter 2016
Assignement 3

Part 3: Execute your unit tests and describe any bugs you find in a file named bug1.c. 

Unit Tests:

1. updateCoins() 
	This unit test passed all tests. First it tested with coppers and silvers and all values came out as expected. 
	Secondly, we tested coppers with golds and all tests came out as expected. 

2. fullDeckCount()

	This unit test passed all tests as well. Each card result matched the expected value. 
	
3. isGameOver()
	
	All tests passed, tested 9 instances with each expected value matching with if the game is over or not (0 or 1).
	
4. scoreFor()
	
	Test 1 for scoreFor() didn't pass, the score for player 0 was expected to be 29 and it actually came up with 22. 
	Same with test 2, the actual score was 0 and we expected 3. This was due to a bug in the score calculation of a 
	player's deck when the discard pile is 0 and a possible bug with the gardens card effect. In short, what I determined
	was that the gardens card uses fullDeckCount() to only get the player's total number of curse cards then dividing by 10. 
	
	
Card Tests:

1. playSmithy():
	
	cardtest1 tested playSmithy() and detected an error with the playedCardCount, each expected value was 1 higher than the actual value. 
	This is because the bug introduced in the refactored function set the flag to 1 in the discardCard(). 
	
2. playAdventurer():

	cardtest2 tested playAdventurer(): and detected an error with the post total is not 2 treasure cards less than pre total. Pre was 1 more than Post values.
	This is because the bug I wrote into the refactored function does not discard the last card in the temp array. 
	
3. playVillage():

	cardtest3 tested playVillage(): and detected no issues. No changes were made to playVillage(). 
	
4. playCouncilRoom():

	cardtest4 tested playCouncil_Room() and detected that the current player should have net gain of 3 cards in hand instead of always having 1 additional. 
	This is because I introduced this bug in the refactored function. 



 */