Nancy Chan
CS 362
Winter 2016
Assignment 2: refactor.c

Document your changes in a text file in your dominion source directory, called “refactor.c.” 
Your implementation of at least 4 of these 5 cards should be incorrect in some way, i.e., you should introduce subtle bugs that are hard to catch in
your changes. Introducing bugs in smithy and adventurer is mandatory. Write information of your bugs also in refactor.c

Changes to dominion.c:
  adventurer, smithy, cutpurse, remodel, and sea_hag were refactored to be implemented in their own functions rather than as part of the switch statement
  in the cardEffect function. The new functions are named playAdventurer, playSmithy, playCutpurse, playRemodel, and playSea_Hag.

Bugs introduced:
  adventurer:
    - Deleted z++; from:
    else{
      temphand[z]=cardDrawn;
      state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
      z++;
    }
    This makes it so that when a non-treasure card is drawn, it keeps overwriting what is at index 0 in the temphand array
    instead of adding more cards to the array because the z counter is not incrementing correctly. It also makes it so that
    while(z-1>=0) never holds true since z stays at 0.

  smithy:
    - Changed the last paramter to 1 in:
    discardCard(handPos, currentPlayer, state, 0);
    This sets the trash flag so that the card is put in the trash pile rather than the discard pile, removing it from play.
    - changed < to <= in:
    for (i = 0; i < 3; i++)
    This causes 4 cards to be drawn instead of 3.

  cutpurse:
    - Changed state->numPlayers to state->numPlayers - 1 in:
    for (i = 0; i < state->numPlayers; i++)
    This makes it seem like there is one fewer player in the game, causing the last player's hand to not be affected by this card.
    - Changed k++ to k += 2 in:
    if (j == state->handCount[i])
    {
      for (k = 0; k < state->handCount[i]; k++)
    If a player has a hand with no coppers, then only every other card in that hand is revealed instead of all of them.

  remodel:
    - Swapped choice1 and choice2 in:
    if ( (getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(choice2) )
    Scenario: choice1 = 3 coins, choice2 = 6 coins
    Normally, this would evaluate to 3 + 2 > 6; false.
    But swapping causes it to be 6 + 2 > 3; true, allowing the player to buy a more expensive card than stated.
    Scenario: choice1 = 6 coins, choice2 = 3 coins
    Normally, this would evaluate to 6 + 2 > 3; true.
    But swapping causes it to be 3 + 2 > 6; false, preventing the player from buying a card they should be able to.

  sea_hag:
    - Changed the below to the opposite operations
    (decrementing to incrementing and incrementing to decrementing):
    state->deckCount[i]--;
    state->discardCount[i]++;
    This card forces other players to discard the top cards of their decks but the bug treats the card as if players were
    taking cards from the discard pile and putting them into their decks so the card pile counts will be incorrect.
