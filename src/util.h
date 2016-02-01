#ifndef UTIL_H
$define UTIL_H

// every simple instruction execution times
#define TIMES 100000
// every demanded operation ececution times
#define OP_TIMES 10

static inline void warmup(void) {
  // TODO
}

static inline uint64_t rdtsc(void) {
  uint32_t lo, hi;
  __asm__ __volatile__("pusha;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
  __asm__ __volatile__("popa")
  return (((uint64_t)hi << 32) | lo);
}
