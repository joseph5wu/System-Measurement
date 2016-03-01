all: FileCacheSize
FileCacheSize: FileCacheSize.o
	gcc -o FileCacheSize FileCacheSize.o
FileCacheSize.o: FileCacheSize.c
	gcc -O0 -c FileCacheSize.c 
clean:
	rm -rf *o FileCacheSize
	rm -rf fileCacheSize_result