/* Andrew M. Calhoun
** Assignment 5 - CS362
** bugsinteammates.c

Observations - Nancy Chan:

    4 Unit Tests and 2 Card Tests should be ample opportunity to find and squash any bugs that may exist.

Bug Observation:

    unitTest 1:
    UpdateCoins():
    No tests failed. No bugs present.

    unitTest 2:
    discardCard():
    No tests failed. All assertions worked. No bugs present.

    unitTest 3:
    buyCard():
    Three trials, no failures.

    unitTest 4:
    EnderTurn():
    All assertions passed.

    cardTest1:
    Nancy's implementation did not contain any bugs that my tests caught.
    cardTest2:
    The test always ends with a segmentation fault.

    From valgrind -v cardTest2

    ==26172== Invalid read of size 4
    ==26172==    at 0x403E57: drawCard (dominion.c:528)
    ==26172==    by 0x406DC5: playSmithy (dominion.c:1295)
    ==26172==    by 0x401642: main (cardTest2.c:170)
    ==26172==  Address 0xfffffffffbfd1248 is not stack'd, malloc'd or (recently) free'd
    ==26172==
    ==26172==
    ==26172== Process terminating with default action of signal 11 (SIGSEGV)
    ==26172==  Access not within mapped region at address 0xFFFFFFFFFBFD1248
    ==26172==    at 0x403E57: drawCard (dominion.c:528)
    ==26172==    by 0x406DC5: playSmithy (dominion.c:1295)
    ==26172==    by 0x401642: main (cardTest2.c:170)


    ==26172== Invalid read of size 4
    ==26172==    at 0x403E57: drawCard (dominion.c:528)
    ==26172==    by 0x406DC5: playSmithy (dominion.c:1295)
    ==26172==    by 0x401642: main (cardTest2.c:170)
    ==26172==  Address 0xfffffffffbfd1248 is not stack'd, malloc'd or (recently) free'd

    From gdb run:

        Program received signal SIGSEGV, Segmentation fault.
        0x0000000000403e57 in drawCard (player=-60272, state=0x0) at dominion.c:528
        528       if (state->deckCount[player] <= 0){//Deck is empty
        Missing separate debuginfos, use: debuginfo-install glibc-2.12-1.166.el6_7.7.x86_64

        Tracepoint 1 was set at dominion.c, line 528
        The error appears to occur in the drawCard function.

    Removed a false positive. Nancy's code works fine with my tests once the issue of the segment fault was resolved.



    testCard3:

        This was the village card, which was actually broken in my card unit tests. However, after evaluating Nancy's code, I found that my tests were actually erroneous.
        As discardCard did not work as I anticipated or believed, I incorrectly created a state->discardCount comparison, rather than counting cards. Also, there was no way for me to test
        the proper increase of cards within the village card, without modifying the village function itself. Commenting the discardCard out of the village function caused the originally intended
        bug to return -- so we had something two fold happen here -- I discovered a bug in my methods and figured out how to fix it.

        The discard check made the "draw" check redundant, so I commented that out of my own tests. There may have been a level of subconscious tailoring to my own bug within my test.

        No bugs were found in my refactored implementation in Nancy's version of the dominion code.

    testCard4:

        Updated tests and reviewed Minion Code. Found no bugs in Nancy's implementation. Found one arithmetic error in my code, and updated it for correctness.

    randomTestAdventurer:

        Ran a 1000 test case implementation. No errors found.

    randomTestCard:

        The tests found no issues in Nancy's implementation.

Observations - Jon Lagrew

    Had to refactor code to work with Jon's, but not as extensively.


    unitTest1:
    updateCoins():
    No bugs found. Code works fine.

    unitTest2:

    discardCard():
    No bugs found. Code works fine.

    unitTest3:

    buyCard() function:
    No bugs found. Code works fine.

    unitTest4:

    endTurn():
    No bugs found. Works fine.

    cardTest1:
    playAdventurer():

    Jon's does not properly draw a card if the proper conditions are met. (drawnTreasure > 2)

    cardTest2:
    playSmithy():
    I had to refactor my card to work with Jon's version. Compared to my code, Jon's did not return the expected number of cards. The expected value is always one higher than the result.


    cardTest3:
    playVillage():
    Refactored code to match Jon's. No bugs found.

    cardTest4:
    playMinion():
    No issues found in Jon's version of Minion.

    randomCardTest:
    playVillage():
    Finds no issues with Jon's. Confirmed from CardTest 3.

    RandomTestAdventurer:
    playAdventurer()
    This identified the error, as causing too many cards to be drawn for the handCount and thus causing a failure.
    Further refactored my code to identify the exact issue -- results are always one greater than they should be. This
    is caused by the cards not discarding properly and being removed from the hand, from the tests being run.
