#!/bin/bash
mycount=3075

for index in $( seq 1 5)
do
	dd if=/dev/urandom of=../../data/filesystem/testInput_fine$index bs=1048576 count=$mycount
	#create a file with size == mycount blocks of blocksize 
    # argv[1] = testInput$index , argv[2] = mycount, argv[3] = fileCacheSize_result
    ./FileCacheSize ../../data/filesystem/testInput_fine$index $mycount fileCacheSize_result
    sudo purge
	let "mycount += 500"
done