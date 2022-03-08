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

//callback
void *thread_work(void *arg)
{
    int cfd=*(int *)arg;
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

        //
        pthread_detach(threadID);
    }

    close(lfd);
    return 0;
}