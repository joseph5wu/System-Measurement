#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <math.h>
/*diskutil info / | grep "Block Size"
   Device Block Size:        512 Bytes
   Allocation Block Size:    4096 Bytes*/
ssize_t BLOCKSIZE = 4096;
ssize_t MEGABYTE = 1048576;
int TIMEOUT = 5;
char *buffer;
long double numBlocks_file;
int countMB = 0;//total MB read in TIMEOUT
time_t start;
char *resultFileName = NULL;
void done_sequential()
{
	time_t end;
	FILE *file;

	time(&end);

	if (end < start + TIMEOUT) {
		printf(".");
		alarm(1);
		return;
	}

	file = fopen(resultFileName, "a");
	
	if (countMB!=0) {
		double MB_per_sec = countMB / (double) TIMEOUT;
		float access_time_per_block = TIMEOUT / ((double)(countMB * MEGABYTE) / BLOCKSIZE);
        printf("Results: countMB: %d, %f MB/second, %f ms access time per block\n",
                                                             countMB, MB_per_sec, access_time_per_block * 1000.0);
		
        //numBlocksInFile, filesize(MB), MB_per_sec, access_time_per_block
		fprintf(file, "%Le %Le %f %f\n", 
				numBlocks_file,
				numBlocks_file * BLOCKSIZE / MEGABYTE,
				MB_per_sec,
				access_time_per_block * 1000.0);
	}
	free(buffer);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	//dd if=/dev/urandom of=../../data/filesystem/16MB bs=1024 count=16384
	//run the above command to create a 16MB test file
	int fd;
	int bytesRead;
    unsigned long total_read;
	setvbuf(stdout, NULL, _IONBF, 0);//change buffering mode of stdout stream
	/*if (argc != 5) {
		printf("Usage: FileCacheSize <raw disk device> <fake_file_size_in_mb>\n");
		exit(EXIT_SUCCESS);
	}*/
	long double  BLOCKS_PER_MB;
	BLOCKS_PER_MB = (long double) MEGABYTE/(long double)BLOCKSIZE;
	//printf("BLOCKS_PER_MB: %Lf",BLOCKS_PER_MB);
	//fd = open("../../data/filesystem/16MB", O_RDONLY);
	fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("Open failed.");
		exit(EXIT_FAILURE);
	}

    buffer = (char *) malloc(MEGABYTE);

   // int accessed_FileSize = 32;//32MB
    int accessed_FileSize = atoi(argv[2]);
    resultFileName = argv[3];
   // for(int i = 1; i < 11; i++){
    	numBlocks_file = (long double)ceil(accessed_FileSize * BLOCKS_PER_MB);
    	//resultFileName = "fileCacheSize_result";
    	printf("File Cache Size: [%d MB]  [%Le blocks], timeout = %d seconds\n", 
    		 accessed_FileSize, numBlocks_file, TIMEOUT);

    	time(&start);
		signal(SIGALRM, &done_sequential);
		alarm(1);


/*The process of reading from beginning of
disk to the current le size repeats until a time-
out signal is asserted.
*/
		for (;;) {
	    //read one MB
		bytesRead = read(fd, buffer, MEGABYTE);
		//if(bytesRead>0) printf("Nonzero");
		if(bytesRead < 0){
			perror("Read failed.");
			exit(EXIT_FAILURE);
		}
		
		bytesRead = bytesRead/MEGABYTE;
        total_read += bytesRead;
       //if(bytesRead == 0) printf("zero\n");
        
        

        // printf("total_read: %lu, bytesRead: %d, accessed_FileSize: %d\n", total_read,bytesRead, accessed_FileSize);
        if (total_read >= accessed_FileSize) {
        	//printf("lseek\n");
        	//exit(0);
            bytesRead = lseek(fd, 0, SEEK_SET);
           // if(bytesRead <=0){
            //	perror("lseek64 failed.");
            //	exit(EXIT_FAILURE);
			//}
            total_read = 0;
            //printf("total_read = %lu\n", total_read);
            continue;
        } 
		countMB++;
		//printf("bytesRead: %d,countMB: %d\n", bytesRead, countMB);
		}
    	//accessed_FileSize = 2*accessed_FileSize;
	//}
}