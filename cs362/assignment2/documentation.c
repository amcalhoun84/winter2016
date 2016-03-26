Andrew M. Calhoun
calhouna@oregonstate.edu
CS 362 Winter 2016
Assignment 2 - DOCUMENTATION.C

TO PLAY:

Log into Flip with your username and password.
Go to the directory where you installed dominion, for example: (onid-id)/dominion
Enter make all
Type in playdom (seed)
This will play a two player game based on a different seed and show you the steps
involved until the game resolves with a victory.

Dominion is a simple card game where the player tries to acquire as many victory points in the form of estates, duchies, and provinces as possible. They play a
series of cards with various effects called "Kingdom Cards" which are used in the action phase.

These cards can pull treasure cards (coins - in the form of copper, silver, and gold) from the player's deck of various value. These coins are used buy items
during the buying phase each hand. Or they can provide additional cards in general and actions, so the player may make more than one action per turn, or have
and additional buy during the buying phase. Or they can discard additional cards beyond what they normally can. Kingdom Cards can provide any number of interesting
effects!


This creates an array and allocates the memory necessary to store up to 10 cards in a player's hand which is the maximum number of hands allowed. Not every array
index (card) needs to be filled, but the creation of the array does allow the memory to be allocated for each card.

The game is then initialized with initializeGame():

This checks various variables, such as the number of players, instantiates the array in the initialization, picks the seed, and creates the game state
where most of the important data regarding the game is stored during game play. The code below is written for the C99 Standard, so variables must be initialized
before they are used in loops.

Game initialization dictates the conditions and number of certain cards available to the player pool. It also starts each player with 3 estates and 7 coppers,
with a total of 10 cards.

Within the game itself, there is a function called the cardEffect(), which contains switch statements for each of the cards. Each of these cards is associated with
an enumeration (enum) found in the dominion.h file. For example, if the adventurer card is called, it searches for the case and then follows the implementation of said card.
Each card has a different effect, and when drawn and played, it will either do something beneficial for the player, and potentially detrimental for their opponents.

NOTE: IN THE REFACTORED CODE, THERE ARE FIVE FUNCTIONS FOR CARDS WHOSE IMPLEMENTATION WERE REMOVED FROM THE cardeffect() FUNCTION -- SEE REFACTOR.C FOR ADDITIONAL DOCUMENTATION

When the current player calls the drawCard() function, they are given a card from their deck. Normally, this is called three times, and adds three
cards to their hand. The drawCard() function pulls cards from the player's deck for the number of times it is called. Within this function, it shuffles the deck after
placing cards in the player's hand and looking at the discarded deck. If Debugging is activated, this function will tell the player how many cards are in their deck
and in their hand.

PLAYING CARDS - USING smithy and adventurer as examples - SEE REFACTOR.C/DOMINION.C for more details.

Playing cards is a result of the cardeffect() function. As stated above, this contains a switch/case statement that contains the function calls and implementations of
all cards. In a standard, non-refactored version of the program, each of the cards will be in its own case implementation. The case calls on the enumerated card and
will perform action(s) defined within the implementaiton.

If an adventurer or smithy card is placed into the player's hand, then there are some special circumstances which come up. Based on their
enum and implementation, an adventurer card will locate the first two treasure (coin) cards in the player's hand. It will also ensure the deck is not empty. If their
deck is empty, it will shuffle the discards and refill the deck accordingly. Then another card is drawn into the players hand and if it is a treasure (coin) card, then
it will increment the drawntreasure variable. Once the program completes this, it will move the hand into a temporary array called temphand[]. When drawntreasure == 2,
the cards from temphand[] are moved into the discarded pile via the discardCard() function.

If the smithy card is drawn, it will add three cards to the player's deck via the drawCard() function. This is done by a for loop, and will repeat itself until all
three cards are drawn and placed into the player's deck, which is represented in an array of the player's hand. Once this is done, the card will be discarded and the
player will have additional cards (random) in their hand.

The updateCoins() resets the player's coin count to zero. It will then scan the player's hand and adds 1, 2, or 3 for each copper, silver, and/or gold respectively.
It will then place these coins into an array, and potentially add a bonus to the count. The coin count is then passed into the player's hand with the bonus for the
buy phase of the game.

The discardCard() function checks the trashFlag, which lets the game know if it should remove a card from the player's deck. If the flag is not active, then
the card can be put back into the player's playedCard array, and then that is incremented. The function will check to make sure the card was not the last card
in the hand. It will then swap positions and finally decrement the hand count.


DOCUMENTATION AUTHOR NOTES:

You may alter the program to increase the number of cards available to a player, how many are drawn, etc. This is not recommended for general use because it may
disrupt the balance of the game. However, if you plan to do what are often called "mods" within the gaming community, I highly recommend that you make backups
and notes of any changes you make so that you can tweak or restore your files. This is in case you do something that either breaks the gameplay, introduces an
unfixable bug or otherwise makes the code unable to compile properly.

Modding is a great way to improve your programming skills, but be sure to make notes about what you are doing! This prevents a lot of heart ache and lost
work down the line.
