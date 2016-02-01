#ifndef UTIL_H
#define UTIL_H

// every simple instruction execution times
#define TIMES 100000
// every demanded operation ececution times
#define OP_TIMES 10


static inline unsigned getBase(void) {
    unsigned base,base_extra=0;
    unsigned cycles_low, cycles_high;
    
    //1.
     //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    
    __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (cycles_low), "=d" (cycles_high));
    
     //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    __asm__ __volatile__("cpuid;" "rdtsc;");
    
    //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    
     //2.
     //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    
    __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (cycles_low), "=d" (cycles_high));
   
     //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    __asm__ __volatile__("cpuid;" "rdtsc;");
    
    //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    
    
     //3.
     //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    
    __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (cycles_low), "=d" (cycles_high));
    
     //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    __asm__ __volatile__("cpuid;" "rdtsc;");
   //__asm__("sub %eax, $cycles_low");
  //  __asm__("mov base_extra, %eax");
    //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    
    
    //4
    //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    
    __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (cycles_low), "=d" (cycles_high));
    
     //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");
    __asm__ __volatile__("cpuid;" "rdtsc;");
   // __asm__("sub %eax, cycles_low");
   // __asm__("mov base_extra, %eax");
    //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");
    
    
    if (base_extra < base)
        base = base_extra;
    return base;
    
    
    
    
}

static inline void warmup(void) {
   getBase();
}

static inline uint64_t rdtsc(void) {
   uint32_t lo, hi;
   
   //pushfd
    __asm__("push %rax");
    __asm__("push %rcx");
    __asm__("push %rdx");
    __asm__("push %rbx");

    __asm__ __volatile__("cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
 
    //popfd
    __asm__("pop %rax");
    __asm__("pop %rcx");
    __asm__("pop %rdx");
    __asm__("pop %rbx");


  return (((uint64_t)hi << 32) | lo);
}

#endif
