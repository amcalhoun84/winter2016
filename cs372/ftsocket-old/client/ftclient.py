''' 
** Name:		Andrew M. Calhoun
** Assignment:  FTP Socket - Project 2
** Description: This is the client for an FTP services socket. Connected
** clients can request or send a file. The FTP control is managed
** over the connection. After closing, a connection with one
** client, it'll listen for another connection until an interrupt
** (Ctrl-C) is sent.
**
** Sources:		http://beej.us/guide/bgnet/
** 				http://en.cppreference.com/w/cpp/io/c/fscanf
**				http://stackoverflow.com/questions/10837514/creating-a-c-ftp-server
**				http://man7.org/linux/man-pages/man3/getnameinfo.3.html
'''


# Partially derived from: 
# https://android.googlesource.com/toolchain/python/+/243b47fbef58ab866ee77567f2f52affd8ec8d0f/Python-2.7.3/Demo/sockets/ftp.py


import os
import re
import sys
from socket import *
import struct

BL = 10
TAG_LEN = 8


# Serves as the main function of the python program. 
def main():

	if len(sys.argv) not in (5, 6):
		print "usage: python ftclient <hostname> <port> -l | -g [<filename>] <dataport>"
		sys.exit(0)

	global hostname
	global port
	global command
	global fileName
	global dPort


	hostname = gethostbyname(sys.argv[1])
	port = sys.argv[2]
	command = sys.argv[3]
	fileName = sys.argv[4] if len(sys.argv) == 6 else None
	dPort = sys.argv[5] if len(sys.argv) == 6 else sys.argv[4]

	if not isInt(port):
		print "ftclient: Server Port must be an integer."
		sys.exit(1)
	port = int(port)

	if not isInt(dPort):
		print "ftclient: Data Port must be an integer."
		sys.exit(1)
	dPort = int(dPort)

	if command not in ("-l", "-g"):
		print "ftclient: command must be either -l or -g"
		sys.exit(1)

	if command == "-g" and fileName == "None":
		print "usage: python ftclient <hostname> <port> -l | -g [<filename>] <dataport>"
		sys.exit(1)
	
	if port < 1024 or port > 65535:
		print "Ports must be integer and in range 1024-65535"
		sys.exit(1)

	if dPort < 1024 or dPort > 65535:
		print "Ports must be integer and in range 1024-65535"
		sys.exit(1)

	# if port == dPort:
	# 	print "ftclient: server port and data port cannot match."
	# 	sys.exit(1)


	ftpClient()

	sys.exit(0)


def isInt(string):

	# http://pythoncentral.io/how-to-check-if-a-string-is-a-number-in-python-including-unicode/
	try:
		int(string)
		return True
	except:
		print "Must be integer."
		sys.exit(1)


def rcvAll(sock, numBytes):
	data == ""
	while len(data) < numBytes:
		try:
			data += socket.recv(numBytes - len(data))
		except Exception as e:
			print e.strerror
			sys.exit(1)

	return data

def rcvPkt(sock):
	pktLn = struct.unpack(">H", rcvAll(sock, 2))[0]
	tag = rcvAll(sock, TAG_LEN).rstrip("\0")
	data = rcvAll(sock, pktLn - TAG_LEN - 2)

	return tag, data



def runControlSession(ctrlSock):

	print "Tranmitting data port... FTP ACTIVE!"
	oTag = "DPORT"
	oData = str(dPort)

	print "Transmitting commands..."
	oTag = "NULL"
	oData = ""
	if command == "-l":
		oTag = "LIST"
	elif command == "-g":
		oTag = "GET"
		oData = fileName
	sndPkt(ctrlSock, oTag, oData)

	iTag, iData = rcvPkt(ctrlSock)

	if iTag == "ERROR":
		print "ftclient: " + idata + " causes an error. Please try again."
		return -1
	return 0

def dataControlSession(ctrlSock, dSock):
	ret = 0 # return values

	iTag, iData = rcvPkt(dSock)

	if iTag == "FNAME":
		print "ftclient: File listing on \"{0}\"".format(hostname, port)

		while intag != "DONE":
			print " " + iData
			iTag, iData = rcvPkt(dSock)

	elif iTag == "FILE":
		filename = iData
		if os.path.exists(fileName):
			print "ftclient: File \"{0}\" already exists.".format(filename)
			ret = -1;
		else:
			with open(fileName) as oFile:
				while iTag != "DONE":
					iTag, iData = rcvPkt(dSock)
					oFile.write(iData)
				print "ftclient: transfer complete"

	else:
		ret = -1

	sndPkt(ctrlSock, "ACK", "")

	return ret

def sndPkt(ctrlSock, tag, data):
	pktLn = 2 + TAG_LEN + len(data)

	pkt = struct.pack(">H", pktLn)
	pkt += tag.ljust(TAG_LEN, "\0")
	pkt += data

	try:
		sendall(pkt)
	except:
		print "Error sending to host."
		sys.exit(0)


def ftpClient():

	try:
		ctrlSock = socket(AF_INET, SOCK_STREAM, 0)
	except Exception as e:
		print e.strerror
		sys.exit(1)

	# Establish FTP control connection
	try: 
		ctrlSock.connect((hostname, port))
	except Exception as e:
		print e.strerror
		sys.exit(1)

	print ("ftclient: FTP control CXN established with: " + "\"{0}\"".format(hostname, port))

	status = runControlSession(ctrlSock)

	if status != -1:
		try:
			cliSock = socket(AF_INET, SOCK_STREAM, 0)
		except Exception as e:
			print e.strerror
			sys.exit(1)

		try: 
			cliSock.setsockopt(SOL_SOCKET, SO_REUASSADR, 1)
			cliSock.bind(("", dPort))
		except Exception as e:
			print e.strerror
			sys.exit(1)

		try:
			cliSock.listen(BL)
		except Exception as e:
			print e.strerror
			sys.exit(1)

		try:
			dataSocket = cliSock.accept()[0]
		except Exception as e:
			print e.strerror
			sys.exit(1)

		print("ftclient: FTP data CXN established with" + "\"{0}\"".format(hostname))

		runDataSession(ctrlSock, dSock)

		while True:
			iTag, iData = rcvPkt(ctrlSock)
			if iTag == "ERROR":
				print "ftclient: " + iData
			if iTag == "CLOSE":
				break

	try:
		ctrlSock.close()
	except Exception as e:
		print e.strerror
		sys.exit(1)

	print "ftclient: FTP CXN closed"


if __name__ == "__main__":
	main()