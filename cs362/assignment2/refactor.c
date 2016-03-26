Andrew M. Calhoun
calhouna@oregonstate.edu
CS 362 Winter 2016 
Assignment 2 - REFACTOR.C

Pick five cards implemented in dominion.c. Choose 3 cards of your choice and smithy and adventurer cards are 
mandatory. Refactor the code so that these cards are implemented in their own functions, rather than as part 
of the switch statement in cardEffect. You should call the functions for these cards in the appropriate place 
in cardEffect. Check in your changes, with appropriate git commit messages. Document your changes in a text file 
in your dominion source directory, called “refactor.c.” Your implementation of at least 4 of these 5 cards should 
be incorrect in some way, i.e., you should introduce subtle bugs that are hard to catch in your changes. Introducing 
bugs in smithy and adventurer is mandatory.  Write information of your bugs also in refactor.c.   Later in this class, 
other students will test your code, so try to keep your bugs not superficial. Refactored program should compile 
without any error.

Cards Refactored:

Smithy - REQUIRED CARD - changed the for loop to be ++i, instead of i++. This will result in prefix incrementation and one less card than
what should be given.

Adventurer - REQUIRED CARD - Changed the shuffle draw from a < to <=. This will result in shuffling even if the player has 1 card, which won't be immediately caught since the likelihood of that is pretty low. 
A potential bug that was introduced but then removed was the drawnTreasure was changed from ++ to --. 
This resulted in the game freezing. Changed the decrement to an increment, giving the player an extra card. Changed increment to +2 for drawnTreasure. 
Changes seeded outcomes.

Minion - rather than giving you an additional action, the minion takes your action, and when given the choice, steals your coins, 
or adds three cards to your hand and gives the other players more cards without having them discard. This is a serious problem!;

Village - Card does not get discarded from the hand when the card is played. This can result in a game breaking bug where the player can keep drawing village cards
to get unlimited moves. Or it could just remain in the deck. I will have to test to know this for certain.

Council Room - switched the increment from i++ to ++i, so player only receives 3 cards. Similar problem to the Smithy.

REFACTORED FUNCTION PROTOTYPES:

int playAdventurer(struct gameState *state, int currentPlayer, int cardDrawn, int drawntreasure, int tempHand[], int z); // The adventurer is actually a pretty complicated card with a lot going on...
int playCouncRoom(struct gameState *state, int currentPlayer, int handPos);
int playMinion(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos); // Minion has two choices. These would be booleans in C++, but we are using 1 or 0 int here. Otherwise, it's not too different than other cards.
int playSmithy(struct gameState *state, int currentPlayer, int handPos);
int playVillage(struct gameState *state, int currentPlayer, int handPos);

REFACTORED FUNCTIONS, BUG NOTES IN COMMENTS.

int playAdventurer(struct gameState *state, int currentPlayer, int cardDrawn, int drawntreasure, int temphand[], int z)
{

      while(drawntreasure<2){
	if (state->deckCount[currentPlayer] <=1){//if the deck is empty we need to shuffle discard and add to deck *** REFACTOR to <= will cause shuffling even if the player has 1 card.
	  shuffle(currentPlayer, state);
	}
	drawCard(currentPlayer, state);
	cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
	if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
	  drawntreasure+=2; // The adventurer should not make off with your treasure. Originally had this put as --, but it actually caused the game to freeze. Opted to give the player more coins. Changes seeded outcomes.
	else{
	  temphand[z]=cardDrawn;
	  state->handCount[currentPlayer]++; //this should just remove the top card (the most recently drawn one).
	  z++;
	}
      }
      while(z-1>=0){
	state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
	z=z-1;
      }
      return 0;
}

//---------------------
// PLAY COUNCIL HALL
//---------------------
int playCouncRoom(struct gameState *state, int currentPlayer, int handPos)
{
    int i; // Needed to be declared under C99 standards, otherwise program breaks.

    //+4 Cards ** REFACTORED TO 3 because of introduced bug.
    for (i = 0; i < 4; ++i)
	{
	  drawCard(currentPlayer, state);
	}

      //+1 Buy
      state->numBuys++;

      //Each other player draws a card
      for (i = 0; i < state->numPlayers; i++)
	{
	  if ( i != currentPlayer )
	    {
	      drawCard(i, state);
	    }
	}

      //put played card in played card pile
    discardCard(handPos, currentPlayer, state, 0);

    return 0;
}

//-------------------------------
// PLAY MINION
//-------------------------------

int playMinion(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos)
{

    int i, j;


      //+1 action
      state->numActions--;

      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);

      if (choice1)		//+2 coins *** REFACTORED: -2 coins.
	{
	  state->coins = state->coins - 2;
	}

      else if (choice2)		//discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
	{

//	  //discard hand ** COMMENTED OUT FOR REFACTORING
//	  while(numHandCards(state) > 0)
//	    {
//	      discardCard(handPos, currentPlayer, state, 0);
//	    }

	  //draw 4 *** REFACTORED TO DRAW 3
	  for (i = 0; i < 3; i++)
	    {
	      drawCard(currentPlayer, state);
	    }

	  //other players discard hand and redraw if hand size > 4 *** REFACTOR: They not only get to keep their cards, they get 3 MORE!
	  for (i = 0; i < state->numPlayers; i++)
	    {
	      if (i != currentPlayer)
		{
		  if ( state->handCount[i] > 4 )
		    {
		    //  //discard hand
		    //  while( state->handCount[i] > 0 )
			// {
			//  discardCard(handPos, i, state, 0);
			// }

		      //draw 4 ** REFACTOR: 3
		      for (j = 0; j < 3; j++)
			{
			  drawCard(i, state);
			}
		    }
		}
	    }

	}
      return 0;

}

//-------------------------------
// PLAY SMITHY
//-------------------------------

int playSmithy(struct gameState *state, int currentPlayer, int handPos)
{

    int i;

//+3 Cards  *** REFACTOR changes i++ to ++i, so only two cards get drawn.
      for (i = 0; i < 3; ++i)
	{
	  drawCard(currentPlayer, state);
	}
      //discard card from hand
      discardCard(handPos, currentPlayer, state, 0);
      return 0;
}


//-------------------------------
// VILLAGE
//-------------------------------

int playVillage(struct gameState *state, int currentPlayer, int handPos)
{
      //+1 Card
      drawCard(currentPlayer, state);

      //+2 Actions
      state->numActions = state->numActions + 2;

      //discard played card from hand
      // discardCard(handPos, currentPlayer, state, 0);	// Discard commented out by 'mistake', resulting in the card not getting discarded.
      return 0;

}

//---------------------------------

Cards were removed from the switch statement and reimplemented like the example below:

case adventurer:
	playAdventurer(state, currentplayer...)
	break; // this prevents a fall through.

Rather than,
	case adventurer:
	//implementation of adventurer card
	return 0;

By using the functions, we are able to substitute the functionality and maintain the initial integrity of the program. Everything was proofed repeatedly for
errors.
