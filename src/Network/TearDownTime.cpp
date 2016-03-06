#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <resolv.h>  
#include <stdlib.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <sys/time.h>  
#include <sys/types.h>  
#include "../util.h"
#include <math.h>

#define SAMPLES 10
void updateMin(double &min, double newV) {
    if (newV < min) {
        min = newV;
    }

}

void updateMax(double &max, double newV) {
    if (newV > max) {
        max = newV;
    }

}

double avg(double data[], int n) {
	double total = 0;
	for (int i = 0; i < n; i ++) {
		total += data[i];
	}

	return total/n;
}


double standard_deviation(double data[], int n)
{
    float mean=0.0, sum_deviation=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
    sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);           
}

int main(int argc, char **argv)  
{  
    int sockfd, len;  
    struct sockaddr_in dest;  


    fd_set rfds;  
    struct timeval tv;  
    int retval, maxfd = -1;  


    double  start;
    double  end;
    double  rawTime;
    double  avgTime; //resultTime is the smallest value

    double minTime =1000000;
    double maxTime = -100000;

   
      
    if (argc != 3) {  
            printf("Usage: %s IP Port",argv[0]);  
            exit(0);  
        }  
  

      
   
  
    double dataPoint[SAMPLES];
    double totalTime = 0;
    int i = 0;
    while ( i < SAMPLES) {      
        //start

            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
            perror("Socket");  
            exit(errno);  
        }  
  
    bzero(&dest, sizeof(dest));  
        dest.sin_family = AF_INET;  
        dest.sin_port = htons(atoi(argv[2]));  
        if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {  
            perror(argv[1]);  
            exit(errno);  
        }  
  


      


        if(connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {  
            perror("Connect ");  
            exit(errno);  
        }  
  
    	printf("connect to server...\n");   

  

        start = monotonic_time(); 


        close(sockfd);  


        end = monotonic_time();
  

        rawTime = end - start;
     
        updateMax(maxTime,rawTime);
        updateMin(minTime,rawTime);
        dataPoint[i] = rawTime*1000;
        printf("finish one...\n");   
        i++;

       
        
    }

    printf("\nTCP setup Time min/avg/max/stddev avgTime is  %f/%f/%f/%f ms\n",minTime*1000,avg(dataPoint,SAMPLES),maxTime*1000,standard_deviation(dataPoint,SAMPLES));

    return 0;  
}  
