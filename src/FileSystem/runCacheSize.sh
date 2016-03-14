#!/bin/bash
mycount=32
#32 64 not fine measure
for index in $( seq 1 11)
do
    # argv[1] = testInput$index , argv[2] = mycount, argv[3] = fileCacheSize_result
    ./FileCacheSize ../../data/filesystem/testInput$index $mycount fileCacheSize_result2
    sudo purge
	let "mycount *= 2"
done