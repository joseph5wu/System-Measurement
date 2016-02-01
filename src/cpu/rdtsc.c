#include "CPU.h"
#include "util.h"
#include <inttypes.h>
void * runThread(void *) {
  pthread_exit(NULL);
}

//Measure read overhead
double CPU::getReadOverhead() {
  double sum = 0;
  uint64_t start, end;
  rdtsc();
  for (int i = 0; i < TIMES; i++) {
    start = rdtsc();
    end = rdtsc();
    sum = sum + (end - start);
  }

  return ((double)sum) / ((double)(TIMES));
}

// Measure loop overhead
/*
double CPU::getLoopOverhead() {
  uint64_t start;
  uint64_t end;

  rdtsc();
  uint64_t total_time = 0;
  start = rdtsc();
  for (int i = 0; i < TIMES; i++) {
  }
  end = rdtsc();
  total_time += end - start;

  return (double)total_time / TIMES;
}
*/

double CPU::getLoopOverhead() {
    int LOOPTIME = 100;
    unsigned base=0, iterations=1, sum=0;
    uint32_t cycles_high1=0, cycles_low1=0;
    uint32_t cycles_high2=0, cycles_low2=0;
    uint64_t temp_cycles1=0, temp_cycles2=0;
    int64_t total_cycles=0; // Stored signed so it can be converted
    // to a double for viewing
    double seconds=0.0L;
    unsigned mhz=150; // If you want a seconds count instead
    // of just cycles, enter the MHz of your
    // machine in this variable.
    base=0;
//https://www.ccsl.carleton.ca/~jamuir/rdtscpm1.pdf
    __asm__ (
    "push %rax\n\t"
    "push %ECX\n\t"
    "push %EDX\n\t"
    "push %EBX\n\t"
    "push %ESP\n\t"
    "push %EBP\n\t"
    "push %ESI\n\t"
    "push %EDI\n\t"
    );
//http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-manual-325462.pdf
    __asm__ (
    "xor %%eax, %%eax;" 
    "cpuid;" 
    "rdtsc;"
    : "=a" (cycles_high1), "=d" (cycles_low1));




    __asm__ (
    "Pop %EDI\n\t"
    "Pop %ESI\n\t"
    "Pop %EBP\n\t"
    "Pop %ESP\n\t"
    "Pop %EBX\n\t"
    "Pop %EDX\n\t"
    "Pop %ECX\n\t"
    "Pop %EAX\n\t"
    );

 for (int i=0; i<LOOPTIME; i++) {
     
 }
 // *** BEGIN OF INCLUDE SECTION 4
 // *** INCLUDE THE FOLLOWING CODE IMMEDIATELY BEFORE SECTION
 // *** TO BE MEASURED
 
 // *** END OF INCLUDE SECTION 4
 // User code to be measured is in this section.
 //Sleep(3000);
 // *** BEGIN OF INCLUDE SECTION 5
 // *** INCLUDE THE FOLLOWING CODE IMMEDIATELY AFTER SECTION
 // *** TO BE MEASURED
 
    __asm__ (
    "push %rax\n\t"
    "push %ECX\n\t"
    "push %EDX\n\t"
    "push %EBX\n\t"
    "push %ESP\n\t"
    "push %EBP\n\t"
    "push %ESI\n\t"
    "push %EDI\n\t"
    );
    
    __asm__ (
    "xor %%eax, %%eax;" 
    "cpuid;" 
    "rdtsc;"
    : "=a" (cycles_high2), "=d" (cycles_low2));


    __asm__ (
    "Pop %EDI\n\t"
    "Pop %ESI\n\t"
    "Pop %EBP\n\t"
    "Pop %ESP\n\t"
    "Pop %EBX\n\t"
    "Pop %EDX\n\t"
    "Pop %ECX\n\t"
    "Pop %EAX\n\t"
    );

 // Move the cycle counts into 64-bit integers
 temp_cycles1 = ((uint64_t)cycles_high1 << 32) | cycles_low1;
 temp_cycles2 = ((uint64_t)cycles_high2 << 32) | cycles_low2;

 // Add to total cycle count
 total_cycles += temp_cycles2 - temp_cycles1 - base;
 //iterations++;
 // *** END OF INCLUDE SECTION 5

  seconds = (double)total_cycles/double(mhz*1000000);
  printf("Average cycles per loop: %f\n", (double)total_cycles/double(iterations));
  //printf("Average seconds per loop: %f\n", seconds/iterations);

 return seconds;

}

// Measure procedure call overhead with different size of arrays
void CPU::getProcedureOverhead(vector<double> &ans) {
  uint64_t start;
  uint64_t end;
  uint64_t s = 0;
  uint64_t total_time = 0;
  rdtsc();

  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test0();
      end = rdtsc();

      total_time += (end - start);
  }
  ans[0] = (double)total_time / (double)TIMES;

  rdtsc();
  total_time = 0;
  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test1(1);
      end = rdtsc();

      total_time += end - start;
  }
  ans[1] = (double)total_time / (double)TIMES;

  rdtsc();
  total_time = 0;
  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test2(1, 2);
      end = rdtsc();

      total_time += end - start;
  }
  ans[2] = (double)total_time / (double)TIMES;

  rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test3(1, 2, 3);
        end = rdtsc();

        total_time += end - start;
    }
    ans[3] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test4(1, 2, 3, 4);
        end = rdtsc();

        total_time += end - start;
    }
    ans[4] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test5(1, 2, 3, 4, 5);
        end = rdtsc();

        total_time += end - start;
    }
    ans[5] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test6(1, 2, 3, 4, 5, 6);
        end = rdtsc();

        total_time += end - start;
    }
    ans[6] = total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test7(1, 2, 3, 4, 5, 6, 7);
        end = rdtsc();

        total_time += end - start;
    }
    ans[7] = (double)total_time / (double)TIMES;

  return;
}

// Measure system call overhead
double CPU::sysCallOverhead() {
  uint64_t start;
  uint64_t end;
  uint64_t total_time = 0;
  
  for (int i = 0; i < TIMES; i++) {
    rdtsc();
    start = rdtsc();
    getppid();
    end = rdtsc();

        total_time += end - start;
    }
  
  return (double)total_time / (double)TIMES;
}

// Measure thread create overhead by using posix thread library
double CPU::threadCreationOverhead() {
  pthread_t td;
  uint64_t start, end, total_time;
  total_time = 0;
  
  for (int i = 0; i < CREAT_TIMES; i++) {
    rdtsc();
    start = rdtsc();

    pthread_create(&td, NULL, runThread, NULL);
    pthread_join(td, NULL);

        end = rdtsc();
    total_time += end - start;
     }
  
  return (double)total_time / (double)CREAT_TIMES;
}

// Measure process creation overhead
double CPU::processCreationOverhead() {
  uint64_t start;
  uint64_t end;
  uint64_t total_time = 0;
  pid_t pid;
  
  for (int i = 0; i < CREAT_TIMES; i++) {
    rdtsc();
    start = rdtsc();
    pid = fork();
    
    if (pid == 0) {
      exit(1);
    }
    else {
      wait(NULL);
      end = rdtsc();
      total_time = total_time + end - start;
    }
  }
  
  return (double)total_time / (double)CREAT_TIMES;

}

// Measure process context switch overhead
uint64_t CPU::calculateSwitchTime(int *fd) {
  uint64_t start;
  uint64_t end;
  pid_t cpid;
  uint64_t result = 0;

  if ((cpid = fork()) != 0) {
    rdtsc();
    start = rdtsc();

    wait(NULL);
    read(fd[0], (void*)&end, sizeof(uint64_t));
  }
  else {
    end = rdtsc();

    write(fd[1], (void*)&end, sizeof(uint64_t));
    exit(1);
  }
  if(end > start){
    result = end - start;
  }
  return (result);
}

// Helper funtion to calculate average process context switch time
double* CPU::getContextSwitchTime() {
  int fd[2];
  pipe(fd);
  uint64_t s[CREAT_TIMES];
  double sum = 0;
  int i = 0;

  while(i < CREAT_TIMES) {
    uint64_t res = calculateSwitchTime(fd);
    if (res > 0) {
      s[i] = res;
      sum += s[i];
      ++i;
    }
  }

//  static double res;
  static double res[2];
  res[0] = (double)sum / (double)CREAT_TIMES;

  return res;
}

//Calculate thread context switch time
uint64_t CPU::calculateKernelSwitch() {
  uint64_t threadSt;
  uint64_t threadEd;

  pthread_t thread;
  pipe(fd);
  pthread_create(&thread, NULL, foo, NULL);

  rdtsc();

  threadSt = rdtsc();
  pthread_join(thread, NULL);
  read(fd[0], (void*)&threadEd, sizeof(uint64_t));

    return threadEd - threadSt;
}

// Helper funtion to calculate average thread context switch time
double* CPU::getKernelSwitchOverhead() {
  uint64_t s[CREAT_TIMES];
  double sum = 0.0;
  double tmp = 0.0;

  int i = 0;
    while (i < CREAT_TIMES) {
    uint64_t res = calculateKernelSwitch();

        if (res > 0) {
      s[i] = res;
      sum += s[i];
      i++;
    }
  }

    static double res[2];
  res[0] = sum / (double)CREAT_TIMES;

    for(i = 0; i < CREAT_TIMES; ++i) {
    tmp += (double)(s[i] - res[0]) * (double)(s[i] - res[0]);
  }

    res[1] = sqrt(tmp / (double)(CREAT_TIMES - 1));
  return res;
}

// From here are all the util function to wrap around previous actuall test functions
void CPU::testReadOverhead(fstream &file) {
  cout << "Getting Read Overhead..." << endl;

  file.open("Read_Overhead.txt", ios::out);
    if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << getReadOverhead() << "\n";
  }
  file.close();
}

void CPU::testLoopOverhead(fstream &file) {
  cout << "Getting Loop Overhead..." << endl;

    file.open("Loop_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << getLoopOverhead() << "\n";
  }
  file.close();
}

void CPU::testProcedureCallOverhead(fstream &file) {
  cout << "Getting Procedure Call Overhead..." << endl;
  file.open("Procedure_Call_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    vector<double> result(8, 0.0);
    getProcedureOverhead(result);

    file << setiosflags(ios::fixed)
       << result[0] << " "
         << result[1] << " "
       << result[2] << " "
       << result[3] << " "
       << result[4] << " "
       << result[5] << " "
       << result[6] << " "
       << result[7] << "\n";
    double increment = (result[7] + result[6] + result[5] + result[4] - result[3] - result[2] - result[1] - result[0]) / 16;
        file << setiosflags(ios::fixed) << increment << "\n";
  }
  file.close();
}

void CPU::testSystemCallOverhead(fstream &file) {
  cout << "Getting System Call Overhead..." << endl;
    file.open("System_Call_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
    for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << sysCallOverhead() << "\n";
  }
  file.close();
}

void CPU::testThreadCreationOverhead(fstream &file) {
  cout << "Getting Thread Creation Overhead..." << endl;
  file.open("Thread_Creation_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << threadCreationOverhead() << "\n";
  }
  file.close();
}

void CPU::testProcessCreationOverhead(fstream &file) {
  cout << "Getting Process Creation Overhead..." << endl;
  file.open("Process_Creation_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << processCreationOverhead() << "\n";
  }
  file.close();
}

void CPU::testContextSwitchOverhead(fstream &file) {
  cout << "Getting Context Switch Overhead..." << endl;
    file.open("Context_Switch_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    double * contextSwitchAvg = getContextSwitchTime();

    file << setiosflags(ios::fixed)
       << contextSwitchAvg[0] << "\n";
  }
  file.close();
}

void CPU::testThreadSwitchOverhead(fstream &file) {
  cout << "Getting Thread Switch Overhead..." << endl;
    file.open("Kernel_Switch_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    double* kernelSwitchAvg = getKernelSwitchOverhead();

    file <<  setiosflags(ios::fixed) 
       << kernelSwitchAvg[0] << " " 
       << kernelSwitchAvg[1] << "\n";
  }
  file.close();
}