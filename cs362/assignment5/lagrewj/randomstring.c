/*Jonathan Lagrew
CS 362 Winter 2016
Quiz 2

Description: Implement a random tester for the function testme() in testme.c that is capable of printing the error message.  
You should implement inputChar() and inputString() to produce random values. Write up the development of your random tester 
and how it finds the error message as randomstring.c. Submit testme.c and randomstring.c under your onid directory in class github. 
under your onid directory and not under dominion directory).

1. char inputChar()
	This function will try all characters in the ascii range 32 to 126. I used rand() to generate random numbers between the min and max range
	for the desired ascii characters. I tested different combinations with the 2 functions and ended up sticking with this amount because the testing 
	time was reasonable. I started the ascii scale at 32 which is a space and ended at 126 which is ~. 

2. char *inputString()
	Originally started test with lowercase and uppercase letter, but scaled back testing to smaller array so that it would find an error quicker.
	My function creates a static array of size 6, which leaves a space for \0 at the end. The ascii range is set to be only lowercase letters. 
	The idea I had for this random test function was for currentChar to be a random lowercase letter 97-122. A for loop iterates through the rand()
	to place random lowercase letters in the first 5 array slots and at the end to add \0 in the last spot of the array. 
	
	Sample test:
	Iteration 25229602: c = ' ', s = reset, state 9 

	Testing took about 10 minutes on average with the current array sizes. 
*/
