// CPU, Scheduling, and OS Services
// // // 5. Context switch time: Report the time to context switch from one process to another, and from one kernel thread to another. How do they compare? In the past students have found using blocking pipes to be useful for forcing context switches.

#ifndef CPU_BENCHMARK_H
#define CPU_BENCHMARK_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <pthread.h>
#include <sys/wait.h>
using namespace std;

class CPUBenchmark {
private:
  double getReadOverhead();
  uint64_t getLoopOverhead(int);
  double getLoopOverhead2();
  void getProcedureOverhead(vector<double> &);
  double getSystemCallOverhead(bool);
  double getProcessCreationTime();
  double getKernelThreadCreationTime();
  static void * foo(void *);

public:
  /**
    1. Measurement overhead:
    Report the overhead of reading time, and report the overhead of using a loop to measure many iterations of an operation.
  */
  void measurementOverhead(fstream &);

  /**
    2. Procedure call overhead:
    Report as a function of number of integer arguments from 0-7.
    What is the increment overhead of an argument?
  */
  void procedureCallOverhead(fstream &);
  /**
    3. System call overhead:
    Report the cost of a minimal system call. How does it compare to the cost of a procedure call?
  */
  void systemCallOverhead(fstream &);

  /**
    4. Task creation time:
    Report the time to create and run both a process and a kernel thread
  */
  void taskCreationTime(fstream &);

  void contextSwitchOverhead(fstream &file);

  double getProcessContextSwitchTime();
  double * getThreadContextSwitchTime();

  uint64_t calculateProcessSwitchTime(int *);
  uint64_t calculateThreadSwitchTime();
};



#endif
