Andrew M. Calhoun
CS372 - Project I
Due Date: 2/7/2015

Instructions:

To Compile:
	
	gcc chatclient.c -o chatclient
	g++ chatserve.cpp -o chatserve

chatclient.c
		This program as a client program that connects to a server and requires the user to enter the hostname (i.e., flip2.engr.oregonstate.edu) 
		and a port (i.e. 1234). The user will then be able to send messages to the server and terminate the session if they type in "\quit"
	
	To Run the Program:

		Usage: ./chatclient <server> <port>


chatserve.cpp
		This program works as the server. It will require the user to enter a port number at the command line. Once this happens, it will wait for a 
		user or users to log in via the chatclient. If the program receives a \quit directive, it will close that connection and if it sends a \quit, 
		the queued connection will close. The chat server will wait for a connection until a SIGINT (ctrl+c)  command is sent.

	To Run the Program:

		Usage: ./chatserve <port>


Extras:
	The program has multithreading capabilities, so multiple clients can connect at once. It can get a little dodgy with over 2 users in testing, but can support
	up to five.

	The user can also change their handle/username with \\nick in the chatclient.

Testing:
	I performed testing on the flip servers, in two windows. 
	ssh calhouna@flip1.engr.oregonstate.edu for the server
	ssh calhouna@flip2.engr.oregonstate.edu for the chat client.


I was able to send and receive messages on both flip servers.