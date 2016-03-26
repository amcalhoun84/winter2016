// Andrew M. Calhoun
// calhouna@oregonstate.edu
// Date: 1/24/2016
// QUIZ 2 - Random Testing

Alterations made to testme.c

    Per the request of the assignment, we were to implement the functions inputChar() and inputString(). Both were specified as
    char functions, meaing they are to return a character or character string.

    My implementation of inputChar() specified a randomChar[] array with the char type. It was 13 indexes in length, and included
    the characters '(', '[', '{', '}', ']', ')', 'a', 'x', ' ', 'r', 'e', 's', 't'. Theoretically, I could have included an implementation
    that contained the entire ASCII list of alphanumeric inputs, but this resulted in test cases that could have taken hours, if not days, to
    resolve. This allowed me to thest the functionality of the test with a reasonable number of tests, ranging from approximately 20,000 to 1,000,000 potential tests before the
    testing resolved itself.

    The randomChar[] is then given its value and returned to the testme() function and put into the inputString() implementation.

    My inputString() implemented an allocated char pointer to serve as a string. I opted to use 6 characters, to allow for up to 5 characters
    and a '\0' null string/character. This will not catch any overflows, but the likelihood of this occuring is minimal, the event of a race condition could cause an overflow
    (Learned from CS344 - Block II, Lecture #2.) would require conditions that would NOT likely happen in this implementation.

    The inputString() would generate a string by taking the randomly generated input charaacter based on a for-loop, and then add a null-character as a constant addition at the end
    of each string. So, as long as the index was 4 or less, we would get a randomly generated character through the inputChar() function. This was called every time the loop iterated. '\0' always in index 5, and place these values into the stringBuf array variable
    and then would return it to the testMe() function where it would be displayed to the user.

    I.E; Iteration 476450: c = s, s = {()(e, state = 9


    The design is deliberately set up to give a comprehensive, but time-reasonable, number of test iterations.

    Assumptions and Self-Imposed Testing Constraints:
    I gave the above mentioned array constraint to ensure the test completed in a reasonable amount of time.
    Strings were limited to 6 bytes (5 characters + null-string character) to limit the length of time for test.
    Random number generators used to pick from limited array of characters.
    Assuming that the seed generation will always be randomized, based on srand(TIME(NULL)) declared in the main function.

    Ten Tests Completed with implementation:
        1. 130,069 Iterations
        2. 769,435 Iterations
        3. 431,677 Iterations
        4. 481,569 Iterations
        5. 384,394 Iterations
        6. 148,680 Iterations
        7. 1,023,750 Iterations
        8. 500,941 Iterations
        9. 19,383 Iterations
        10. 476,459 Iterations





