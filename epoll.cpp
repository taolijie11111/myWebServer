#include <iostream>
using namespace std;
#include <sys/epoll.h>
#include<netinet/in.h>
#include "wrap.h"
#include <assert.h>
//memset
#include <string.h>

#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>

int main()
{
    int ret,n,i,k,nready,lfd,cfd,epfd,sockfd;
    char buf[1024];
    socklen_t socklen;
    struct epoll_event ev;
    struct epoll_event events[1024];

    lfd=Socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

    Listen(lfd,128);

    //data

    //create a epoll tree 
    epfd=epoll_create(1024);
    assert(epfd>=0);

    cout<<"OK"<<endl;

    //lfd | epoll tree
    ev.events=EPOLL_CTL_ADD;
    ev.data.fd=lfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);

    while (1)
    {
        nready=epoll_wait(epfd,events,1024,-1);
        cout<<"nready="<<nready<<endl;
        if(nready<0)
        {
            if(errno==EINTR)continue;
            perror("epoll_wait error");
            break;
        }
        for(i=0;i<nready;i++){
            //client come
            sockfd=events[i].data.fd;
            cout<<"sockfd="<<sockfd<<endl;
            if(sockfd==lfd){//here wrang = ->  ==
                cfd=Accept(lfd,NULL,NULL);
                

                //up new cfd on tree

                ev.data.fd=cfd;
                ev.events=EPOLLIN | EPOLLET;//ET 
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);

                //将cfd设置为非阻塞
                int flag=fcntl(cfd,F_GETFL);
                flag |=O_NONBLOCK;
                fcntl(cfd,F_SETFL,flag);

                continue;
            }

            //client data come
            memset(buf,0x00,sizeof(buf));
            n=Read(sockfd,buf,sizeof(buf));
            //add n=-1 
            if(n<=0){
                Close(sockfd);
                //deletet note
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                cout<<"delete notes form tree\n\n"<<endl;
                break;
            }else{
                cout<<"n: "<<n<<"\tbuf: "<<buf<<endl;
                for(k=0;k<n;k++){
                    buf[k]=toupper(buf[k]);
                }
                Write(sockfd,buf,n);
                cout<<"send over\n\n"<<endl;
            }
        }
    }
    
    Close(epfd);
    Close(lfd);

    return 0;
}


/*
epoll 
--LT :default mode ,if it don't read all the data epoll_wait will continue to read 
--ET :only once  we can set cycle

*/
