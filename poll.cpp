//IO Poll
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

#include "wrap.h"

#include <errno.h>

#include <poll.h>

int main()
{
    int i,n;
    int lfd,cfd,nready,maxi,sockfd;
    char buf[1024];
    //socket
    lfd=Socket(AF_INET,SOCK_STREAM,0);

    //Port multiplexing
    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_RCVBUFFORCE,&opt,sizeof(opt));

    //bind
    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

    //listen
    listen(lfd,128);

    //poll
    struct pollfd client[1024];
    for(i=0;i<1024;i++){
        client[i].fd=-1;
    }
    client[0].fd=lfd;
    client[0].events=POLLIN;

    maxi=0;
    while(1)
    {
        nready=poll(client ,maxi+1,-1);
        if(nready<0){
            if(errno==EINTR)continue;
            perror("poll errno");
            exit(1);
        }

        if(client[0].fd==lfd && (client[0].revents & POLLIN))
        {
            cfd=Accept(lfd,NULL,NULL);

            for(i=0;i<1024;i++){
                if(client[i].fd==-1){
                    client[i].fd=cfd;
                    client[i].events=POLLIN;
                    break;
                }
            }

            if(i==1024){
                Close(cfd);
                continue;
            }
            if(maxi<i)maxi=i;

            if(--nready==0)continue;
        }
    }
    for(i=1;i<=maxi;i++)
    {
        if(client[i].fd==-1)continue;

        if(client[i].revents==POLLIN){

        sockfd=client[i].fd;
        memset(buf,0x00,sizeof(buf));
        n=Read(sockfd,buf,sizeof(buf));
        if(n<=0)
        {
            printf("read error or client closed,n==[%d]\n",n);
            close(sockfd);
            client[i].fd=-1;
        }else
        {
            printf("read over,n==[%d],buf==[%s]",n,buf);
            write(sockfd,buf,n);
        }
        }

    }
    return 0;
}

/*
this code have some wrong 
*/