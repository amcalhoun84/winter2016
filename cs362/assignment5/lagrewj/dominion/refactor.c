/*Jonathan Lagrew
CS 362 Winter 2016
Assignment 2

Pick five cards implemented in dominion.c. Choose 3 cards of your choice and smithy and adventurer cards are mandatory. 
Refactor the code so that these cards are implemented in their own functions, rather than as part of the switch statement in cardEffect. 
You should call the functions for these cards in the appropriate place in cardEffect. Check in your changes, with appropriate git commit messages. 
Document your changes in a text file in your dominion source directory, called “refactor.c.” Your implementation of at least 4 of these 5 cards 
should be incorrect in some way, i.e., you should introduce subtle bugs that are hard to catch in your changes. Introducing bugs in smithy and adventurer is mandatory. 
Write information of your bugs also in refactor.c  Later in this class, other students will test your code, so try to keep your bugs not superficial. 
Refactored program should compile without any error.

Documented Changes:

Smithy:
new function: playSmithy
Notes: Card not set to discard pile when discardCard() called. 

adventurer:
new function: playAdventurer
Notes: The last card in tempHand not put in discard pile. 

playFeast:
Notes: For loop stops one less than hand count. 

playCouncil_Room:
Notes: draws one extra card now because of minor change to the for loop. 
*/
