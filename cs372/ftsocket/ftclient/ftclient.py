#!/usr/bin/python

import sys
import socket
import getopt
import os
import signal
from struct import *

def initiateSocket(host, port):
	clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	if (port < 1024 or port > 65535):
		print "Server port must be in range [1024..65535]"
		sys.exit(1)


	clientSocket.connect((host, port))
	#try:
		
	# Track errors. 
	#except Exception as err:
	#	import traceback
	#	print(traceback.format_exc())
	#except: 
	#	print "Error connecting to " + host + " : " + str(port)
	#	sys.exit(0)

	return clientSocket

def directoryList(directory):
	directory.send("OK")
	buffer = directory.recv(1024)
	while buffer != "COMPLETE":
		directory.send("OK")
		buffer = directory.recv(1024)
		if(buffer == "COMPLETE"):
			print "Completed listing."
		else:
			print "%s" % buffer

def receiveFile(directory, host, port, dataport, filename):
	buffer = directory.recv(1024)
	if(buffer == "OK"):
		if(os.path.isfile(filename)):
			print "\"%s\" already exists." % filename
			print "Transfer from %s:%d aborted." % (host, dport)
			directory.send("CANCEL")
		else:
			print "Receiving \"%s\" from %s:%d" % (filename, host, dport)
			directory.send("OK")
			f=open(filename, "w")
			textLength = directory.recv(1024)
			directory.send("OK")
			buffer = directory.recv(1024)
			i=0

			while 1:
				f.write(buffer)
				i+=len(buffer)
				if(i>=int(textLength)):
					break
				buffer = directory.recv(1024)
				directory.send("OK")
				buffer = directory.recv(1024)
				if(buffer=="COMPLETE"):
					print "File transfer completed."
				else:
					print "%s" % buffer

	else:
		print "%s:%d says %s" % (host, port, buffer)

def isInt(string):
	try:
		int(string)
		return True
	except:
		print "Must be integer or all integer literal."
		sys.exit(1)


def main():

	if len(sys.argv) == 5:
		host = sys.argv[1]
		serverPort = int(sys.argv[2])
		serverPortString = sys.argv[2]
		command = sys.argv[3]
		dataport = int(sys.argv[4])
		dataportString = sys.arg[4]

	elif len(sys.argv) == 6:
		host = sys.argv[1]
		host = sys.argv[1]
		serverPort = int(sys.argv[2])
		serverPortString = sys.argv[2]
		command = sys.argv[3]
		filename = sys.argv[4]
		dataport = int(sys.argv[5])
		dataportString = sys.arg[5]

	else:
		print "Usage: python ftclient.py <host> <serverPort> <-l | -g> <filename if -g> <dataPort>"
		exit(1)

	connection = initiateSocket(host, serverPort)

	connection.send(command)

	buffer = s.recv(1024)
	if buffer == "OK":
		connection.send(dataportString)


if __name__ == "__main__":
	main()