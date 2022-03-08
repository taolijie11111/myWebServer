//多进程服务器
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;
#include <netinet/in.h>
#include <assert.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "wrap.h"

//Q1:int g_lfd;
//callback
void *thread_work(void *arg)
{
    //Q3
    //sleep(20);
    int cfd=*(int *)arg;
    cout<<"cfd=="<<cfd<<endl;
   //Q1: close(g_lfd);
    int n;
    char buf[1024];
    while (1)
    {
        //read
        n=Read(cfd,buf,sizeof(buf));
        assert(n>0);
        //closr cfd
        cout<<"n="<<n<<endl;
        for(int i=0;i<n;i++)
        {
            buf[i]=toupper(buf[i]);
        }
        Write(cfd,buf,n);
    }
    close(cfd);
    pthread_exit(NULL);
}
int main()
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
   //Q1: g_lfd=lfd;//save lfd 


    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    Listen(lfd,128);

    int cfd;
    pthread_t threadID;
    while (1)
    {
        cfd=Accept(lfd,NULL,NULL);

        //create child thread
        pthread_create(&threadID,NULL,thread_work,&cfd);

         //Q2:
        //close(cfd);

        //
        pthread_detach(threadID);

       
    }

    close(lfd);
    return 0;
}

/*
问题：
1：子线程能否关闭lfd
A:x
R:子线程和主线程共享文件描述符，而不是复制的

2：主线线程能否关闭cfd
A:x
R:test: mult-thread-test.cpp:28: void* thread_work(void*): Assertion `n>0' failed.
Aborted (core dumped)
子线程和主线程共享cfd，而不是复制的,if father close cfd ,child close cfd

3：多个子线程共享cfd，会有什么问题发生？
覆盖wenti 

struct INFO
{
   int cfd;
   pthread_t threadID; 
   struct  sockaddr_in client;
};
struct  INFO info[100];


for(i=0;i<100;i++)
{
    if(info[i].cfd==-1)
    {
        //it can be use
    }
}
if(i==100)
{
    //refuse conneted it
    close(cfd);
}
*/
