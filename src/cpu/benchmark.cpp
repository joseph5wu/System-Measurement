#include "benchmark.h"
#include "../util.h"

#define DATA_DIR "../../data/cpu/"
#define READ_OVERHEAD_FILE DATA_DIR "read_overhead.txt"
#define LOOP_OVERHEAD_FILE DATA_DIR "loop_overhead.txt"
#define SYSTEM_CALL_OVERHEAD_FILE DATA_DIR "sys_call_overhead.txt"

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

double CPUBenchmark::getSystemCallOverhead() {
  double sum = 0;
  uint64_t start, end;

  for(int i = 0; i < TIMES; i++) {
    start = rdtsc();
    getppid();
    end = rdtsc();

    sum += (end - start);
  }

  return (double) sum / (double) TIMES;
}

double CPUBenchmark::getProcessOverhead() {
  double sum = 0;
  uint64_t start, end;
  pid_t pid;

  for(int i = 0; i < TASK_OP_TIMES; i++) {
    start = rdtsc();
    pid = fork();
    if(pid == 0) {
      // child process
    }
  }
}

void CPUBenchmark::prepare() {
  warmup();
  // cout << "warmup starts" << endl;
}

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

void CPUBenchmark::systemCallOverhead(fstream &file) {
  cout << "3. System call overhead starts:" << endl;

  double overhead;
  file.open(SYSTEM_CALL_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getSystemCallOverhead();
      file << overhead << "\n";
      cout << overhead << " ";
    }
    cout << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << SYSTEM_CALL_OVERHEAD_FILE << endl;
  }
}
