Nancy Chan
CS 362
Winter 2016
Random Testing Quiz 2

In inputChar(), I created an array of characters that trigger the states 1 through 9 in testme().
I used rand() to generate random numbers between 0 and up to, but not including, the number of
characters in the array. These random numbers are used for array indices so that inputChar() randomly
returns a character from the array. I added extra characters that do not trigger any states so that
the program goes through more iterations before it finally reaches state 9 which is one of the conditions
for printing the error message.

In inputString(), I created an array of characters which is used to generate random strings. The strings
are of a random length between 0 and up to, but not including, the full length of the string that would
cause the error message to print ("reset"). I also added extra characters not found in "reset" to add more
randomness so that the program goes through more iterations before "reset" is formed by the random generation
of strings.

The error message prints once the state is set to 9 and the string returned is "reset".
