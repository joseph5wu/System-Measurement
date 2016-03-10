#include "../util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
using namespace std;

#define DATA_DIR "../../data/fileSystem/"
#define TEST_FILE_PREFIX DATA_DIR "test_"
#define LOCAL_SEQUENTIAL_READ_DATA DATA_DIR "local_sequential_read.csv"

const int TEST_FILE_NUMBER = 8;
const int TEST_TIMES = 100;
const int BLOCK_SIZE = 4096;

void localRead(fstream &file, bool isSequential);
void sequentialRead(int fileIndex, int fd, void* &buffer, double* totalPerFile, fstream &file, double overhead);
void randomRead(int fileIndex, int fd, void* &buffer, double* totalPerFile, fstream &file, double overhead);

int main() {

    // open data file for outputs
    fstream file;
    file.open(LOCAL_SEQUENTIAL_READ_DATA, ios::out);
    if(!file.is_open()) {
      cout << "Can't open file-" << LOCAL_SEQUENTIAL_READ_DATA << endl;
    }

    cout << "Local Sequentially Read Start:" << endl;
    file << "Local Sequentially Read" << "\n";
    localRead(file, true);
    cout << "Local Sequentially Read End" << endl;

    cout << "Local Randomly Read Start:" << endl;
    file << "\n\n\n\n\n" << "Local Randomly Read" << "\n";
    localRead(file, false);
    cout << "Local Randomly Read End" << endl;

    file.close();
    return 0;
}

void localRead(fstream &file, bool isSequential) {
    // pre-create different size of files [4k, ..., 64MB]
    int fd, readBytes;
    uint64_t start, end;
    double total, average;
    int readTimes;
    const char* fileName;

    warmup();
    double overhead = getReadOverhead();

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
            if(isSequential) {
                sequentialRead(j, fd, buffer, totalPerFile, file, overhead);
            }
            else {
                randomRead(j, fd, buffer, totalPerFile, file, overhead);
            }

            close(fd);
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
}

void sequentialRead(int fileIndex, int fd, void* &buffer, double* totalPerFile, fstream &file, double overhead) {
    // read file block by block
    int readBytes;
    double average;
    uint64_t start, end;
    int readTimes = 0;
    double total = 0;
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

    average = total / readTimes;
    totalPerFile[fileIndex - 1] += average;

    file << average;
    if(fileIndex != TEST_FILE_NUMBER) {
        file << ", ";
    }
    else {
        file << "\n";
    }
}

void randomRead(int fileIndex, int fd, void* &buffer, double* totalPerFile, fstream &file, double overhead) {
    // read file in random block
    double average;
    uint64_t start, end;
    double total = 0;

    off_t blockNumber = (off_t) pow(2.0, fileIndex - 1);
    off_t offset;

    for(int i = 0; i < blockNumber; i++) {
        offset = rand() % blockNumber;
        start = rdtscStart();
        // using lseek to set offset
        lseek(fd, offset, SEEK_SET);
        read(fd, buffer, BLOCK_SIZE);
        end = rdtscEnd();
        total += (double) end - (double) start - overhead;
    }

    average = total / blockNumber;
    totalPerFile[fileIndex - 1] += average;

    file << average;
    if(fileIndex != TEST_FILE_NUMBER) {
        file << ", ";
    }
    else {
        file << "\n";
    }
}