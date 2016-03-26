Andrew M. Calhoun
calhouna@oregonstate.edu
CS 362 - Assignment 3
coverage1.c

Tests require expansion, even though many of them have decent coverage. There is likely a variety of cases that I did not consider when I first set up the tests and began the testing.

----- unitTest1.c ------
updateCoin() function

Lines executed:74.58% of 118
unitTest1.c:creating 'unitTest1.c.gcov'

flip3 ~/cs362/assignment3 389% gcov -b unitTest1
File 'unitTest1.c'
Lines executed:74.58% of 118
Branches executed:100.00% of 36
Taken at least once:66.67% of 36
Calls executed:78.26% of 46
unitTest1.c:creating 'unitTest1.c.gcov'

Implications: While a good amount of the code is executed and the branches are all covered, there are likely areas that are being missed or need to be further investigated to make sure there are no underlying
bugs that I have missed based on the creation of my tests. Because there is perfect branch coverage, it may mean that I need to expand my branching for this test, as everything was done in a fairly straight
forward manner and I relied on assertions rather than if/else statements for testing. However, if there were any bugs detected or miscalculations, the program would abort, rather than complete.

----- unitTest2.c -----
discardCard() function

File 'unitTest2.c'
Lines executed:95.92% of 49
Branches executed:91.67% of 24
Taken at least once:54.17% of 24
Calls executed:68.00% of 25
unitTest2.c:creating 'unitTest2.c.gcov'

Implications: The suite is either complete and there is little issue with the discardCard() function. The test is very streamlined, and may actually not cover a set of parameters that need to be investigated. However, the
tests are based on assertions and if they were unsuccessful, the program would abort. Not all branches are taken either, as I was focusing on making sure discard counts were correct.

----- unitTest3.c -----
buyCard() function


File 'unitTest3.c'
Lines executed:97.44% of 39
Branches executed:100.00% of 18
Taken at least once:66.67% of 18
Calls executed:76.00% of 25
unitTest3.c:creating 'unitTest3.c.gcov'

Implications:  This is either a very complete or limited test, basaed on a If a player does not have the coins to purchase the card, the purchase is rejected, and if they do, it is allowed. The function is very
straight forward, so there really should not be much in the way of variance. Either the player can buy a card with their allocated coins/gold or they cannot. Again, this uses assertions, so if anything went awry,
then the program should abort. There are likely forced-failure cases which might be worth looking into.

----- unitTest4.c -----
endTurn() Function

File 'unitTest4.c'
Lines executed:95.45% of 44
Branches executed:94.44% of 36
Taken at least once:52.78% of 36
Calls executed:50.00% of 30
unitTest4.c:creating 'unitTest4.c.gcov'

Implications: The endTurn function is well balanced and works okay. However, only 50% of the calls were executed. There may need to be expansion of tests to ensure that every potential test case is covered and the
function is seen as "bug-free".

----- cardTest1.c ----
playAdventurer() function

Lines executed:88.06% of 67
Branches executed:84.62% of 26
Taken at least once:69.23% of 26
Calls executed:92.31% of 26
cardTest1.c:creating 'cardTest1.c.gcov'

Implications: We have decent, but not great coverage here. There is definite room for improvement and likely places where things need to be expanded or streamlined to ensure that we get better coverage. However, most of the calls were executed
and that shows there is at least a fair amount of things being found. The lines not being executed may also have to do with the fact of the refactored bugs and branches not being met under any condition with the current seed setting. Given there
is a heavy amount of branching in this particular file. There are likely other bugs that are not being founded in this coverage test.

----- cardTest2.c -----
playSmithy() function

File 'cardTest2.c'
Lines executed:84.43% of 122
Branches executed:100.00% of 64
Taken at least once:60.94% of 64
Calls executed:59.26% of 54
cardTest2.c:creating 'cardTest2.c.gcov'

Implications: Most of the code and all of the branches are being executed, but slightly less than 60% of the calls are. This means there needs to be an expansion of the test cases to ensure that more calls are met
and that gives us the informaiton we need to properly debug the code.

----- cardTest3.c -----
playVillage() function

Lines executed:73.17% of 41
Branches executed:100.00% of 20
Taken at least once:55.00% of 20
Calls executed:50.00% of 24
cardTest3.c:creating 'cardTest3.c.gcov'

Implications: There are a number of lines that are not being taken, but all branches are covered at least once. Half the calls are likely not being executed as they are on branches that are not being covered as the
requirements are not being met. This is likely due to the extensive branching I wrote into my test cases. However, this also means I may need to narrow down my test cases in the future to be more effective, as only
half of the 24 calls are being run. This means we are missing half of the potential bugs and test results on a regular basis.

------ cardTest4.c ------
playMinion() function


Lines executed:79.79% of 94
Branches executed:100.00% of 26
Taken at least once:57.69% of 26
Calls executed:73.21% of 56
cardTest4.c:creating 'cardTest4.c.gcov'

Implications: Again, we have decent coverage, but there are areas where it can be improved. An expansion of test cases might help to ensure that more calls are executed and more lines are run over with the coverage. Again, the lines
not executed very well may be part of branches not followed because the conditions are not met and thus cannot be pursued.


Overall Implications: By reviewing the coverage files, I found many of my suspicions were correct. AS I focused on making sure that the functions were processed correctly and functioning to the standards of the
game rules, I generally did not account for wilder cases. As we refactored in certain bugs, many tests were guaranteed to fail over time. When calls are not made, it is generally because the conditions required to make them
are not met or there is a bug hiding somewhere that prevents the call from being made. Reviewing the code will help me find areas in which to improve my test suite, generally by having more expansive cases and making
subtle in program adjustments to make sure that as much of the files is covered as possible.

