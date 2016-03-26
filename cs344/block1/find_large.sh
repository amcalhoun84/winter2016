#!/bin/sh
#	find_large.sh
# This Shell script locates all files 
# larger than a kilobyte i nmy home directory
# and prints the top 5 largest in decreasing
# order.

find $HOME -size +1000 -ls | sort -nrk 7 | head -5

