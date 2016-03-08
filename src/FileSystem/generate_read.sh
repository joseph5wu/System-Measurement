#!/bin/bash
count=4096

for index in $( seq 1 8)
do
	dd if=/dev/urandom of=../../data/fileSystem/test_$index bs=$count count=1
	let "count *= 4"
done
