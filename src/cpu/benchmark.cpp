#include "benchmark.h"
#include "util.h"

#define READ_OVERHEAD_FILE "../read_overhead.txt"

double CPUBenchmark::getReadOverhead() {
  double sum = 0;
  uint64_t start, end;

  // warm up to make sure that the CPUID and RTDSC instruction are ready
  warmup();

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

  // warm up to make sure that the CPUID and RTDSC instruction are ready
  warmup();

  start = rdtsc();
  for(int i = 0; i < TIMES; i++) {
    // end loop to avoid new overhead
  }
  end = rdtsc();

  return (double)(end - start) / (double)TIMES;
}

void CPUBenchmark::measurementOverhead(fstream &file) {
  count << "1. Measurement overhead starts:" << endl;

  double overhead;
  count << "1.1 Get read overhead:" << endl;
  file.open(READ_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getReadOverhead();
      file << overhead << "\n";
      count << overhead << " ";
    }
    count << endl;
    file.close();
  }
  else {
    count << ""
  }
}
