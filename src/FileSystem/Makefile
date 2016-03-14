all: FileCacheSize file_read contention
FileCacheSize: FileCacheSize.o
	gcc -o FileCacheSize FileCacheSize.o
FileCacheSize.o: FileCacheSize.c
	gcc -O0 -c FileCacheSize.c
file_read:
	g++ file_read.cpp -o file_read
contention:
	g++ contention.cpp -o contention
clean:
	rm -rf *o FileCacheSize
	rm -rf fileCacheSize_result
	rm -rf *o file_read
	rm -rf *o contention
