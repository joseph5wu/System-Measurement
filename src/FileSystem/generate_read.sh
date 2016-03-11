#!/bin/bash
count=4096
contentionCount=67108864

for index in $(seq 1 8)
do
	dd if=/dev/urandom of=../../data/fileSystem/test_$index bs=$count count=1
	chmod 644 ../../data/fileSystem/test_$index
	let "count *= 4"

	dd if=/dev/urandom of=../../data/fileSystem/contention/test_$index bs=$contentionCount count=1
	chmod 644 ../../data/fileSystem/contention/test_$index
done
