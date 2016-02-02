#include "benchmark.h"
#include "../util.h"
#include <math.h>
#define DATA_DIR "../../data/cpu/"
#define READ_OVERHEAD_FILE DATA_DIR "read_overhead.txt"
#define LOOP_OVERHEAD_FILE DATA_DIR "loop_overhead.txt"
#define PROCEDURE_CALL_OVERHEAD_FILE DATA_DIR "procedure_call_overhead.txt"
#define PROCESS_CONTEXT_SWITCH_OVERHEAD DATA_DIR "process_context_switch_overhead.txt"
#define THREAD_CONTEXT_SWITCH_OVERHEAD DATA_DIR "thread_context_switch_overhead.txt"
#define SYSTEM_CALL_OVERHEAD_FILE DATA_DIR "sys_call_overhead.txt"
#define CACHED_SYSTEM_CALL_OVERHEAD_FILE DATA_DIR "cached_sys_call_overhead.txt"
#define PROCESS_CREATION_TIME_FILE DATA_DIR "process_creation_time.txt"
#define THREAD_CREATION_TIME_FILE DATA_DIR "thread_creation_time.txt"

/**
  prototype for thread start routine
*/
void* threadStartRountine(void *);

double CPUBenchmark::getReadOverhead() {
  uint64_t sum = 0;
  uint64_t start, end;

  warmup();
  for(int i = 0; i < TIMES; i++) {
    start = rdtscStart();
    end = rdtscEnd();
    sum += (end - start);
  }

  return (double)sum / (double)TIMES;
}

uint64_t CPUBenchmark::getLoopOverhead(int times) {
  uint64_t start, end;

  warmup();
  start = rdtscStart();
  for(int i = 0; i < times; i++) {
    // end loop to avoid new overhead
  }
  end = rdtscEnd();

  return end - start;
}


void CPUBenchmark::getProcedureOverhead(vector<double> &result){
    uint64_t totalTime = 0;
    uint64_t start, end;

    //0 argument
    warmup();
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_0();
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[0] = (double)totalTime / (double)TIMES;

    //1 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_1(1);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[1] = (double)totalTime / (double)TIMES;

     //2 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_2(1, 2);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[2] = (double)totalTime / (double)TIMES;

     //3 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_3(1, 2, 3);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[3] = (double)totalTime / (double)TIMES;

     //4 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_4(1, 2, 3, 4);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[4] = (double)totalTime / (double)TIMES;

     //5 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_5(1, 2, 3, 4, 5);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[5] = (double)totalTime / (double)TIMES;

     //6 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_6(1, 2, 3, 4, 5, 6);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[6] = (double)totalTime / (double)TIMES;

     //7 argument
    warmup();
    totalTime = 0;
    for (int i = 0; i < TIMES; i++) {
      start = rdtscStart();
      fun_7(1, 2, 3, 4, 5, 6, 7);
      end = rdtscEnd();
      totalTime += (end - start);
    }
    result[7] = (double)totalTime / (double)TIMES;
}

double CPUBenchmark::getSystemCallOverhead(bool isCached) {
  uint64_t sum = 0;
  uint64_t start, end;

  warmup();
  for(int i = 0; i < TIMES; i++) {
    // in order to reduce overhead, leave the time count inside the if block
    if(isCached) {
      start = rdtscStart();
      // this one has cache
      getpid();
      end = rdtscEnd();
    }
    else {
      start = rdtscStart();
      // this one don't have cache
      getppid();
      end = rdtscEnd();
    }

    sum += (end - start);
  }

  return (double) sum / (double) TIMES;
}


double CPUBenchmark::getProcessCreationTime() {
  uint64_t sum = 0;
  uint64_t start, end;
  pid_t pid;

  warmup();
  for(int i = 0; i < TASK_OP_TIMES; i++) {
    start = rdtscStart();
    pid = fork();
    if(pid == 0) {
      // child process, just exit
      exit(1);
    }
    else {
      // parent process, wait child exit
      wait(NULL);
      end = rdtscEnd();
      sum += (end - start);
    }
  }

  return (double) sum / (double) TASK_OP_TIMES;
}

double CPUBenchmark::getProcessContextSwitchTime(){
  int fd[2];
  pipe(fd);
  uint64_t s[TASK_OP_TIMES];
  uint64_t totalTime = 0;
  int i = 0;

  while(i < TASK_OP_TIMES) {
    uint64_t res = calculateProcessSwitchTime(fd);
    if (res > 0) {
      s[i] = res;
      totalTime += s[i];
      i++;
    }
  }

  static double res;
  res = (double)totalTime / (double)TASK_OP_TIMES;

  return res;
}

double* CPUBenchmark::getThreadContextSwitchTime(){
  uint64_t s[TASK_OP_TIMES];
  uint64_t totalTime = 0.0;
  double tmp = 0.0;

  int i = 0;
    while (i < TASK_OP_TIMES) {
    uint64_t res = calculateThreadSwitchTime();
      if (res > 0) {
      s[i] = res;
      totalTime += s[i];
      i++;
    }
  }

    static double res[2];
    res[0] = (double)totalTime / (double)TASK_OP_TIMES;

    for(i = 0; i < TASK_OP_TIMES; ++i) {
    tmp += (double)(s[i] - res[0]) * (double)(s[i] - res[0]);
  }

     res[1] = sqrt(tmp / (double)(TASK_OP_TIMES - 1));
  return res;
}

uint64_t CPUBenchmark::calculateProcessSwitchTime(int *fd){
  uint64_t start;
  uint64_t end;
  pid_t cpid;
  uint64_t result = 0;
  warmup();
  if ((cpid = fork()) != 0) {
    start = rdtscStart();
    wait(NULL);
    read(fd[0], (void*)&end, sizeof(uint64_t));
  }
  else {
    end = rdtscEnd();
    write(fd[1], (void*)&end, sizeof(uint64_t));
    exit(1);
  }
  if(end > start){
    result = end - start;
  }
  return (result);
}

uint64_t CPUBenchmark::calculateThreadSwitchTime(){
  uint64_t start;
  uint64_t end;

  pthread_t thread;
  warmup();
  // pipe(fd);
  pthread_create(&thread, NULL, foo, &end);

  start = rdtscStart();
  pthread_join(thread, NULL);
  // read(fd[0], (void*)&end, sizeof(uint64_t));

    return end - start;
}


 void *  CPUBenchmark::foo(void * result)
 {
     uint64_t end = rdtscEnd();
     *((uint64_t*)result) = end;
    // write(fd[1], (void*)&t, sizeof(uint64_t));

    pthread_exit(NULL);
}
void* threadStartRountine(void *ptr) {
  // avoid of overhead
  pthread_exit(0);
}

double CPUBenchmark::getKernelThreadCreationTime() {
  double sum = 0;
  uint64_t start, end;
  pthread_t thread;

  warmup();
  for(int i = 0; i < TASK_OP_TIMES; i++) {
    start = rdtscStart();
    pthread_create(&thread, NULL, &threadStartRountine, NULL);
    // make the main process to wait new thread
    pthread_join(thread, NULL);
    end = rdtscEnd();
    sum += end - start;
  }

  return (double) sum / (double) TASK_OP_TIMES;
}

void CPUBenchmark::measurementOverhead(fstream &file) {
  cout << "1. Measurement overhead starts:" << endl;

  double overhead;
  double sum = 0;
  cout << "1.1 Get read overhead: ";
  file.open(READ_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getReadOverhead();
      file << overhead << "\n";
      sum += overhead;
    }
    cout << (sum / OP_TIMES) << " cycles" << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << READ_OVERHEAD_FILE << endl;
  }

  cout << "1.2 Get loop overhead:" << endl;
  file.open(LOOP_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    int LOOP_TIMES_ARRAY[15] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
    for(int i = 0; i < 15; i++) {
      overhead = 0;
      int times = 0;
      int loopCounts = 0;
      if(i == 0) {
        times = TIMES;
        loopCounts = TIMES;
      }
      else {
        times = TIMES / LOOP_TIMES_ARRAY[i];
        loopCounts = times * LOOP_TIMES_ARRAY[i];
      }


      for(int j = 0; j < times; j++) {
        overhead += getLoopOverhead(LOOP_TIMES_ARRAY[i]);
      }

      file << LOOP_TIMES_ARRAY[i] << " " << overhead / times << " ";

      cout << "Loop #" << LOOP_TIMES_ARRAY[i] << ": " << overhead / times << " ";

      if(i != 0) {
        file << (double) overhead / loopCounts << "\n";
        cout << (double) overhead / loopCounts << " cycles" << endl;
      }
      else {
        file << (double) overhead / times << "\n";
        cout << (double) overhead / times << " cycles" << endl;
      }
    }
    file.close();
  }
  else {
    cout << "Can't open file-" << LOOP_OVERHEAD_FILE << endl;
  }
}

void CPUBenchmark::procedureCallOverhead(fstream &file){
  cout << "2. Procedure Call Overhead Starts:" << endl;
  file.open(PROCEDURE_CALL_OVERHEAD_FILE, ios::out);

  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
    vector<double> result(8, 0.0);
    getProcedureOverhead(result);

    file << result[0] << " "
       << result[1] << " "
       << result[2] << " "
       << result[3] << " "
       << result[4] << " "
       << result[5] << " "
       << result[6] << " "
       << result[7] << "\n";
    /*cout <<"operation: "  << i << "0 arg: " << result[0] << " "
       << "1 arg: " << result[1] << " "
       << "2 arg: " << result[2] << " "
       << "3 arg: " << result[3] << " "
       << "4 arg: " << result[4] << " "
       << "5 arg: " << result[5] << " "
       << "6 arg: " << result[6] << " "
       << "7 arg: " << result[7] << "\n";*/
    //double increment = (result[7] + result[6] + result[5] + result[4] - result[3] - result[2] - result[1] - result[0]) / 16;
      //  file << increment << "\n";
        //cout << "increment" << increment << "\n";
      }
      file.close();
  }
  else{
    cout << "Can't open file-" << PROCEDURE_CALL_OVERHEAD_FILE << endl;
    return;
  }
}

void CPUBenchmark::systemCallOverhead(fstream &file) {
  cout << "3. System call overhead starts:" << endl;

  double overhead;
  double sum = 0;
  cout << "3.1 Get non-cached system call overhead overhead: ";
  file.open(SYSTEM_CALL_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getSystemCallOverhead(false);
      file << overhead << "\n";
      sum += overhead;
    }
    cout << (sum / OP_TIMES) << " cycles" << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << SYSTEM_CALL_OVERHEAD_FILE << endl;
  }

  sum = 0;
  cout << "3.2 Get cached system call overhead overhead: ";
  file.open(CACHED_SYSTEM_CALL_OVERHEAD_FILE, ios::out);
  if(file.is_open()) {
    for(int i = 0; i < OP_TIMES; i++) {
      overhead = getSystemCallOverhead(true);
      file << overhead << "\n";
      sum += overhead;
    }
    cout << (sum / OP_TIMES) << " cycles" << endl;
    file.close();
  }
  else {
    cout << "Can't open file-" << CACHED_SYSTEM_CALL_OVERHEAD_FILE << endl;
  }
}


void CPUBenchmark::taskCreationTime(fstream &file) {
  cout << "4. Task creation time starts:" << endl;

  double overhead;
  double sum = 0;
  cout << "4.1 Time to create and run a process: ";
  cout.flush();
  file.open(PROCESS_CREATION_TIME_FILE, ios::out);
  if(file.is_open()) {
      for(int i = 0; i < OP_TIMES; i++) {
        overhead = getProcessCreationTime();
        file << overhead << "\n";
        file.flush();
        sum += overhead;
      }
      cout << (sum / OP_TIMES) << " cycles" << endl;
      file.close();
  }
  else {
    cout << "Can't open file-" << PROCESS_CREATION_TIME_FILE << endl;
  }

  cout << "4.2 Time to create and run a kernel-managed thread: ";
  sum = 0;
  file.open(THREAD_CREATION_TIME_FILE, ios::out);
  if(file.is_open()) {
      for(int i = 0; i < OP_TIMES; i++) {
        overhead = getKernelThreadCreationTime();
        file << overhead << "\n";
        sum += overhead;
      }
      cout << (sum / OP_TIMES) << " cycles" << endl;
      file.close();
  }
  else {
    cout << "Can't open file-" << THREAD_CREATION_TIME_FILE << endl;
  }
}

void CPUBenchmark::contextSwitchOverhead(fstream &file){
  cout << "5. Context Switch Overhead:" << endl;

  double sum = 0;
  cout << "5.1 Process Context Switch Overhead: ";
  cout.flush();
  file.open(PROCESS_CONTEXT_SWITCH_OVERHEAD, ios::out);
  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
      double contextSwitchAvg = getProcessContextSwitchTime();
      file << contextSwitchAvg << "\n";
      sum += contextSwitchAvg;
    }
    cout << sum / OP_TIMES << endl;
    file.close();
  }else{
    cout << "File open failed!" << " cycles" << endl;
    return;
  }

  cout << "5.2 Thread Context Switch Overhead: ";
  file.open(THREAD_CONTEXT_SWITCH_OVERHEAD, ios::out);
  sum = 0;
  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
      double* kernelSwitchAvg = getThreadContextSwitchTime();
     //  file << kernelSwitchAvg[0] << " "
     //       << kernelSwitchAvg[1] << "\n";
      file << kernelSwitchAvg[0] << "\n";
      sum += kernelSwitchAvg[0];
    }
    cout << sum / OP_TIMES << " cycles" << endl;
    file.close();
  }else{
    cout << "File open failed!" << endl;
    return;
  }
}
