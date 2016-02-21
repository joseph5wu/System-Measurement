#include <iostream>
#include "../util.h"
#include "MemoryWriter.h"

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


void read_memory_loop() {
  unsigned int  val = 0;
  for (int i = 0; i < SIZE /sizeof(char); i++) {
    val += array[i];
  //  __builtin_prefetch (&array[i+1], 0, 0);
  }
   //we need to use val, otherwise the compiler may optimize 
  // std::cout<<val;
  assert(!(val==0));
  //return val;
}




void write_memory_loop() {
  unsigned int  val = 0;
  for (int i = 0; i < SIZE /sizeof(char); i++) {
     array[i] = 2;
     val ++;
  }
   //we need to use val, otherwise the compiler may optimize 
  // std::cout<<val;
  assert(!(val==0));
   //return val;
}


//ref http://codearcana.com/posts/2013/05/18/achieving-maximum-memory-bandwidth.html
__m256 read_memory_avx() {
  __m256* varray = (__m256*) array;
  //__m256 accum = _mm256_set1_ps((float) 0xDEADBEEF);
  __m256 accum = _mm256_set1_ps((float)   0x00000000);
  for (int i = 0; i < SIZE/sizeof(__m256i); i++) { // here is __m256i
    accum = _mm256_add_ps(varray[i], accum);
  }

assert(!_mm256_testz_ps(accum, accum));
  return accum;
}

unsigned int read_memory_loop_unroll() {
  unsigned int  val = 0;
  register int size = SIZE - 8;
  register int i = 0;
  for (i = 0; __builtin_prefetch(&array[i+8],0,1),i < size ; i+=8) {
    val += array[i] ;
    val += array[i+1];
    val += array[i+2];
    val += array[i+3];
    val += array[i+4];
    val += array[i+5];
    val += array[i+6];
    val += array[i+7];
    
    
  }
   //we need to use val, otherwise the compiler may optimize 
  // std::cout<<val;
   return val;


}

void read_memory_loop_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    unsigned int fake_receiver = 0;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
             read_memory_loop();
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
     std::cerr<<fake_receiver;
     std::cout <<"reading"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}

void read_memory_loop_unroll_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    unsigned int fake_receiver = 0;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            fake_receiver = read_memory_loop_unroll();
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
     std::cerr<<fake_receiver;
     std::cout <<"reading"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}

void read_memory_avx_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    __m256  fake_receiver = _mm256_set1_ps((float)   0x00000000);
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            fake_receiver = read_memory_avx();
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
     //make sure not optmized away
  assert(!_mm256_testz_ps(fake_receiver, fake_receiver));
     std::cout <<"reading"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}



void write_memory_loop_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            write_memory_loop();
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
     //again, do not optimize me !!
     int val = 0;
     for (int i = 0 ; i < SIZE; i ++) {
         val += array[i];
         
         //std::cout<<array[i];
      
      }
     std::cout<<val;
     std::cout <<"write"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}


void write_memory_loop_outsider_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            write_memory_loop_outsider(array,SIZE);
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
     //again, do not optimize me !!
     int val = 0;
     for (int i = 0 ; i < SIZE; i ++) {
         val += array[i];
         
         //std::cout<<array[i];
      
      }
     std::cout<<val;
     std::cout <<"write"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}

void write_memory_avx_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
            write_memory_avx(array,SIZE);
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
     //again, do not optimize me !!
     int val = 0;
     for (int i = 0 ; i < SIZE; i ++) {
         val += array[i];
         
         //std::cout<<array[i];
      
      }
     std::cout<<val;
     std::cout <<"write"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}


void write_memory_memset_wrapper( ){
    double  start;
    double  end;
    double  rawTime;
    double  resultTime; //resultTime is the smallest value
   
    int first_loop = 1;
    for ( int i  = 0 ; i < SAMPLES; i++) {
        start = monotonic_time();
        for (int j  = 0; j < MULTIPLICATION; j ++) {
           write_memory_memset(array,SIZE);
          // memset(array, '$', SIZE);
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
     //again, do not optimize me !!
     int val = 0;
     for (int i = 0 ; i < SIZE; i ++) {
         val += array[i];
         
         //std::cout<<array[i];
      
      }
      //int tt = rand()%SIZE;
      
    // std::cout<<val<<"hehe"<<array[tt];
     std::cout <<"write"<<MULTIPLICATION<<"GB"<<" result Time is "<<resultTime<<std::endl;
     std::cout <<" bandwidth"<< bw_calculator(MULTIPLICATION , resultTime)<<"GB/s"<<std::endl;
     
     
}



int main() {
  memset(array, 0xFF, SIZE);  
  
  std::cout<<" read_memory_loop"<<std::endl;
  read_memory_loop_wrapper();
  
 // read_memory_loop_unroll_wrapper();
 //we instead us complier unroll here 
 //http://stackoverflow.com/questions/11529006/can-modern-compilers-unroll-for-loops-expressed-using-begin-and-end-iterators
 //https://software.intel.com/en-us/articles/introduction-to-intel-advanced-vector-extensions
 
 
  std::cout<<"read_memory_avx"<<std::endl;
  read_memory_avx_wrapper();
  
  //this will be optimized
  std::cout<<"write_memory_loop <---------- this must be error"<<std::endl;
 write_memory_loop_wrapper();
  
  //should use this ,
  std::cout<<"write_memory_loop_outsider_wrapper"<<std::endl;
  write_memory_loop_outsider_wrapper();
  
  std::cout<<"write_memory_avx_wrapper"<<std::endl;
  write_memory_avx_wrapper();
  
  std::cout<<"write_memory_memset_wrapper"<<std::endl;
  write_memory_memset_wrapper();
  return 0;
}
  