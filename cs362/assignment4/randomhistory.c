Andrew M. Calhoun
CS 362 - ASSIGNMENT 4
Random Testing
Due Date: 2/14/2016

Aggregate Tests Totals for Tests:
Original 8 Tests:

Lines executed: 86.23%
Branches executed: 96.34%
Taken at least once: 60.39%
Calls executed: 68.38%

Original 10 Tests:
Lines executed: 86.38%
Branches executed: 97.07%
Taken at least once: 60.88%
Calls executed: 69.83%

Adventurer Tests:

---- Assignment 3 ----

Lines executed:88.06% of 67
Branches executed:84.62% of 26
Taken at least once:69.23% of 26
Calls executed:92.31% of 26
cardTest1.c:creating 'cardTest1.c.gcov'

---- Assignment 4 ----
Lines executed:79.35% of 92
Branches executed:100.00% of 70
Taken at least once:68.57% of 70
Calls executed:60.53% of 38
randomTestAdventurer.c:creating 'randomTestAdventurer.c.gcov'


Village Tests:

---- Assignment 3 ----

Lines executed:73.17% of 41
Branches executed:100.00% of 20
Taken at least once:55.00% of 20
Calls executed:50.00% of 24
cardTest3.c:creating 'cardTest3.c.gcov'

---- Assignment 4 ----
Lines executed:85.92% of 71
Branches executed:100.00% of 14
Taken at least once:57.14% of 14
Calls executed:90.70% of 43
randomTestCard.c:creating 'randomTestCard.c.gcov'


randomTestAdventurer:

		My previous tests included some random tests, so I decided to create a new
		series of tests. Albeit, they are very similar, the branch coverage for my second test
        was more comprehensive, even though fewer lines were executed. This is likely in part to more
        lines being in the program, and zeroing in on the bugs that I "discovered" during my assignment 3
        testing. I also had randomized treasure draws for the first time, whereas I let the cards in the player's deck
        and hand determine whether or not certain parameters were met. This probably created a more 'organic' testing situation,
        at the expense of line coverage. However, with full branch coverage, we probably have a slightly more comprehensive test.

        The biggest difference between the previous version of my test and the current version is the scale. Originally, there were only
        20 random tests done, whereas in the updated/new tests, there are 1000 tests being performed. This could account for some of the difference
        in call coverage as well, as call coverage was 92.31% in the original test and only 60.53% in the current test.

        This testing suite was also set up with a seed (29365) so that some tests would succeed and others fail. The tests only failed when the player drew
        two or fewer treasure cards, which would result in the deck being reshuffled excessively and other conditions not being met. Given that this happened
        approximately 80% of the time, it gives us a reasonable expectation that there was a bug in the drawn treasure check within the adventurer card itself.

        Other issues do seem to appear as well if there are multiple treasure cards drawn, which were then discarded. This should not happen either, and it is
        probably a side effect of the implemented bugs from my original versions of the files.

             7000:   98:			if(temphand[j] == 4 || temphand[j] == 5 || temphand[j] == 6)
                    branch  0 taken 96% (fallthrough)
                    branch  1 taken 4%
                    branch  2 taken 96% (fallthrough)
                    branch  3 taken 4%
                    branch  4 taken 4% (fallthrough)
                    branch  5 taken 96%

                -- early on is a branch where the treasure cards are randomly drawn. It is fairly rare that treasure is drawn, given we have a full
                suite of cards to choose from. This testing suite also shows an inconsistency in how the Adventurer card plays out, given the bug that exists
                where cards are not properly discarded and shuffled too many times if the treasure draw is under two. The random chance here reveals the potential
                cascade of problems that can occur because this functionality is not implemented correctly and based on the previous tests, we have confirmation that it is
                something that has to do with the treasure draw.

Village Tests:

        My original card tests were reasonably thorough and random testing improved this further. The tests were much smaller scale; however, and there were only 14 branches versus 20 in the new
        update. I reviewed my original tests and trimmed the branches to focus primarily on the area where bugs might have been. The other player's play was also added into the suite, and I found
        that this error tends to grow over time, between both players, especially if endTurn is used, in particular if both cards are not discarded properly.

        The improvement in coverage means we are also more likely to find the bugs. Both versions of the test suit had 100% branch execution, which is good, but with an increase in proportional line execution
        we can also further isolate the bugs. And as 90.70% of all calls are now being executed with nearly twice the number, we are getting closer to an ideal test suite. There are still areas and scenarios
        that probably were not considered in this version of the test. However, the village bug is fairly simple and card discarding is easily fixed by uncommenting out the discard function.

        Currently, test failures to successes are 50/50. The discardCount for both players does not work properly, and it appears the second player does not pick up the card properly either. Whether this is a
        bug of the testing suite or an actual bug within the card itself will require further testing. A big positive of this suite, is that it shows a bug that is consistent and can be readily isolated and
        hammered out with further testing and debugging. This test suite also confirms the improper discard bug from the previous tests.
