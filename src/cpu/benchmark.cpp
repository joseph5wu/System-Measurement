#include "benchmark.h"
#include "../util.h"

#define DATA_DIR "../../data/cpu/"
#define READ_OVERHEAD_FILE DATA_DIR "read_overhead.txt"
#define LOOP_OVERHEAD_FILE DATA_DIR "loop_overhead.txt"
#define PROCEDURE_CALL_OVERHEAD_FILE DATA_DIR "procedure_call_overhead.txt"
#define PROCESS_CONTEXT_SWITCH_OVERHEAD DATA_DIR "process_context_switch_overhead.txt"
#define THREAD_CONTEXT_SWITCH_OVERHEAD DATA_DIR "thread_context_switch_overhead.txt"
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
    uint64_t totalTime = 0;
    uint64_t start, end;

    //0 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_0();
      end = rdtsc();
      totalTime += (end - start);
    }
    result[0] = (double)totalTime / (double)TIMES;
    
    //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_1(1);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[1] = (double)totalTime / (double)TIMES;

     //2 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_2(1, 2);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[2] = (double)totalTime / (double)TIMES;

     //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_3(1, 2, 3);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[3] = (double)totalTime / (double)TIMES;

     //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_4(1, 2, 3, 4);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[4] = (double)totalTime / (double)TIMES;

     //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_5(1, 2, 3, 4, 5);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[5] = (double)totalTime / (double)TIMES;

     //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_6(1, 2, 3, 4, 5, 6);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[6] = (double)totalTime / (double)TIMES;

     //1 argument
    rdtsc();
    for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      fun_7(1, 2, 3, 4, 5, 6, 7);
      end = rdtsc();
      totalTime += (end - start);
    }
    result[7] = (double)totalTime / (double)TIMES;

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

double CPUBenchmark::getProcessContextSwitchTime(){
  int fd[2];
  pipe(fd);
  uint64_t s[CREAT_TIMES];
  double sum = 0;
  int i = 0;

  while(i < CREAT_TIMES) {
    uint64_t res = calculateProcessSwitchTime(fd);
    if (res > 0) {
      s[i] = res;
      sum += s[i];
      ++i;
    }
  }

  static double res;
  res = (double)sum / (double)CREAT_TIMES;

  return res;
}

double* CPUBenchmark::getThreadContextSwitchTime(){

}

uint64_t CPUBenchmark::calculateProcessSwitchTime(int *){

}

uint64_t CPUBenchmark::calculateThreadSwitchTime(){

}


void CPUBenchmark::prepare() {
  warmup();
  // cout << "warmup starts" << endl;
}

void CPUBenchmark::procedureCallOverhead(fstream &file){
  cout << "Getting Procedure Call Overhead..." << endl;
  file.open(PROCEDURE_CALL_OVERHEAD_FILE, ios::out);
  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
    vector<double> result(8, 0.0);
    getProcedureOverhead(result);

    file <<"operation "  << i << ":  0 arg: " << result[0] << " "
       << "1 arg: " << result[1] << " "
       << "2 arg: " << result[2] << " "
       << "3 arg: " << result[3] << " "
       << "4 arg: " << result[4] << " "
       << "5 arg: " << result[5] << " "
       << "6 arg: " << result[6] << " "
       << "7 arg: " << result[7] << "\n";
    cout <<"operation "  << i << ":  0 arg: " << result[0] << " "
       << "1 arg: " << result[1] << " "
       << "2 arg: " << result[2] << " "
       << "3 arg: " << result[3] << " "
       << "4 arg: " << result[4] << " "
       << "5 arg: " << result[5] << " "
       << "6 arg: " << result[6] << " "
       << "7 arg: " << result[7] << "\n";
    double increment = (result[7] + result[6] + result[5] + result[4] - result[3] - result[2] - result[1] - result[0]) / 16;
        file << increment << "\n";
        cout << "increment" << increment << "\n";
  }
  file.close();
  }
  else{
    cout << "Can't open file-" << PROCEDURE_CALL_OVERHEAD_FILE << endl;
    return;
  }
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
void CPUBenchmark::contextSwitchOverhead(fstream &file){
  cout << "Getting Context Switch Overhead..." << endl;
  file.open(PROCESS_CONTEXT_SWITCH_OVERHEAD, ios::out);
  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
      double contextSwitchAvg = getProcessContextSwitchTime();
      file << setiosflags(ios::fixed)
        << contextSwitchAvg[0] << "\n";
    }
    file.close();
  }else{
    cout << "File open failed!" << endl;
    return;
  }

  cout << "Getting Thread Switch Overhead..." << endl;
  file.open(THREAD_CONTEXT_SWITCH_OVERHEAD, ios::out);
  if (file.is_open()) {
    for (int i = 0; i < OP_TIMES; i++) {
      double* kernelSwitchAvg = getThreadContextSwitchTime();
       file <<  setiosflags(ios::fixed) 
            << kernelSwitchAvg[0] << " " 
            << kernelSwitchAvg[1] << "\n";
      }
  file.close();
}else{
  cout << "File open failed!" << endl;
    return;
}
}
