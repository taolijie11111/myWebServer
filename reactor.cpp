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

/*
some do not code
*/

#define _BUF_LEN_ 1024
#define _EVENT_SIZE_ 1024

int gepfd=0;

struct _event{
    int fd;
    int events;
    void (*call_back)(int fd,int events,void *arg);
    void *arg;
    char buf[1024];
    int buflen;
    int epfd;
};

_event myevent[_EVENT_SIZE_+1];

void eventadd(int fd,int events,void (*call_back)(int ,int ,void*),void * arg,_event *ev)
{
    ev->fd=fd;
    ev->events=events;
    ev->call_back=call_back;

    struct epoll_event epv;
    epv.events=events;
    epv.data.ptr=ev;//key idea
    epoll_ctl(gepfd,EPOLL_CTL_ADD,fd,&epv);//on tree 
}

void eventset(int fd,int events,void(*call_back)(int,int ,void*),void *arg,_event *ev)
{
    ev->fd=fd;
    ev->events=events;
    ev->call_back=call_back;

    struct epoll_event epv;
    epv.events=events;
    epv.data.ptr=ev;
    epoll_ctl(gepfd,EPOLL_CTL_MOD,fd,&epv);
}


void readData(int fd,int events,void *arg){
    cout<<"begin call "<<__FUNCTION__<<endl;
    _event *ev=arg;
    ev->buflen=Read(fd,ev->buf,sizeof(ev->buf));
    if(ev->buflen>0)eventset(fd,EPOLLOUT,senddata,arg,ev);
    else if(ev->buflen==0){
        Close(fd);
        eventdel(ev,fd,EPOLLIN);
    }
    
}

void senddata(int fd,int events,void *arg)
{
    cout<<"begin call:"<<__FUNCTION__<<endl;
    _event *ev=arg;
    Write(fd,ev->buf,ev->buflen);
    eventset(fd,EPOLLIN,readData,arg,ev);
}

void initAccept(int fd,int events,void*arg)
{
    cout<<"begin call "<<__FUNCTION__<<"\tgepfd:"<<gepfd<<endl;
    int i;
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);

    int cfd=Accept(fd,(struct sockaddr*)&addr,&len);

    for(i=0;i<_EVENT_SIZE_;i++)
    {
        if(myevent[i].fd==0)break;
    }

    eventadd(cfd,EPOLLIN,readData,&myevent[i],&myevent[i]);
}


int main ()
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);

    int opt=1;
    setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    struct sockaddr_in serv;
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));

    Listen(lfd,128);

    int gepfd=epoll_create(1024);
    cout<<"gepfd"<<gepfd<<endl;

    struct epoll_event events[1024];

    eventadd(lfd,EPOLLIN,initAccept,&myevent[_EVENT_SIZE_],&myevent[_EVENT_SIZE_]);

    while(1)
    {
        int nready=epoll_wait(gepfd,events,1024,-1);
        if(nready<0)perr_exit("epoll_wait error");
        else if(nready>0){
            int i=0;
            for(i=0;i<nready;i++)
            {

                _event *xe=events[i].data.ptr;
                cout<<"fd:"<<xe->fd<<endl;

                if(xe->events & events[i].events)xe->call_back(xe->fd,xe->events,xe);//use call_back function from events
            }
        }
    }
    Close(lfd);
    return 0;
}

