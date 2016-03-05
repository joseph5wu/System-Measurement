#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include "../util.h"
#include <fstream>
#include <iostream>
int MB (1024*1024);
long  MAXBUF ( MB * 4);  //4 MB buffer ->>>.big buffer will segmentation fault


using namespace std;

//#define SAMPLES 9
#define SAMPLES 9

typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;
}CLIENT;

/***************************
**server for multi-client
**PF_SETSIZE=1024
****************************/
int main(int argc, char** argv)
{
    double start = 0;
    double end = 0;
    double dataPoints[SAMPLES];
    int cumulation = 0;
    int currentSampleIndex = 0;
    int sizeList[SAMPLES];
    
    sizeList[0]   =  512;
    sizeList[1] =  2048;
    sizeList[2] =  8192;
    sizeList[3] =  32768;
    sizeList[4] =  131072;
    sizeList[5] =  524288;
    sizeList[6] =  2097152;
    sizeList[7] =  8388608;
    sizeList[8] =  33554432;


  


    int i,n,maxi = -1;
    int nready;
    int slisten,sockfd,maxfd=-1,connectfd;

    unsigned int myport,lisnum;

    struct sockaddr_in  my_addr,addr;
    struct timeval tv;

    socklen_t len;
    fd_set rset,allset;

    char buf[MAXBUF + 1];

    CLIENT client[FD_SETSIZE];

    if(argv[1])
        myport = atoi(argv[1]);
    else
        myport = 1234;

    if(argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = FD_SETSIZE;

    if((slisten = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //set socket NO TIME_WAIT
    int on=1;
    if((setsockopt(slisten,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)

    {
        perror("setsocketopt fail");
        std::cout<<std::endl;
        exit(1);
    }

    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(slisten, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(slisten, lisnum) == -1) {
        perror("listen");
        exit(1);
    }

    for(i=0;i<FD_SETSIZE;i++)
    {
        client[i].fd = -1;
    }

    FD_ZERO(&allset);
    FD_SET(slisten, &allset);
    maxfd = slisten;

    printf("Waiting for connections and data...\n");
    while (1)
    {
        rset = allset;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        nready = select(maxfd + 1, &rset, NULL, NULL, &tv);

        if(nready == 0)
            continue;
        else if(nready < 0)
        {
            printf("select failed!\n");
            break;
        }
        else
        {
            if(FD_ISSET(slisten,&rset)) // new connection
            {
                len = sizeof(struct sockaddr);
                if((connectfd = accept(slisten,
                                       (struct sockaddr*)&addr,&len)) == -1)
                {
                    perror("accept() error\n");
                    continue;
                }
                for(i=0;i<FD_SETSIZE;i++)
                {
                    if(client[i].fd < 0)
                    {
                        client[i].fd = connectfd;
                        client[i].addr = addr;
                        printf("Yout got a connection from %s.\n",
                               inet_ntoa(client[i].addr.sin_addr));
                        start = monotonic_time(); 
                        break;
                    }
                }
                if(i == FD_SETSIZE)
                    printf("too many connections");
                FD_SET(connectfd,&allset);
                if(connectfd > maxfd)
                    maxfd = connectfd;
                if(i > maxi)
                    maxi = i;
            }
            else
            {
                for(i=0;i<=maxi;i++)
                {
                    if((sockfd = client[i].fd)<0)
                        continue;
                    if(FD_ISSET(sockfd,&rset))
                    {
                        bzero(buf,MAXBUF + 1);
                        int j_request_type=0;
                        printf("===== start timer for %d",currentSampleIndex);
                        
                        if((n = recv(sockfd,buf,MAXBUF,0)) > 0) {

                            //printf("received data:%s\n from %s size: %lu\n",buf,inet_ntoa(client[i].addr.sin_addr),strlen(buf));
                            printf("received data from %s size: %lu\n",inet_ntoa(client[i].addr.sin_addr),strlen(buf));

                  
             
                            if ((cumulation += n) == sizeList[currentSampleIndex]) {
                                printf("one sample done, cumulate at %d\n",cumulation);
                                printf("=====stop timer for %d\n",currentSampleIndex);
                                end = monotonic_time();
                                dataPoints[currentSampleIndex]=(double)(end - start);
                                cumulation = 0;


                            }

                        }
                        
                        else {

                            printf("disconnected by client!\n");
                            close(sockfd);
                            FD_CLR(sockfd,&allset);
                            client[i].fd = -1;
                            currentSampleIndex ++;
                            if (currentSampleIndex == SAMPLES ) {
                                printf("going to wrap up\n");
                                close(slisten);

                                 for (int i = 0 ; i < SAMPLES ; i++) {
                                        if (sizeList[i] < 1024) {
                                            printf("[%d]IPDataGram SIZE: %d B, time: %fs, SPEED: %f MB/S\n",i,
                                                             sizeList[i],dataPoints[i],(double)sizeList[i]/1024/1024/dataPoints[i]);
                                        }
                                        if (sizeList[i] >= 1024) {
                                            printf("[%d]IPDataGram SIZE: %d KB, time: %fs, SPEED: %f MB/S\n",i,
                                                             sizeList[i]/1024,dataPoints[i],(double)sizeList[i]/1024/1024/dataPoints[i]);
                                        }
                                    }
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }


    close(slisten);
}
