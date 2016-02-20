#include <iostream>
#include "../util.h"


#define STRIDES_NUMS 4
#define ARRAYS_NUMS 13
//#define ARRAYS_NUMS 10
#define KB 1024
#define ITERATION 1000000
//#define ITERATION 100000


//no random, no 10 base
void latency1 (uint64_t  arraySize, uint64_t  strideSize) { 
    double start;
    double end;
    double rawTime;
    //check the word, hence use char pointer 
    char ** p_list = new char* [arraySize]; 
    for ( int i =0; i < arraySize ; i ++) {
        uint64_t  index =  i/strideSize * strideSize + strideSize;
        p_list [i] = (char * ) &p_list[index];     // pointer at i point to index 
    }
    
    char ** p = p_list;
    
    uint64_t Count = arraySize/ (strideSize ) + 1;
    //why base 10 , because check bound each time cost a lot of time
    start = monotonic_time();
    for ( int i = 0 ; i < ITERATION; i ++ ) {
         for ( int j = 0; j < Count ; j ++) {
             
             p = (char**) *p;
             
             }
         p = p_list;
     }
     end = monotonic_time();
     rawTime = (end - start);
     std::cout <<"count"<<Count<<"raw time" << rawTime<<std::endl;
     double time = (double)rawTime / (ITERATION *Count);
     delete [] p_list;
     
     std::cout<<"arraySize: "<<(arraySize * 8) / KB<<"KB strideSize: "<<strideSize<<" taking time.... "<<time <<std::endl;
     

}

// random, but not base 10 
void latency2 (uint64_t  arraySize, uint64_t  strideSize) { 
    double start;
    double end;
    double rawTime;
    //check the word, hence use char pointer 
    char ** p_list = new char* [arraySize]; 
    for ( int i =0; i < arraySize ; i ++) {
        uint64_t  index =  i/strideSize * strideSize + rand() % strideSize % arraySize;
        p_list [i] = (char * ) &p_list[index];     // pointer at i point to index 
    }
    
    char ** p = p_list;
    
    uint64_t Count = arraySize/ (strideSize ) + 1;
    //why base 10 , because check bound each time cost a lot of time
    start = monotonic_time();
    for ( int i = 0 ; i < ITERATION; i ++ ) {
         for ( int j = 0; j < Count ; j ++) {
             
             p = (char**) *p;
             
             }
         p = p_list;
     }
     end = monotonic_time();
     rawTime = (end - start);
    // std::cout <<"raw time" << rawTime<<std::endl;
     double time = (double)rawTime / (ITERATION *Count);
     delete [] p_list;
     
     std::cout<<"arraySize: "<<(arraySize * 8) / KB<<"KB strideSize: "<<strideSize<<" taking time.... "<<time <<std::endl;
     

}


void latency3 (uint64_t  arraySize, uint64_t  strideSize) { 
    double start;
    double end;
    double rawTime;
    //check the word, hence use char pointer 
    char ** p_list = new char* [arraySize]; 
    for ( int i =0; i < arraySize ; i ++) {
        uint64_t  index =  i/strideSize * strideSize + rand() % strideSize % arraySize;
        p_list [i] = (char * ) &p_list[index];     // pointer at i point to index 
    }
    
    
    
    
    char ** p = p_list;
    
    //use 10 count instead, check 10 load each time instead one load 
    uint64_t TenLoadBaseIteration =  ITERATION / 10 ;
    uint64_t TenLoadBaseCount = arraySize/ (strideSize * 10) + 1;
    //why base 10 , because check bound each time cost a lot of time
    start = monotonic_time();
    for ( int i = 0 ; i < TenLoadBaseIteration; i ++ ) {
         for ( int j = 0; j < TenLoadBaseCount ; j ++) {
             //10 load as base unit 
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             p = (char**) *p;
             
             }
          p = p_list;
     }
     end = monotonic_time();
     rawTime = (end - start);
  //   std::cout <<"raw time" << rawTime<<std::endl;
     double time = (double)rawTime / (ITERATION * TenLoadBaseCount);
     delete [] p_list;
     
     std::cout<<"arraySize: "<<(arraySize * 8) / KB<<"KB strideSize: "<<strideSize<<" taking time.... "<<time <<std::endl;
     

}
    
    
    
int main() {
   
    uint64_t  array_sizes[ARRAYS_NUMS];
    uint64_t  stride_sizes[STRIDES_NUMS];
    
    stride_sizes[0] = 8; //start from 8 , becuase 16 is the cache line 
    array_sizes[0] = KB/2;   // in paper, it is from 512 to bigger than 8MB
    for (int i = 1 ; i < STRIDES_NUMS; i ++) {
        stride_sizes[i] = stride_sizes[i-1] * 4;
    }
    for (int i = 1 ; i < ARRAYS_NUMS ; i++) {
        array_sizes[i] = array_sizes[i-1]<<1;
    }
   
   
   
    //start test 
    std::cout<<"latency 1 ================ "<<std::endl;
    for (int j = 0; j < STRIDES_NUMS; j ++) {
        for ( int i = 0 ; i < ARRAYS_NUMS; i ++) {
            
                latency1 (array_sizes[i], stride_sizes[j]);
         }
     }
     
     //start test 
    std::cout<<"latency 2 ======================="<<std::endl;
    for (int j = 0; j < STRIDES_NUMS; j ++) {
        for ( int i = 0 ; i < ARRAYS_NUMS; i ++) {
            
                latency2 (array_sizes[i], stride_sizes[j]);
         }
     }
     
    //start test 
    std::cout<<"latency 3=============== "<<std::endl;
    for (int j = 0; j < STRIDES_NUMS; j ++) {
        for ( int i = 0 ; i < ARRAYS_NUMS; i ++) {
            
                latency3 (array_sizes[i], stride_sizes[j]);
         }
     }
     return 0;
}