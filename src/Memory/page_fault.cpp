#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include "../util.h"
using namespace std;

#define DATA_DIR "../../data/memory/"
#define TEST_FILE DATA_DIR "test"
#define PAGE_FAULT_DATA DATA_DIR "page_fault_random.csv"

void get_memory_usage(unsigned int& free_pages) {
  ifstream vm_stat_stream("vm_stat | grep free | awk '{ print $3 }' | sed 's/\.//'", ios_base::in);

  vm_stat_stream >> free_pages;
  vm_stat_stream.close();
}

int main() {

  // open data file for outputs
  fstream file;
  file.open(PAGE_FAULT_DATA, ios::out);
  if(!file.is_open()) {
    cout << "Can't open file-" << PAGE_FAULT_DATA << endl;
  }

  int testFile = open(TEST_FILE, O_RDWR);
  if(testFile < 0) {
    cout << "Can't open the test file, please create a test file first" << endl;
    return -1;
  }

  unsigned int FILESIZE = 3435973836;
  unsigned int PAGESIZE = getpagesize();
  warmup();

  double overhead = getReadOverhead();

  for(int i = 1; i <= 16384; i *= 4) {
    // call purge to clear cache
    system("purge");

    file << i << " pages stride start \n";
    unsigned int STRIDE = i * PAGESIZE; // 4194304 bytes = 4MB
    unsigned int TEST_TIMES = min<unsigned int>(100, FILESIZE / STRIDE);

    // using mmap to map file into memory
    char* map = (char*) mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, testFile, 0);
    char testByte;
    uint64_t start;
    uint64_t end;
    double total = 0;
    double diff;
    // double start;
    // double end;
    // double total = 0;
    for(int j = 0; j < TEST_TIMES; j++) {
      uint64_t index = min((j * STRIDE + rand() % STRIDE), FILESIZE - 1);
      // uint64_t index =  (j * STRIDE);
      // get_memory_usage(before);
      start = rdtscStart();
      // start = monotonic_time();
      testByte = map[index];
      end = rdtscEnd();
      // end = monotonic_time();
      // get_memory_usage(after);
      diff = (double)end - (double)start - overhead;
      total += diff;
      file << diff << "\n";
    }

    double average = total / TEST_TIMES;
    cout << "Average time for stride size = " << STRIDE / PAGESIZE << " is "<< average << endl;
    // cout << "total_page = " << total_page << " and test time = " << TEST_TIMES << endl;
    munmap(map, FILESIZE);

    file << i << " pages stride end \n";
  }

  close(testFile);
  file.close();
  return 0;
}
