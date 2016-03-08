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
int MB (1024 * 1024 );    //1MB
//WTF ? #define MB 1024*1024 
//#define MAXBUF MB    //1MB
#define MAXBUF MB    //1MB
//#define SAMPLES 2
//#define SAMPLES 8  512b -> 64MB


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

	int TEST_SIZE = atoi(argv[3]);



    int sockfd, len;  
    struct sockaddr_in dest;  
    char buf[MAXBUF + 1];  
    char recBuf [MAXBUF +1];
    fd_set rfds;  
    struct timeval tv;  
    int retval, maxfd = -1;  




    bzero(buf, MAXBUF + 1);  
    for ( int i = 0; i < MAXBUF ; i ++) {
            buf[i] = '0' + i%10;
            if ( (i + 1) % 10 ==0) {
                buf[i] = 'x';
            }
    }
      
    if (argc != 4) {  
            printf("Usage: %s IP Port and IPDataGramLoad",argv[0]);  
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
   
  

    if (TEST_SIZE < MB ) {
    len = send(sockfd, buf, TEST_SIZE , 0);  
	    if (len > 0)  
	        printf("msg:%s send successful Totalbytes: %d\n", buf, len);  
	    else {  
	        printf("msg:'%s  failed!\n", buf);  
	    }  
	} else {
             printf("TEST_SIZE: %d MB:%d \n",TEST_SIZE,MB);  
			int sendTime = (int)TEST_SIZE/MB;
            printf("need to send  %d times\n",sendTime);  
          
			while(sendTime-- > 0)  {
                printf("===================1 MB ======== start\n");  
			len = send(sockfd, buf, MB , 0);  
		    if (len > 0)  
		        printf("msg:%s send successful Totalbytes: %d\n", buf, len);  
		    else {  
		        printf("msg:'%s  failed!\n", buf);  
		    }  
             printf("===================1 MB ======== end\n");  
		}
	}



    close(sockfd);  
    return 0;  
}  
