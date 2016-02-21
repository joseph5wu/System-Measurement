#include "MemoryWriter.h"

void write_memory_memset(void* array, size_t size) {
     size_t * carray = (size_t *) array;
  memset(carray, 0xff, size);
}

void write_memory_loop_outsider(void * array, size_t SIZE) {
    size_t * carray = (size_t *) array;
  unsigned int  val = 0;
  for (int i = 0; i < SIZE /sizeof(size_t); i++) {
     carray[i] = 2;
     val ++;
  }
   //we need to use val, otherwise the compiler may optimize 
  // std::cout<<val;
  assert(!(val==0));
   //return val;
};


void write_memory_avx(void* array, size_t size) {
  __m256i* varray = (__m256i*) array;

  //__m256i vals = _mm256_set1_epi32(0xDEADBEEF);
    __m256i vals = _mm256_set1_epi32(0x00000001);
  size_t i;
  for (i = 0; i < size / sizeof(__m256i); i++) {
    _mm256_store_si256(&varray[i], vals);
  }
};