#include <assert.h>
#include <stdint.h>
#include <immintrin.h>
#include <stdio.h>
#include <string>
void write_memory_loop_outsider(void * array, size_t SIZE);
void write_memory_avx(void* array, size_t size) ;


void write_memory_memset(void* array, size_t size);