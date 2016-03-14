#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../util.h"
const off_t BLOCKSIZE = 4096;

int main(int argc, const char * argv[])
{
    void* buf = malloc(BLOCKSIZE);
    int fd = open(argv[1], O_RDONLY | O_SYNC);
    //argv[2] in MB
	const off_t FILESIZE = atoll(argv[2])*1048576;
    
    while(read(fd, buf, BLOCKSIZE)!=0){
    }
    
    close(fd);

    //We close the file and read it again
    fd = open(argv[1], O_RDONLY| O_SYNC);
    off_t  totalBytes = 0;
    
    uint64_t st;
    uint64_t ed;
    uint64_t total_time = 0;

    while(1){
        st = rdtscStart();
        ssize_t bytes = read(fd, buf, BLOCKSIZE);
        ed = rdtscEnd();
        total_time += ed - st;
        if (bytes <= 0 || totalBytes >= FILESIZE)
            break;
        totalBytes += bytes;
    }
    
    close(fd);
    free(buf);
    double ans = (double)totalBytes/total_time;
    printf("%lf\n",ans);
    return 0;
}

