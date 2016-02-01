#include "benchmark.h"
#include "../util.h"

#define DATA_DIR "../../data/cpu/"
#define READ_OVERHEAD_FILE DATA_DIR "read_overhead.txt"
#define LOOP_OVERHEAD_FILE DATA_DIR "loop_overhead.txt"

double CPUBenchmark::getReadOverhead() {
  double sum = 0;
  uint64_t start, end;

  for(int i = 0; i < TIMES; i++) {
    start = rdtsc();
    end = rdtsc();
    sum += (end - start);
  }

  return (double)sum / (double)TIMES;
}

double CPUBenchmark::getLoopOverhead() {
  double sum = 0;
  uint64_t start, end;
  warmup();
  start = rdtsc();
  for(int i = 0; i < TIMES; i++) {
    // end loop to avoid new overhead
  }
  end = rdtsc();

  return (double)(end - start) / (double)TIMES;
}


void CPUBenchmark::getProcedureOverhead(vector<double> &result){
    double totalTime = 0;

}
/*
void CPUBenchmark::warmup() {
  warmup();
}
*/
void CPUBenchmark::measurementOverhead(fstream &file) {
  cout << "1. Measurement overhead starts:" << endl;

  double overhead;
  cout << "1.1 Get read overhead:" << endl;
  file.open(READ_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getReadOverhead();
      file << overhead << "\n";
      cout << overhead << " ";
    }
    cout << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << READ_OVERHEAD_FILE << endl;
  }

  cout << "1.2 Get loop overhead:" << endl;
  file.open(LOOP_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getLoopOverhead();
      file << overhead << "\n";
      cout << overhead << " ";
    }
    cout << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << LOOP_OVERHEAD_FILE << endl;
  }
}
