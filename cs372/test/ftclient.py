# Loyd Maverick Ty
# CS372 Project 2
# ftclient.py
# This is the client portion of the client-server data trasnfer application.
# This program uses the python socket API to connect to a server host
# in a port that is specified in the command line argument. It will also
# have commands -l, and -g in the command line arguments that will receive
# directory listing or files from the server in a specified port.

import sys
import socket
import os.path

# initSocket function, it takes in HOST and PORT as arguments. HOST
# is a string variable, and PORT is an int variable. This function is
# to initialize a socket and connects it to the host in the specified
# port.
def initSocket(HOST, PORT):
	# Creates an instance of the socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# Connects to the HOST in the specified PORT
	s.connect((HOST, PORT))
	# Return the socket
	return s

# directoryList function, it takes in d as arguments. d is the socket for
# the data connection. This function is to receive the directory listing
# from the server and print it out.
def directoryList(d):
	# Ok message to the server, so that it knows it can proceed
	d.send("OK");
	# Gets the directory listing from the server
	buffer = d.recv(1000);
	# While loop to get all the directory listing
	while buffer != "COMPLETE":
		# Ok message to the server, so that it knows it can proceed with the next file name
		d.send("OK")
		# Get next message
		buffer = d.recv(1000);
		# If the message is COMPLETE, then transfer is complete
		if (buffer == "COMPLETE"):
			# Message to indicate successful directory listing
			print "Directory listing complete."
		else:
			# Prints out the file in the directory
			print "%s" % buffer

# receiveFile function, it takes in d, HOST, PORT, DPORT, FILENAME as arguments. 
# d is the socket for the data connection. HOST, PORT, DPORT are the host, port number
# and data port number respectively. FILENAME is the file name that the client wants to
# receive. This function is to receive a specified file from the server. It will check
# if the file already exists before it tries to receive the file
def receiveFile(d, HOST, PORT, DPORT, FILENAME):
	# Get the okay message to proceed
	buffer = d.recv(1000);
	# Everything is good to go
	if (buffer == "OK"):
		# Check if file exists
		if (os.path.isfile(FILENAME)):
			# If it exsits, print out error message
			print "\"%s\" already exists." % FILENAME
			print "Transfer from %s:%d aborted." % (HOST, DPORT)
			# Send transfer cancel message to the server
			d.send("CANCEL")
		else:
			# If it does not exist, receive the file, print out message
			print "Receiving \"%s\" from %s:%d" % (FILENAME, HOST, DPORT)
			d.send("OK")
			# Open a file to write to
			f = open(FILENAME, "w")
			# Get text length
			textLength = d.recv(1000)
			d.send("OK")
			# Get first file chunk
			buffer = d.recv(1000)
			# Set initial value of i
			i = 0;
			# Loop until all file chunks are received
			while 1:
				# Write into the file
				f.write(buffer)
				# Increment i by the length of the chunk received
				i+=len(buffer)
				# If at the end of the file chunks, get out of the loop
				if (i >= int(textLength)):
					break
				# Get next chunk
				buffer = d.recv(1000)
			# Send message to the server to proceed
			d.send("OK")
			# Get complete message from server
			buffer = d.recv(1000)
			if (buffer == "COMPLETE"):
				# Print out complete message
				print "File Transfer complete."
			else:
				# Print out error
				print "%s" % buffer
	else:
		# Error from the server
		print "%s:%d says %s" % (HOST, PORT, buffer)

	
# main function, it takes in argv as an argument. This is the main function that calls the other
# functions and has the if statement for either -g or -l commands and calls the appropriate function
def main(argv):
	# Checks the length of the argv, to make sure the right amount of arguments is used
	if len(sys.argv) == 5:
		# Sets HOST as first argument
		HOST = sys.argv[1]
		# Sets PORT to the int specified by second command line argument
		PORT = int(sys.argv[2])
		# Sets COMMAND as the third argument
		COMMAND = sys.argv[3]
		# Sets DPORT as the int of the fourth argument
		DPORT = int(sys.argv[4])
		# Sets DPORTSTRING as the fourth argument
		DPORTSTRING = sys.argv[4]
	elif len(sys.argv) == 6:
		# Sets HOST as first argument
		HOST = sys.argv[1]
		# Sets PORT to the int specified by second command line argument
		PORT = int(sys.argv[2])
		# Sets COMMAND as the third argument
		COMMAND = sys.argv[3]
		# Sets FILENAME as the fourth argument
		FILENAME = sys.argv[4]
		# Sets DPORT as the int of the fifth argument
		DPORT = int(sys.argv[5])
		# Sets DPORTSTRING as the fifth argument
		DPORTSTRING = sys.argv[5]
	else:
		# Prints out the proper usage
		print "Usage: python chatclient.py <host> <command (-l or -g)> <filename (only in -g)> <port#>"
		exit(1)
	# Calls initSocket to initialize a socket connection
	# This is connection P
	s = initSocket(HOST, PORT)
	# Message to indicate connection established
	print "Now connected to %s" % HOST
	# Send the command to the server
	s.send(COMMAND)
	# Wait for okay message
	buffer = s.recv(1000);
	if buffer == "OK":
		# Send the port to the server for the data connection
		s.send(DPORTSTRING)
		# Wait for okay message
		buffer = s.recv(1000);
		if buffer == "OK":
			# This is connection Q for file transfer
			d = initSocket(HOST, DPORT)
			# Listing command
			if COMMAND == "-l":
				# Directory listing screen output
				print "Receiving directory structure from %s:%d" % (HOST, DPORT)
				# Call directoryList function
				directoryList(d)
			# File transfer command
			elif COMMAND == "-g":
				# Send filename to the server
				d.send(FILENAME)
				# Call receiveFile function
				receiveFile(d, HOST, PORT, DPORT, FILENAME)
	else:
		# Error message
		print "%s:%d says %s" % (HOST, PORT, buffer)
	# Close connections
	s.close()
	d.close()
	# Exit program
	exit(0)

# This is just to start/call the main function
if __name__ == "__main__":
    main(sys.argv[1:])