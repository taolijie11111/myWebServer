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
#include "wrap.h"


int main()
{
    int lfd=Socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv;
    bzero(&serv,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_port=htons(8888);
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    Bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    listen(lfd,128);

    pid_t pid;
    int cfd;
    struct sockaddr_in client;
    socklen_t len;

    while (1)
    {
        //accept new linked
        char SIP[16];
        memset(SIP,0x00,sizeof(SIP));
        len=sizeof(client);
        cfd=Accept(lfd,(struct sockaddr *)&client,&len);
       cout<<"client:IP:"<<inet_ntop(AF_INET,&client.sin_addr.s_addr,SIP,sizeof(SIP))<<"\tPORT:"<<ntohs(client.sin_port)<<endl;

        pid=fork();

        assert(pid>=0);

        if(pid>0){
            close(cfd);
        }
        else if (pid==0)
        {
            close(lfd);

            int n;
            char buf[1024];

            while (1)
            {

                n=Read(cfd,buf,sizeof(buf));

                assert(n>0);
                 cout<<"client:"<<ntohs(client.sin_port)<<"------>n:"<<n<<"\tbuf:"<<buf<<endl;

                for(int i=0;i<n;i++){
                    buf[i]=toupper(buf[i]);
                }

                write(cfd,buf,n);

            }
            close(cfd);
            exit(0);
        }
    }
    close(lfd);
    return 0;
}

