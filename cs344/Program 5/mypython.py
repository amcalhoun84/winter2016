#!/usr/bin/python
## Andrew M. Calhoun
## Project 5
## CS344-400
## Due Date: 3/9/2016

## Description:

import sys
import string
import random

random.seed()

print "Creating file1."
fo1 = open("file1", 'w')
print "file1 created."
print "Creating file2."
fo2 = open("file2", 'w')
print "file2 created."
print "Creating file3."
fo3 = open("file3", 'w')
print "file3 created."

print "Generating random characters for file write..."

for x in range(1, 10):
	fo1.write(random.choice(string.ascii_lowercase))
	fo2.write(random.choice(string.ascii_lowercase))
	fo3.write(random.choice(string.ascii_lowercase))

fo1.close()
fo2.close()
fo3.close()

print "Complete."

fo1 = open("file1", 'r')
fo2 = open("file2", 'r')
fo3 = open("file3", 'r')

print "file1 contains: " + fo1.read()
print "file2 contains: " + fo2.read()
print "file3 contains: " + fo3.read()

print "Generating two integers."

a = random.randint(1,42)
b = random.randint(1,42)
c = a * b

print "The first integer is: %d" % a
print "The second integer is: %d" % b
print "Your product is: %d" % c