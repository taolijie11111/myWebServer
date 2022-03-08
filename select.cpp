//mult-load I/O -select
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


int main()
{
    //socket
    int lfd=Socket(AF_INET,SOCK_STREAM,0);

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

    //define fd_set
    fd_set readfds;
    fd_set tmpfds;

    //clear readfds and tmp fds
    FD_ZERO(&readfds);
    FD_ZERO(&tmpfds);

    //add lfd into readfds
    FD_SET(lfd,&readfds);

    int nready;
    int maxfd=lfd;
    int cfd;
    int sockfd;
    char buf[1024];
    int n;
    while (1)
    {
        tmpfds=readfds;
        /*
        tmpfds tell kernel information:
        input: tell kernel to check what fd
        output: tell kernel what fd have changed
        */
        nready=select(maxfd+1,&tmpfds,NULL,NULL,NULL);
        if(nready<0){
            if(errno==EINTR)
            {
                continue;
            }
            break;
        }

        cout<<"nready:"<<nready<<endl;

        //clinet connet apply
        if(FD_ISSET(lfd,&tmpfds)){
            //recive new apply
            cfd=Accept(lfd,NULL,NULL);
            
            //add cfd in readfds
            FD_SET(cfd,&readfds);

            //change select check range 
            if(maxfd<cfd){
                maxfd=cfd;
            }

            if(--nready==0){
                continue;
            }
        }

        //clinet send data apply
        for(int i=lfd+1;i<=maxfd;i++){
            sockfd=i;
            //judge sockfd has changed
            if(FD_ISSET(sockfd,&tmpfds)){
                //read data 
                memset(buf,0x00,sizeof(buf));
                n=Read(sockfd,buf,sizeof(buf));
                if(n<=0){
                    close(sockfd);

                    //delete sockfd from readfds
                    FD_CLR(sockfd,&readfds);
                }
                else{
                    cout<<"n:"<<n<<"buf:"<<buf<<endl;
                    for(int k=0;k<n;k++){
                        buf[k]=toupper(buf[k]);
                    }
                    Write(sockfd,buf,n);
                }
                if(--nready==0)break;
            }
        }

    }
    //close
    close(lfd);

    return 0;
}