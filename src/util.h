#ifndef UTIL_H
#define UTIL_H

// every simple instruction execution times
#define TIMES 100000
// every demanded operation execution times
#define OP_TIMES 10
// every task operation execution times
#define TASK_OP_TIMES 200

static inline void warmup(void) {
  // TODO
}

static inline uint64_t rdtsc(void) {
  uint32_t lo, hi;
  // __asm__ __volatile__("pusha;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
  // __asm__ __volatile__("popa");
  __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));

  return (((uint64_t)hi << 32) | lo);
}

static inline void fun_0(){}

static inline void fun_1(int arg1){}

static inline void fun_2(int arg1, int arg2){}

static inline void fun_3(int arg1, int arg2, int arg3){}

static inline void fun_4(int arg1, int arg2, int arg3, int arg4){}

static inline void fun_5(int arg1, int arg2, int arg3, int arg4, int arg5){}

static inline void fun_6(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6){}

static inline void fun_7(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7){}

#endif
