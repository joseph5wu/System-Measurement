#include <iostream>
#include "../util.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#ifdef __AVX__
#include <immintrin.h>
#endif

#define SAMPLES 5
#define MULTIPLICATION 5

#define BYTES_PER_GB (1024*1024*1024LL)
#define SIZE (1*BYTES_PER_GB)

char array[SIZE] __attribute__((aligned (32)));
//reference http://codearcana.com/posts/2013/05/18/achieving-maximum-memory-bandwidth.html
//https://www.akkadia.org/drepper/cpumemory.pdf

double bw_calculator(unsigned int size_gb, double secs) {
  return size_gb / secs;
}


unsigned int read_memory_loop() {
  unsigned int  val = 0;
  for (int i = 0; i < SIZE ; i++) {
    val += array[i];
  }
   //we need to use val, otherwise the compiler may optimize 
  // std::cout<<val;
   return val;


}


void read_memory_loop_wrapper(){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    unsigned int fake_receiver = 0;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            fake_receiver = read_memory_loop();
        }
        
        end = monotonic_time();
        
        
        rawTime = end - start;
        if (first_loop) {
            resultTime = rawTime;
            first_loop = 0;
        }
        else if ( resultTime > rawTime) 
            resultTime = rawTime;
     }
     std::err<<fake_receiver;
     std::cout <<"reading"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<std::endl;
     
     
}


int main() {
  memset(array, 0xFF, SIZE);  // un-ZFOD the page.
  * ((uint64_t *) &array[SIZE]) = 0;
  
  read_memory_loop_wrapper();
  return 0;
}
  