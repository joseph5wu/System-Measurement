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
#define MAXBUF 1024  
#define PACKAGE_SIZE 56
#define SAMPLES 100
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
    char buf[MAXBUF + 1];  
    char recBuf [MAXBUF +1];
    fd_set rfds;  
    struct timeval tv;  
    int retval, maxfd = -1;  


    double  start;
    double  end;
    double  rawTime;
    double  avgTime; //resultTime is the smallest value

    double minTime =1000000;
    double maxTime = -100000;

    bzero(buf, MAXBUF + 1);  
    for ( int i = 0; i < PACKAGE_SIZE ; i ++) {
            buf[i] = '0' + i%10;
            if ( (i + 1) % 10 ==0) {
                buf[i] = 'x';
            }
    }
      
    if (argc != 3) {  
            printf("Usage: %s IP Port",argv[0]);  
            exit(0);  
        }  
  
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
   
  
    double dataPoint[SAMPLES];
    double totalTime = 0;
    int samples = SAMPLES;
    while (samples >0) {      
        //start
        start = monotonic_time(); 
        len = send(sockfd, buf, strlen(buf) , 0);  
        if (len > 0)  
            printf("msg:%s send successful Totalbytes: %d\n", buf, len);  
        else {  
            printf("msg:'%s  failed!\n", buf);  
            continue;
        }  

        printf("Start receiving echo message\n");  
        bzero(recBuf, MAXBUF + 1);  

        int tempRec = 0;
        while (tempRec != PACKAGE_SIZE) {
            len = recv(sockfd, recBuf, MAXBUF, 0);
            if (len > 0)   {
                printf("recv:'%s, total: %d \n", buf, len);  
                tempRec = tempRec + len;
            }
            else    
            {  
                if (len < 0)   
                    printf("recv failed rrno:%d锛宔rror msg: '%s'\n", errno, strerror(errno));  
                else  
                    printf("other exit erminal chat\n");  
                    
                continue;
            }  
        }

        end = monotonic_time();
        samples --;

        rawTime = end - start;
        totalTime += rawTime;
        updateMax(maxTime,rawTime);
        updateMin(minTime,rawTime);
        dataPoint[samples-1] = rawTime*1000;
        
    }

    printf("\nTCP round-trip min/avg/max/stddev avgTime is  %f/%f/%f/%f \n",minTime*1000,totalTime/SAMPLES*1000,maxTime*1000,standard_deviation(dataPoint,SAMPLES));
    close(sockfd);  
    return 0;  
}  
