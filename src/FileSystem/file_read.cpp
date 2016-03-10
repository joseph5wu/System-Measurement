#include "../util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#define DATA_DIR "../../data/fileSystem/"
#define TEST_FILE_PREFIX DATA_DIR "test_"
#define LOCAL_SEQUENTIAL_READ_DATA DATA_DIR "local_sequential_read.csv"

const int TEST_FILE_NUMBER = 8;
const int TEST_TIMES = 100;
const int BLOCK_SIZE = 4096;

void localSequentialRead();

int main() {
    localSequentialRead();
    return 0;
}

void localSequentialRead() {
    // pre-create different size of files [4k, ..., 64MB]
    int fd, readBytes;
    uint64_t start, end;
    double total, average;
    int readTimes;
    const char* fileName;

    warmup();
    double overhead = getReadOverhead();

    // open data file for outputs
    fstream file;
    file.open(LOCAL_SEQUENTIAL_READ_DATA, ios::out);
    if(!file.is_open()) {
      cout << "Can't open file-" << LOCAL_SEQUENTIAL_READ_DATA << endl;
    }

    void* buffer = malloc(BLOCK_SIZE);
    double totalPerFile[TEST_FILE_NUMBER];
    for(int i = 0; i < TEST_FILE_NUMBER; i++) {
        totalPerFile[i] = 0;
    }

    // each file need to read through for TEST_TIMES to get average
    for (int i = 0; i < TEST_TIMES; i++) {
        for(int j = 1; j <= TEST_FILE_NUMBER; j++) {
            // using O_DIRECT to open files, each time read a block from such file
            // http://stackoverflow.com/questions/2299402/how-does-one-do-raw-io-on-mac-os-x-ie-equivalent-to-linuxs-o-direct-flag
            fileName = (TEST_FILE_PREFIX + to_string(j)).c_str();
            fd = open(fileName, O_SYNC | O_RDONLY);
            if(fd < 0) {
                cout << "Can't open the test file, please create a test file first: "<< fileName << endl;
                return;
            }
            if(fcntl(fd, F_NOCACHE, 1) < 0) {
                cout << "Can't close cache of the test file" << endl;
                return;
            }
            // call purge to clear cache
            // system("purge");

            // read file block by block
            total = 0;
            readTimes = 0;
            while(true) {
                start = rdtscStart();
                readBytes = read(fd, buffer, BLOCK_SIZE);
                end = rdtscEnd();
                if(readBytes > 0) {
                    total += (double) end - (double) start - overhead;
                    readTimes++;
                }
                else {
                    break;
                }
            }
            close(fd);

            average = total / readTimes;
            totalPerFile[j - 1] += average;

            file << average;
            if(j != TEST_FILE_NUMBER) {
                file << ", ";
            }
            else {
                file << "\n";
            }
        }

        if(i % 10 == 0) {
            cout << "Test time: " << i << endl;
        }
    }

    for(int i = 0; i < TEST_FILE_NUMBER; i++) {
        average = totalPerFile[i] / TEST_TIMES;
        cout << "Average time for file size = " << BLOCK_SIZE * pow(2.0, i) / 1024<< "KB is "<< average << endl;
    }

    // close file and free memory
    free(buffer);
    file.close();
}

//
// // Measure random read time
// cycle_t RandomRead(){
//     cycle_t time_1,time_2, overhead = READ_OVERHEAD;
//     char* file_name[5] = {"/home/yren/Download/3.mkv","/home/yren/Download/IP.zip","/home/yren/Download/6.rmvb","/home/yren/Download/12.zip","/home/yren/Download/5.txt"};
//     int count=3,index;
//     int fd;
//
//     //open file
//     if((fd= fileOpen(file_name[count],O_DIRECT))<0){
//         perror("File open error\n");
//         exit(1);
//     }
//     //measure file size
//     int filesize = measureSize(fd);
//     int blockcount=filesize/BLOCKSIZE;
//     int blocksize=BLOCKSIZE;
//     char * filelocation = malloc(blocksize);
//     posix_memalign((void **)&filelocation, blocksize, blocksize);
//     printf("Read Block number is: %d\n",blockcount/STRIDE);
//     //Read file randomly in stride
//
//     time_1 = cyclecount();
//     //can we try read from the begining, then from the end, then beginning + stride, then end -stride?
//     for(index=0;index<blockcount/STRIDE;index++){
//         lseek(fd, blocksize*index*STRIDE, 0);//move to position blocksize After you have used lseek() to seek to a new location, the next I/O operation on the file begins at that location.
//         read(fd,filelocation,blocksize);//read filesize bytes from discriptor fd to buffer filelocation
//     }
//     time_2 = cyclecount();
//     //free location
//     free(filelocation);
//     //close file
//     close(fd);
//     return (time_2 - time_1)/CPUFREQ/1000;
// }
