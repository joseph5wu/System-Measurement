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

#define DATA_DIR "../../data/fileSystem/contention/"
#define TEST_FILE_PREFIX DATA_DIR "test_"
#define SEQ_READ_DATA DATA_DIR "seq_read.csv"
// #define SEQ_READ_DATA DATA_DIR "seq_read.csv"
#define RAN_READ_DATA DATA_DIR "ran_read.csv"

// const int TEST_FILE_NUMBER = 8;
// const int TEST_TIMES = 100;
// const int BLOCK_SIZE = 4096;
// const int BLOCK_NUMBER = pow(4, 7);

const int TEST_FILE_NUMBER = 16;
const int TEST_TIMES = 50;
const int BLOCK_SIZE = 4096;
const int BLOCK_NUMBER = pow(4, 4);


void readFile(fstream &file, bool isSequential);
void pureRead(int fileIndex, bool isSequential);

int main() {

    // open data file for outputs
    fstream file;
    file.open(SEQ_READ_DATA, ios::out);
    if(!file.is_open()) {
      cout << "Can't open file: " << SEQ_READ_DATA << endl;
    }
    cout << "Contention Only Sequentially Read Start:" << endl;
    readFile(file, true);
    cout << "Contention Only Sequentially Read End" << endl;
    file.close();

    file.open(RAN_READ_DATA, ios::out);
    if(!file.is_open()) {
      cout << "Can't open file: " << RAN_READ_DATA << endl;
    }
    cout << "Contention Only Randomly Read Start:" << endl;
    readFile(file, false);
    cout << "Contention Only Randomly Read End" << endl;
    file.close();

    return 0;
}

void readFile(fstream &file, bool isSequential) {
    int fd;
    uint64_t start, end;
    double average, total = 0, totalAverage = 0;
    const char* fileName;

    warmup();
    double overhead = getReadOverhead();

    void* buffer = malloc(BLOCK_SIZE);
    pid_t pids[TEST_FILE_NUMBER];
    // using TEST_FILE_NUMBER as thread number
    for(int i = 1; i < TEST_FILE_NUMBER; i++) {
        file << "Company process number=" << i << "\n";
        for(int j = 0; j < i; j++) {
            // fork new process
            pids[j] = fork();
            if(pids[j] < 0) {
                cout << "Can't fork a new process" << endl;
                abort();
            }
            else if(pids[j] == 0) {
                // enter the child process, do pure read process without any fetching data
                pureRead(1 + j, isSequential);
                exit(0);
            }
        }

        totalAverage = 0;
        // parent process finishing fetching performance data
        for (int j = 0; j < TEST_TIMES; j++) {
            fileName = (TEST_FILE_PREFIX + to_string(1)).c_str();
            fd = open(fileName, O_SYNC | O_RDONLY);
            if(fd < 0) {
                cout << "Can't open the test file, please create a test file first: "<< fileName << endl;
                return;
            }

            // http://stackoverflow.com/questions/2299402/how-does-one-do-raw-io-on-mac-os-x-ie-equivalent-to-linuxs-o-direct-flag
            if(fcntl(fd, F_NOCACHE, 1) < 0) {
                cout << "Can't close cache of the test file" << endl;
                return;
            }
            // call purge to clear cache
            // system("purge");
            total = 0;
            if(isSequential) {
                for(int k = 0; k < BLOCK_NUMBER; k++) {
                    start = rdtscStart();
                    read(fd, buffer, BLOCK_SIZE);
                    end = rdtscEnd();
                    total += (double) end - (double) start - overhead;
                }
            }
            else {
                off_t offset;
                for(int k = 0; k < BLOCK_NUMBER; k++) {
                    start = rdtscStart();
                    offset = rand() % BLOCK_NUMBER;
                    // using lseek to set offset
                    lseek(fd, offset, SEEK_SET);
                    read(fd, buffer, BLOCK_SIZE);
                    end = rdtscEnd();
                    total += (double) end - (double) start - overhead;
                }
            }

            average = (total / BLOCK_NUMBER) * 0.37 / 1000000;
            file << average << "\n";
            file.flush();
            totalAverage += average;
            close(fd);
        }

        cout << "Company thread count = " << i << " average cycles = " << totalAverage / TEST_TIMES << endl;
        wait(NULL);
    }

    // close file and free memory
    free(buffer);
}

void pureRead(int fileIndex, bool isSequential) {
    // to make sure this pureRead keep running while we do the data-fetching process(parent process)
    const char* fileName;
    int fd;
    void* buffer = malloc(BLOCK_SIZE);
    for (int i = 0; i < TEST_TIMES * 2; i++) {
        fileName = (TEST_FILE_PREFIX + to_string(fileIndex + 1)).c_str();
        fd = open(fileName, O_SYNC | O_RDONLY);
        if(fd < 0) {
            cout << "Can't open the test file, please create a test file first: "<< fileName << endl;
            return;
        }

        // http://stackoverflow.com/questions/2299402/how-does-one-do-raw-io-on-mac-os-x-ie-equivalent-to-linuxs-o-direct-flag
        if(fcntl(fd, F_NOCACHE, 1) < 0) {
            cout << "Can't close cache of the test file" << endl;
            return;
        }

        // used to mimic fixed read
        // if(fileIndex % 2 != 0) {
        if(isSequential) {
            for(int i = 0; i < BLOCK_NUMBER; i++) {
                read(fd, buffer, BLOCK_SIZE);
            }
        }
        else {
            off_t offset;
            for(int i = 0; i < BLOCK_NUMBER; i++) {
                offset = rand() % BLOCK_NUMBER;
                // using lseek to set offset
                lseek(fd, offset, SEEK_SET);
                read(fd, buffer, BLOCK_SIZE);
            }
        }

        close(fd);
    }
    free(buffer);
}
