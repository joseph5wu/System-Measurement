#include "../util.h"
using namespace std;

#define DATA_DIR "../../data/fileSystem/"
#define TEST_FILE_PREFIX DATA_DIR "test_"
#define PAGE_FAULT_DATA DATA_DIR "page_fault_random.csv"

int main() {

}

void localSequentialRead() {

    
    // pre-create different size of files [4k, ..., 64MB]
    // each file need to read through for 1000 times to get average
    // using O_DIRECT to open files, each time read a block from such file
    // using posix_memalign to align memory allocation ?
    // close file and free memory

}


// Measure sequential read time
cycle_t SequentialRead(){
    cycle_t time_1,time_2, overhead = READ_OVERHEAD;
    char* file_name[5] = {"/home/yren/Download/3.mkv","/home/yren/Download/IP.zip","/home/yren/Download/6.rmvb","/home/yren/Download/12.zip","/home/yren/Download/5.txt"};
    int count=1,index;
    int fd;
    //open file
    if((fd= fileOpen(file_name[count],O_DIRECT))<0){
        perror("File open error\n");
        exit(1);
    }
    //measure file size
    int filesize = measureSize(fd);
    int blockcount=filesize/BLOCKSIZE;
    int blocksize=BLOCKSIZE;
    printf("Read Block number is: %d\n",blockcount);

    //Read file sequential
    char * filelocation = malloc(blocksize);
    posix_memalign((void **)&filelocation, blocksize, blocksize);
    time_1 = cyclecount();

    for(index=0;index<blockcount;index++){
    //I don't think we need this lseek here,since read would move the file position by the size it reads
        lseek(fd, blocksize*index, 0);//move to position blocksize After you have used lseek() to seek to a new location, the next I/O operation on the file begins at that location.
        read(fd,filelocation,blocksize);//read filesize bytes from discriptor fd to buffer filelocation
    }
    time_2 = cyclecount();

    //free location
    free(filelocation);
    //close file
    close(fd);
    return (time_2 - time_1)/CPUFREQ/1000;

}

// Measure random read time
cycle_t RandomRead(){
    cycle_t time_1,time_2, overhead = READ_OVERHEAD;
    char* file_name[5] = {"/home/yren/Download/3.mkv","/home/yren/Download/IP.zip","/home/yren/Download/6.rmvb","/home/yren/Download/12.zip","/home/yren/Download/5.txt"};
    int count=3,index;
    int fd;

    //open file
    if((fd= fileOpen(file_name[count],O_DIRECT))<0){
        perror("File open error\n");
        exit(1);
    }
    //measure file size
    int filesize = measureSize(fd);
    int blockcount=filesize/BLOCKSIZE;
    int blocksize=BLOCKSIZE;
    char * filelocation = malloc(blocksize);
    posix_memalign((void **)&filelocation, blocksize, blocksize);
    printf("Read Block number is: %d\n",blockcount/STRIDE);
    //Read file randomly in stride

    time_1 = cyclecount();
    //can we try read from the begining, then from the end, then beginning + stride, then end -stride?
    for(index=0;index<blockcount/STRIDE;index++){
        lseek(fd, blocksize*index*STRIDE, 0);//move to position blocksize After you have used lseek() to seek to a new location, the next I/O operation on the file begins at that location.
        read(fd,filelocation,blocksize);//read filesize bytes from discriptor fd to buffer filelocation
    }
    time_2 = cyclecount();
    //free location
    free(filelocation);
    //close file
    close(fd);
    return (time_2 - time_1)/CPUFREQ/1000;
}
