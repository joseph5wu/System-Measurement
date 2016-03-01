#!/bin/bash
mycount=32

for index in $( seq 1 11)
do
	dd if=/dev/urandom of=../../data/filesystem/testInput$index bs=1048576 count=$mycount
	#create a file with size == mycount blocks of blocksize 
    # argv[1] = testInput$index , argv[2] = mycount, argv[3] = fileCacheSize_result
    ./FileCacheSize ../../data/filesystem/testInput$index $mycount fileCacheSize_result
	let "mycount *= 2"
done